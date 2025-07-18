export module Core.ImplOwner;
import Core.Memory;
namespace hive
{

    export template<typename T>
    class ImplOwner
    {
    public:
        ImplOwner()
        {
            m_Ptr = hive_new<T>()();
        }

        ~ImplOwner()
        {
            hive_delete(m_Ptr);
            m_Ptr = nullptr;
        }

        T* Get() { return m_Ptr; }
        T* operator->() { return m_Ptr; }
        T& operator*() { return *m_Ptr; }

        ImplOwner(const ImplOwner&) = delete;
        ImplOwner& operator=(const ImplOwner&) = delete;
        ImplOwner(ImplOwner&&) = delete;
        ImplOwner& operator=(ImplOwner&&) = delete;
    private:
        T* m_Ptr{nullptr};
    };
}