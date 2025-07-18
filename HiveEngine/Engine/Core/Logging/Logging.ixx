export module Core.Logging;
import Core.Utils;
import Core.Functor;
import std;

namespace hive
{
    constexpr bool LOGGING_ENABLED = true;
    constexpr bool LOGGING_TRACE_ENABLED = LOGGING_ENABLED && true;
    constexpr bool LOGGING_INFO_ENABLED = LOGGING_ENABLED && true;
    constexpr bool LOGGING_WARNING_ENABLED = LOGGING_ENABLED && true;
    constexpr bool LOGGING_ERROR_ENABLED = LOGGING_ENABLED && true;

    export class LogCategory
    {
    public:
        constexpr explicit LogCategory(const char* name) : m_Name(name)
        {
        }

        constexpr const char* GetName() const { return m_Name; }

        LogCategory(const LogCategory& other) = delete; //Copy constructor
        LogCategory(LogCategory&& other) = delete; //Move constructor
        LogCategory& operator=(const LogCategory& other) = delete; //Copy assignment
        LogCategory& operator=(LogCategory&& other) = delete; //Move assignment
    private:
        const char* m_Name;
    };

    export constexpr LogCategory LogDefault{"Default"};


    export enum class LogSeverity
    {
        TRACE, INFO, WARN, ERROR
    };


    export class LogManager final : public Singleton<LogManager>
    {
    public:
        using LoggerId = unsigned int;
        using LogCallback = Functor<void, const LogCategory&, LogSeverity, const char*>;

        void UnregisterLogger(LoggerId id);
        void Log(const LogCategory& cat, LogSeverity sev, const char* msg);

        template <typename T>
        [[nodiscard]] LoggerId RegisterLogger(T* obj, void (T::*method)(const LogCategory&, LogSeverity, const char*))
        {
            m_loggers[m_count++] = {m_count, LogCallback{obj, method}};
            return ++m_IdCount;
        }

    private:
        std::array<std::pair<LoggerId, LogCallback>, 10> m_loggers;
        unsigned int m_count = 0;
        unsigned int m_IdCount = 0;
    };

    inline void LogGeneral(const LogCategory& cat, LogSeverity sev, const char* msg)
    {
        LogManager::GetInstance().Log(cat, sev, msg);
    }

    export inline void LogTrace(const LogCategory& category, const char* message)
    {
        if constexpr (LOGGING_TRACE_ENABLED)
        {
            LogGeneral(category, LogSeverity::TRACE, message);
        }
    }

    export inline void LogInfo(const LogCategory& category, const char* message)
    {
        if constexpr (LOGGING_TRACE_ENABLED)
        {
            LogGeneral(category, LogSeverity::INFO, message);
        }
    }

    export inline void LogWarning(const LogCategory& category, const char* message)
    {
        if constexpr (LOGGING_TRACE_ENABLED)
        {
            LogGeneral(category, LogSeverity::WARN, message);
        }
    }

    export inline void LogError(const LogCategory& category, const char* message)
    {
        if constexpr (LOGGING_TRACE_ENABLED)
        {
            LogGeneral(category, LogSeverity::ERROR, message);
        }
    }
}
