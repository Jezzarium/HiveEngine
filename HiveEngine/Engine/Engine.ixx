export module Engine;

export import Core;
export import Platform;
import Profiling;

import std;
namespace hive
{
    export class EngineModule : public Module
    {
    public:
        EngineModule()
        {
            std::cout << "Engine module" << std::endl;
        }

    protected:
        void DoConfigure(ModuleContext& context) override
        {
            context.AddDependency<CoreModule>();
            context.AddDependency<PlatformModule>();
        }
    };
}

namespace
{
    const hive::ModuleAutoRegister<hive::EngineModule> s_EngineModule;
}
