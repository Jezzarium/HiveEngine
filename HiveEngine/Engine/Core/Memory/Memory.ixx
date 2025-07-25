export module Core.Memory;
import Core.Singleton;
import Core.Functor;
import std;

namespace hive
{
    constexpr bool MEMORY_ENABLED = true;

    struct AllocationHeader
    {
        const char* file{nullptr};
        unsigned int line{0};
        unsigned int size{0};
    };

    export class MemoryManager final : public Singleton<MemoryManager>
    {
    public:
        using AllocFunctor = Functor<void, unsigned int, std::source_location, void*>;
        using FreeFunctor = Functor<void, void*>;
        using MemoryCallbackId = unsigned int;


        MemoryManager();
        ~MemoryManager();

        [[nodiscard]] void* Allocate(unsigned int size, std::source_location loc);
        void Deallocate(void* p);

        [[nodiscard]] MemoryCallbackId RegisterCallbacks(AllocFunctor &&functorAlloc, FreeFunctor &&functorFree);
        void RemoveCallbacks(MemoryCallbackId id);

    private:
        std::pmr::unordered_set<AllocationHeader*> m_Allocations;
        std::vector<std::tuple<MemoryCallbackId, AllocFunctor, FreeFunctor>> m_MemoryCallbacks;
        unsigned int m_IdCounter{0};
    };

    template <typename T>
    struct AllocProxy
    {
        explicit AllocProxy(T* ptr) : m_Ptr(ptr)
        {
        }

        template <typename... Args>
        T* operator()(Args... args)
        {
            return new(m_Ptr) T(std::forward<Args>(args)...);
        }

        T* m_Ptr;
    };

    export template <typename T>
    AllocProxy<T> hive_new(std::source_location loc = std::source_location::current())
    {
        void* ptr = nullptr;
        if constexpr (MEMORY_ENABLED)
        {
            ptr = MemoryManager::GetInstance().Allocate(sizeof(T), loc);
        }
        else
        {
            ptr = std::malloc(sizeof(T));
        }
        return AllocProxy<T>(static_cast<T*>(ptr));
    }

    export template <typename T>
    void hive_delete(T* ptr)
    {
        ptr->~T();
        if constexpr (MEMORY_ENABLED)
        {
            MemoryManager::GetInstance().Deallocate(ptr);
        }
        else
        {
            std::free(ptr);
        }
    }
}
