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
        m_ConsoleLoggerId = LogManager::GetInstance().RegisterLogger(&m_ConsoleLogger, &ConsoleLogger::Log);
        LogInfo(LogDefault, "CoreModule");
    }

    void CoreModule::DoShutdown()
    {
        LogManager::GetInstance().UnregisterLogger(m_ConsoleLoggerId);
        m_SingletonStorer.reset();
    }
}