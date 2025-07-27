#include "LogManager.h"
#include "hive_internal.h"

#include <chrono>
#include <cstdarg>
#include <iomanip>
// hive::LogManager::LoggerID hive::LogManager::AddCallback(LoggerCallback callback)
// {
//     const LoggerID id = id_counter;
//     p_callbacks[id] = std::move(callback);
//     id_counter++;
//     return id;
// }

void hive::LogManager::Init()
{

}

void hive::LogManager::Shutdown()
{
    for (auto [id, logger] : p_callbacks)
    {
        HV_FREE_OOP logger;
    }
}
hive::LogManager::LoggerID hive::LogManager::AddLogger(const ILogger *logger)
{
    const LoggerID id = m_idCounter;
    p_callbacks[id] = logger;
    m_idCounter++;
    return id;
}

void hive::LogManager::RemoveCallback(const LoggerID id)
{
    if (p_callbacks.contains(id))
    {
        // HV_FREE_OOP p_callbacks[id];
        p_callbacks.erase(id);
    }
}

void hive::LogManager::SetLogLevel(LogLevel level)
{
    m_logLevel = level;
}

void hive::LogManager::Log(LogLevel level, const char* msg, ...) const
{
    //TODO make it thread safe with a message queue and flush it every frame or 2 idk
    if (level < m_logLevel)
        return;

    LogEntry entry{};
    entry.level = level;

    auto now = std::chrono::system_clock::now();
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);
    std::tm now_tm = *std::localtime(&now_time);
    std::stringstream ss;
    ss << std::put_time(&now_tm, "%Y-%m-%d %H:%M:%S");
    entry.timestamp = ss.str();

    char buffer[1080];
    va_list args;
    va_start(args, msg);
    vsnprintf(buffer, 1024, msg, args);
    va_end(args);

    entry.message = buffer;

    for (auto& [key, callback] : p_callbacks)
    {
        callback->Log(entry);
    }
}

hive::LogManager& hive::GetLogManager()
{
    return g_hive.logManager;
}
