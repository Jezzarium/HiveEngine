module Hive.Tracy;
import <tracy/Tracy.hpp>;

namespace hive::Tracy
{
    void StartupProfiler()
    {
        tracy::StartupProfiler();
    }

    void ShutdownProfiler()
    {
        tracy::ShutdownProfiler();
    }
}
