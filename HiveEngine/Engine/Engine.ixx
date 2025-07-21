export module Engine;

export import Core;
export import Platform;
export import Profiling;

import std;
namespace hive
{
    export class EngineModule : public Module<EngineModule>
    {
    public:
        const char *GetName() const override { return "EngineModule"; }

    protected:
        void DoConfigure(ModuleContext& context) override
        {
            context.AddDependency<CoreModule>();
            context.AddDependency<PlatformModule>();
            context.AddDependency<ProfilingModule>();
        }

        void DoInitialize() override
        {
            LogInfo(LogDefault, "Engine module initialized");
        }
    };
}