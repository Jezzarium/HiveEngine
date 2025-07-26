module Hive.Tracy;
import <tracy/Tracy.hpp>;
import <tracy/TracyC.h>;

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

    ZoneContext BeginZoneColored(uint8_t color, const char *name, const char *file, const char *function,
        unsigned int line)
    {
        const ___tracy_source_location_data tracySourceLocation = {
            name, function, file, line, color
        };
        TracyCZoneCtx ctx = ___tracy_emit_zone_begin_callstack(&tracySourceLocation, 0, true);;
        return {ctx.id, ctx.active};
    }

    ZoneContext BeginZone(ZoneLocationData *locationData)
    {
        TracyCZoneCtx ctx = ___tracy_emit_zone_begin_callstack(static_cast<___tracy_source_location_data*>(static_cast<void *>(locationData)), 0, true);
        return {ctx.id, ctx.active};
    }

    void EndZone(ZoneContext &ctx)
    {
        TracyCZoneCtx tracyCtx = {ctx.id, ctx.active};
        ___tracy_emit_zone_end(tracyCtx);
    }

    void MarkFrame()
    {
        ___tracy_emit_frame_mark("Frame");
    }
}
