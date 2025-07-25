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

        std::unordered_set<std::string> initializedModules;
        std::vector<std::unique_ptr<ModuleBase>> orderedModules;

        std::vector<std::unique_ptr<ModuleBase>> remainingModules = std::move(m_Modules);

        while (!remainingModules.empty())
        {
            bool progress = false;

            for (auto it = remainingModules.begin(); it != remainingModules.end(); ++it)
            {
                if ((*it)->CanInitialize(initializedModules))
                {
                    initializedModules.insert((*it)->GetName());
                    orderedModules.push_back(std::move(*it));
                    remainingModules.erase(it);
                    progress = true;
                    break;
                }
            }

            if (!progress)
            {
                // Circular dependency or unsatisfiable constraint
                throw std::runtime_error("Unresolvable module dependency detected.");
            }
        }

        m_Modules = std::move(orderedModules);
    }

    void ModuleRegistry::InitModules()
    {
        for (auto &module : m_Modules)
        {
            module->Initialize();
        }
    }

    void ModuleRegistry::ShutdownModules()
    {
        for (int i = m_Modules.size() - 1; i >= 0; i--)
        {
            m_Modules[i]->Shutdown();
        }
    }
}
