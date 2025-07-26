export module Profiling;

import Core;
import std;
import Hive.Tracy;
namespace hive
{
    export class ProfilingModule : public Module<ProfilingModule>
    {
    public:
        static std::string_view GetStaticClassName() { return "ProfilingModule"; }
        const char *GetName() const override { return "ProfilingModule"; }

    protected:
        void DoConfigure(ModuleContext &context) override;
        void DoInitialize() override;
        void DoShutdown() override;

    private:
        void CreateSingletonStorer();

        SingletonStorerBase* m_SingletonStorer{nullptr};
        MemoryManager::MemoryCallbackId m_MemoryCallbackId{0};
    };





#if PROFILING_ENABLED
    export class ScopedProfiler
    {
    public:
        explicit ScopedProfiler(const char* name = nullptr, std::source_location location = std::source_location::current());
        ~ScopedProfiler();
    private:
        Tracy::ZoneContext m_ZoneContext;
        Tracy::ZoneLocationData m_ZoneLocationData;
    };

#else
    export class ScopedProfiler
    {
    public:
        explicit inline constexpr ScopedProfiler() noexcept {};
        constexpr inline ~ScopedProfiler() noexcept = default;
    };

#endif
}