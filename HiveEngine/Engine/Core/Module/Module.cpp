module Core.Module;

namespace hive
{
    void ModuleBase::Configure()
    {
        DoConfigure(m_Context);
    }

    void ModuleBase::Initialize()
    {
        DoInitialize();
        m_IsInitialized = true;
    }

    void ModuleBase::Shutdown()
    {
        DoShutdown();
    }

    bool ModuleBase::CanInitialize(const std::unordered_set<std::string>& initModulesNames) const
    {
        int depCount {0};
        for (auto depName : m_Context.GetDependencies())
        {
            if (initModulesNames.find(depName) != initModulesNames.end())
            {
                depCount++;
            }
        }

        return depCount == m_Context.GetDependencies().size();
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
        std::vector<ModuleContext> contexts;
        contexts.reserve(10);
        for (auto& module : m_Modules)
        {
            module->Configure();
        }

        //TODO: do some stuff here
        //How to sort them by dependency
    }

    void ModuleRegistry::InitModules()
    {
        std::unordered_set<std::string> initModulesNames;
        std::vector<std::string> modulesToInit;

        for (const auto& module : m_Modules)
        {
            modulesToInit.push_back(module->GetName());
        }

        while (!modulesToInit.empty())
        {
            bool anyModuleInitialized = false;

            for (const auto& module : m_Modules)
            {
                const std::string& name = module->GetName();

                // Only consider modules still in the list
                auto it = std::find(modulesToInit.begin(), modulesToInit.end(), name);
                if (it != modulesToInit.end() && module->CanInitialize(initModulesNames))
                {
                    module->Initialize();
                    initModulesNames.insert(name);
                    modulesToInit.erase(it);
                    anyModuleInitialized = true;
                    break; // Optional: restart loop after initializing a module
                }
            }

            if (!anyModuleInitialized)
            {
                throw std::runtime_error("Circular dependency detected or missing dependency.");
            }
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
