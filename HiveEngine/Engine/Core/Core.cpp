module Core;

namespace hive
{
    struct CoreSingletonStorer : SingletonStorer<CoreSingletonStorer>
    {
        LogManager m_LogManager;
        MemoryManager m_MemoryManager;
    };

    std::unique_ptr<SingletonStorerBase> CreateCoreSingletonStorer()
    {
        //This should technically be the only new in our codebase
        CoreSingletonStorer* storer = new CoreSingletonStorer();
        return std::unique_ptr<CoreSingletonStorer>(storer);
    }

    void CoreModule::DoInitialize()
    {
        m_SingletonStorer = CreateCoreSingletonStorer();
    }

    void CoreModule::DoShutdown()
    {
    }
}

namespace {
    const hive::ModuleAutoRegister<hive::CoreModule> s_CoreModuleAutoRegister;
}
