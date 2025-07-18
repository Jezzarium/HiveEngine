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
        void AddDependency() {}
    };

    export class Module
    {
    public:
        Module() = default;
        virtual ~Module() = default;

        ModuleContext Configure(); //TODO: pass a context for it to append it's dependency
        void Initialize();
        void Shutdown();

    protected:
        virtual void DoConfigure(ModuleContext& context) {}
        virtual void DoInitialize() {}
        virtual void DoShutdown() {}
    };

    export class ModuleRegistry
    {
    public:
        static inline ModuleRegistry& GetInstance()
        {
            static ModuleRegistry instance;
            return instance;
        }

        using ModuleFactoryFn = std::unique_ptr<Module>(*)();
        void RegisterModule(ModuleFactoryFn fn);

        void CreateModules();
        void ConfigureModules();
        void InitModules();
        void ShutdownModules();

    private:
        std::vector<ModuleFactoryFn> m_ModuleFactories;
        std::vector<std::unique_ptr<Module>> m_Modules;
    };

    export template<typename T>
    class ModuleAutoRegister
    {
    public:
        ModuleAutoRegister()
        {
            hive::ModuleRegistry::GetInstance().RegisterModule([]() -> std::unique_ptr<hive::Module>
            {
                return std::make_unique<T>();
            });

        }
    };
}
