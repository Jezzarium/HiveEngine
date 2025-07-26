import Engine;

import std;
import Hive.Tracy;
int main()
{
    hive::ModuleRegistry::GetInstance().CreateModules();
    hive::ModuleRegistry::GetInstance().ConfigureModules();
    hive::ModuleRegistry::GetInstance().InitModules();

    hive::ConsoleLogger logger;
    auto logger_id = hive::LogManager::GetInstance().RegisterLogger(&logger, &hive::ConsoleLogger::Log);
    while (true)
    {
        {
            hive::ScopedProfiler _p{"WhileLoop"};
            std::this_thread::sleep_for(std::chrono::milliseconds(16));
        }
    }

    hive::LogManager::GetInstance().UnregisterLogger(logger_id);

    hive::ModuleRegistry::GetInstance().ShutdownModules();
}