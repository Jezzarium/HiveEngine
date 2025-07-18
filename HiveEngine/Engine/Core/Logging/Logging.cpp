module Core.Logging;

namespace hive
{

    void LogManager::UnregisterLogger(LoggerId id)
    {
        for (int i = 0; i < m_loggers.size(); i++)
        {
            auto logger = m_loggers[i];
            if (logger.first == id)
            {
                m_loggers[i] = m_loggers.back();
                m_count--;
            }
        }
    }

    void LogManager::Log(const LogCategory &cat, LogSeverity sev, const char *msg)
    {
        for (int i = 0; i < m_count; i++)
        {
            m_loggers[i].second(cat, sev, msg);
        }
    }
}
