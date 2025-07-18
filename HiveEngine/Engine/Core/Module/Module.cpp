module Core.Module;

namespace hive
{
    ModuleContext Module::Configure()
    {
        ModuleContext context;
        DoConfigure(context);
        return context;
    }

    void Module::Initialize()
    {
        DoInitialize();
    }

    void Module::Shutdown()
    {
        DoShutdown();
    }

    void ModuleRegistry::RegisterModule(ModuleFactoryFn fn)
    {
        m_ModuleFactories.push_back(fn);
    }

    void ModuleRegistry::CreateModules()
    {
        for (const auto& module_factory : m_ModuleFactories)
        {
            m_Modules.push_back(module_factory());
        }
    }

    void ModuleRegistry::ConfigureModules()
    {
        for (auto& module : m_Modules)
        {
            ModuleContext context = module->Configure();
        }
    }

    void ModuleRegistry::InitModules()
    {
        for (const auto& module : m_Modules)
        {
            module->Initialize();
        }
    }

    void ModuleRegistry::ShutdownModules()
    {
        for (const auto& module : m_Modules)
        {
            module->Shutdown();
        }
    }
}
