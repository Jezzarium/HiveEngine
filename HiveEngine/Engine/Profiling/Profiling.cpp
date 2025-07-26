module Profiling;

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
        Tracy::StartupProfiler();
#endif
    }

    void ProfilingModule::DoShutdown()
    {
#if defined(PROFILING_ENABLED)
        Tracy::StartupProfiler();
        MemoryManager::GetInstance().RemoveCallbacks(m_MemoryCallbackId);
        hive_delete(m_SingletonStorer);
#endif
    }

    struct ProfilingSingletonStorer : SingletonStorer<ProfilingSingletonStorer>
    {
    };

    void ProfilingModule::CreateSingletonStorer()
    {
        m_SingletonStorer = hive_new<ProfilingSingletonStorer>()();
    }

#if defined(PROFILING_ENABLED)
    ScopedProfiler::ScopedProfiler(const char *name, std::source_location location)
    {
        m_ZoneLocationData = {
            name != nullptr ? name : location.function_name(),
            location.function_name(),
            location.file_name(),
            location.line(),
            0
        };

        m_ZoneContext = Tracy::BeginZone(&m_ZoneLocationData);
    }

    ScopedProfiler::~ScopedProfiler()
    {
        Tracy::EndZone(m_ZoneContext);
    }
#endif
}
