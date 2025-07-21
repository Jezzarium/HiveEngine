export module Profiling;

import Core;
import std;
namespace hive
{
    export class ProfilingModule : public Module<ProfilingModule>
    {
    public:
        static std::string_view GetStaticClassName() { return "ProfilingModule"; }
        const char *GetName() const override { return "ProfilingModule"; }

    protected:
        void DoConfigure(ModuleContext &context) override
        {
            context.AddDependency<CoreModule>();
        }

        void DoInitialize() override
        {
            LogInfo(LogDefault, "ProfilingModule");
        }
    };

}