export module Core.Logging.Logger;

import Core.Logging;
import std;

namespace hive
{
    export class ConsoleLogger
    {
    public:
        void Log(const LogCategory& cat, LogSeverity sev, const char* msg)
        {
            switch (sev)
            {
            case LogSeverity::TRACE:
                std::cout << "[TRACE] - ";
                break;
            case LogSeverity::INFO:
                std::cout << "[INFO] - ";
                break;
            case LogSeverity::WARN:
                std::cout << "[WARN] -  ";
                break;
            case LogSeverity::ERROR:
                std::cout << "[ERROR] - ";
                break;
            }
            std::cout << cat.GetName() << " : " << msg << std::endl;
        }
    };


    export class FileLogger
    {
    public:
        FileLogger(std::filesystem::path path)
        {
            file.open(path);
        }

        ~FileLogger()
        {
            file.close();
        }

        void Log(const hive::LogCategory& category, hive::LogSeverity sev, const char* msg)
        {
            file.write(msg, std::strlen(msg));
        }

    private:
        std::ofstream file;

    };}
