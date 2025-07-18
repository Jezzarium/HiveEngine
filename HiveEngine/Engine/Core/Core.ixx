export module Core;

export import Core.Logging;
export import Core.Logging.Logger;
export import Core.Memory;
export import Core.Assert;
export import Core.Module;
export import Core.ImplOwner;
export import Core.Functor;

import std;
namespace hive
{
    export class CoreModule final : public Module
    {
    public:
        CoreModule()
        {
            std::cout << "Core module" << std::endl;
        }
    protected:
        void DoInitialize() override;
        void DoShutdown() override;

    private:
        std::unique_ptr<SingletonStorerBase> m_SingletonStorer;

    };
}


