export module Platform;

export import Platform.Window;

import Core;
import std;
namespace hive
{
    export class PlatformModule : public Module
    {
    public:
        PlatformModule()
        {
            std::cout << "Platform module" << std::endl;
        }
    protected:
        void DoConfigure(ModuleContext& context) override
        {
            context.AddDependency<CoreModule>();
        }
    };

}
 namespace
{
    const hive::ModuleAutoRegister<hive::PlatformModule> s_PlatformModule;
}