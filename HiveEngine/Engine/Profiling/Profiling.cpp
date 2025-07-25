module Profiling;
import HiveTracy;

namespace hive
{
#if defined(PROFILING_ENABLED)
    #define PROFILING_ONLY(X) X
#else
    #define PROFILING_ONLY(X)
#endif

    void ProfilingModule::DoConfigure(ModuleContext &context)
    {
        context.AddDependency<CoreModule>();
    }

    void ProfilingModule::DoInitialize()
    {
#if defined(PROFILING_ENABLED)
        LogInfo(LogDefault, "ProfilingModule");
        CreateSingletonStorer();
#endif
    }

    void ProfilingModule::DoShutdown()
    {
        MemoryManager::GetInstance().RemoveCallbacks(m_MemoryCallbackId);
        hive_delete(m_SingletonStorer);
    }

    struct ProfilingSingletonStorer : SingletonStorer<ProfilingSingletonStorer>
    {
        PROFILING_ONLY(HiveTracy::HiveTracyProfiler m_tracyProfiler);
    };

    void ProfilingModule::CreateSingletonStorer()
    {
        m_SingletonStorer = hive_new<ProfilingSingletonStorer>()();
    }

#if defined(PROFILING_ENABLED)
    ScopedProfiler::ScopedProfiler(std::source_location location)
    {

    }

    ScopedProfiler::~ScopedProfiler()
    {
    }
#endif
}
