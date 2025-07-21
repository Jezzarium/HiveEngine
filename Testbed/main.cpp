import Engine;

int main()
{
    hive::ModuleRegistry::GetInstance().CreateModules();
    hive::ModuleRegistry::GetInstance().ConfigureModules();
    hive::ModuleRegistry::GetInstance().InitModules();

    hive::ConsoleLogger logger;
    auto logger_id = hive::LogManager::GetInstance().RegisterLogger(&logger, &hive::ConsoleLogger::Log);

    hive::LogManager::GetInstance().UnregisterLogger(logger_id);

    hive::ModuleRegistry::GetInstance().ShutdownModules();
}