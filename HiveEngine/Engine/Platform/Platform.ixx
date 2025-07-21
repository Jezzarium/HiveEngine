export module Platform;

export import Platform.Window;

import Core;
import std;
namespace hive
{
    export class PlatformModule : public Module<PlatformModule>
    {
    public:
        PlatformModule()
        {
            std::cout << "Platform module" << std::endl;
        }
        const char * GetName() const override { return "PlatformModule"; }

    protected:
        void DoConfigure(ModuleContext& context) override
        {
            context.AddDependency<CoreModule>();
        }

        void DoInitialize() override
        {
            LogInfo(LogDefault, "Initialize PlatformModule");
        }

    public:
    };

}

