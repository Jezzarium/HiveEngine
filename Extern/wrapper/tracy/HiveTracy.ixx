export module Hive.Tracy;
import <source_location>;
namespace hive::Tracy
{
    export void StartupProfiler();
    export void ShutdownProfiler();

    export struct ZoneContext
    {
        unsigned int id;
        int active;
    };

    export struct ZoneLocationData
    {
        const char *name;
        const char *function;
        const char *file;
        uint32_t line;
        uint32_t color;
    };

    export ZoneContext BeginZone(ZoneLocationData* locationData);
    export void EndZone(ZoneContext& ctx);

    export void MarkFrame();
}
