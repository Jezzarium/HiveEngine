export module Core.Module;
import std;

namespace hive
{
    export struct SingletonStorerBase
    {

    };

    export template<typename T>
    struct SingletonStorer : SingletonStorerBase
    {
        std::unique_ptr<T> m_Instance;
    };

    export class ModuleContext
    {
    public:
        template<typename T>
        void AddDependency()
        {
            m_Dependencies.emplace_back(typeid(T).name());
        }

        const std::vector<std::string> &GetDependencies() const { return m_Dependencies; }
    private:
        std::vector<std::string> m_Dependencies;
    };

    export class ModuleBase
    {
    public:
        ModuleBase() = default;

        virtual ~ModuleBase() = default;

        void Configure(); //TODO: pass a context for it to append it's dependency
        void Initialize();

        void Shutdown();

        bool CanInitialize(const std::unordered_set<std::string> &initModulesNames) const;

        bool IsInitialized() const { return m_IsInitialized; }

        virtual const char *GetName() const = 0;

    protected:
        virtual void DoConfigure(ModuleContext &context)
        {
        }

        virtual void DoInitialize()
        {
        }

        virtual void DoShutdown()
        {
        }

    private:
        ModuleContext m_Context;
        bool m_IsInitialized{false};
    };


    export class ModuleRegistry
    {
    public:
        static inline ModuleRegistry& GetInstance()
        {
            static ModuleRegistry instance;
            return instance;
        }

        using ModuleFactoryFn = std::unique_ptr<ModuleBase>(*)();
        void RegisterModule(ModuleFactoryFn fn);

        void CreateModules();
        void ConfigureModules();
        void InitModules();
        void ShutdownModules();

    private:
        std::vector<ModuleFactoryFn> m_ModuleFactories;
        std::vector<std::unique_ptr<ModuleBase>> m_Modules;
    };

    export template<typename T>
    class ModuleAutoRegister
    {
    public:
        ModuleAutoRegister()
        {
            hive::ModuleRegistry::GetInstance().RegisterModule([]() -> std::unique_ptr<hive::ModuleBase>
            {
                return std::make_unique<T>();
            });

        }
    };

    export template<typename T>
    class Module : public ModuleBase
    {
        static inline ModuleAutoRegister<T> s_AutoRegister;
    };
}
