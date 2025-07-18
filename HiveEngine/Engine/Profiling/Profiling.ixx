export module Profiling;

import Core;
import std;
namespace hive
{
    class Tracy
    {
    public:
        void Alloc(unsigned int size, std::source_location loc, void* ptr){}
        void Free(void* ptr){}
    };

    export class ProfilingModule : public Module
    {
    public:
        void DoConfigure(ModuleContext& context) override;
        void DoInitialize() override;
        void DoShutdown() override;

    private:
        Tracy m_tracy;
    };

    void ProfilingModule::DoConfigure(ModuleContext& context)
    {
        context.AddDependency<CoreModule>();
    }

    void ProfilingModule::DoInitialize()
    {
        MemoryManager::GetInstance().RegisterCallbacks({&m_tracy, &Tracy::Alloc}, {&m_tracy, &Tracy::Free});
    }

    void ProfilingModule::DoShutdown()
    {
    }
}

namespace
{
    hive::ModuleAutoRegister<hive::ProfilingModule> s_ProfilingModule;
}
