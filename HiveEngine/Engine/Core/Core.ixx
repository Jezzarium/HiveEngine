export module Core;

export import Core.Logging;
export import Core.Logging.Logger;
export import Core.Memory;
export import Core.Assert;
export import Core.Module;
export import Core.ImplOwner;
export import Core.Functor;
export import Core.Singleton;

import std;
namespace hive
{
    export class CoreModule : public Module<CoreModule>
    {
    public:
        static std::string_view GetStaticClassName() { return "CoreModule"; }
        const char * GetName() const override { return "CoreModule"; }
    protected:
        void DoInitialize() override;
        void DoShutdown() override;

    private:
        std::unique_ptr<SingletonStorerBase> m_SingletonStorer{nullptr};
        ConsoleLogger m_ConsoleLogger;
        LogManager::LoggerId m_ConsoleLoggerId{0};
    };
}


