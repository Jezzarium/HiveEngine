export module Platform;

export import Platform.Window;

import Core;
import std;
namespace hive
{
    export class PlatformModule : public Module<PlatformModule>
    {
    public:
        static std::string_view GetStaticClassName() { return "PlatformModule"; }

        const char * GetName() const override { return "PlatformModule"; }

    protected:
        void DoConfigure(ModuleContext& context) override
        {
            context.AddDependency<CoreModule>();
        }

        void DoInitialize() override
        {
            LogInfo(LogDefault, "PlatformModule");
        }
    };

}

