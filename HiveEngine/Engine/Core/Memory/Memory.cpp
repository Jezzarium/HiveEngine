module Core.Memory;
import Core.Logging;
import Core.Assert;

namespace hive
{
    constexpr LogCategory LogMemory{"Memory"};

    MemoryManager::MemoryManager() = default;

    MemoryManager::~MemoryManager()
    {
        if (m_Allocations.empty()) return;

        LogWarning(LogMemory, "Memory leak detected");
        for (auto allocation : m_Allocations)
        {
            LogWarning(LogMemory, std::format("\t {}:{}", allocation->file, allocation->line).c_str());
        }
    }

    void* MemoryManager::Allocate(unsigned int size, std::source_location loc)
    {
        void* ptr = std::malloc(size + sizeof(AllocationHeader));
        if (ptr == nullptr)
        {
            LogError(LogMemory, "MemoryManager::Allocate() failed allocating memory");
            return nullptr;
        }

        AllocationHeader* allocation = reinterpret_cast<AllocationHeader*>(ptr);
        allocation->file = loc.file_name();
        allocation->line = loc.line();
        allocation->size = size;

        m_Allocations.insert(allocation);

        void* offset_ptr = static_cast<void*>(static_cast<char*>(ptr) + sizeof(AllocationHeader));

        for (auto& [id, allocFn, freeFn] : m_MemoryCallbacks)
        {
            allocFn(size, loc, offset_ptr);
        }

        return offset_ptr;
    }

    void MemoryManager::Deallocate(void* ptr)
    {
        for (auto& [id, allocFn, freeFn] : m_MemoryCallbacks)
        {
            freeFn(ptr);
        }

        void* original_ptr = static_cast<void*>(static_cast<char*>(ptr) - sizeof(AllocationHeader));
        AllocationHeader* allocation = reinterpret_cast<AllocationHeader*>(original_ptr);



        m_Allocations.erase(allocation);
        std::free(original_ptr);
    }

    MemoryManager::MemoryCallbackId MemoryManager::RegisterCallbacks(AllocFunctor &&functorAlloc,
                                                                     FreeFunctor &&functorFree)
    {
        m_MemoryCallbacks.emplace_back(std::make_tuple(m_IdCounter, std::move(functorAlloc), std::move(functorFree)));
        return m_IdCounter++;
    }

    void MemoryManager::RemoveCallbacks(MemoryCallbackId id)
    {
        for (int i = 0; i < m_MemoryCallbacks.size(); i++)
        {
            const auto callback = m_MemoryCallbacks[i];
            const auto callbackId = std::get<MemoryCallbackId>(callback);

            if (callbackId == id)
            {
                m_MemoryCallbacks.erase(m_MemoryCallbacks.begin() + i);
                return;
            }
        }

        //TODO: assert the id was not found
    }
}

