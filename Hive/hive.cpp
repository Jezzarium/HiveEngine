#include "Core/Logging/ConsoleLogger.h"
#include "Core/Logging/LogManager.h"
#include "Display/DisplayAPI.h"
#include "Graphic/GFX.h"
#include "hive.h"

#include <Core/Profiling/Profiler.h>


#include "hive_internal.h"


hive::HiveLibrary hive::g_hive;

namespace hive
{
    bool InitCore();
    bool InitProfiler();
    bool InitModules();
    bool InitDisplay();
    bool InitGraphic();
}

bool hive::Init()
{
    if(!InitCore()) return false;

    // Stack-based logger for early-stage errors before the profiler (if enabled) is available
    g_hive.logManager.SetLogLevel(LOG_LEVEL_DEBUG);
    const ConsoleLogger boot_logger;
    auto tmp_logger_id = g_hive.logManager.AddLogger(&boot_logger);

    if(!InitProfiler()) return false;
    if(!InitModules()) return false;
    if(!InitDisplay()) return false;
    if(!InitGraphic()) return false;

    // Remove Stack-based logger
    g_hive.logManager.RemoveCallback(tmp_logger_id);

    //Allocate heap console logger for the rest of the duration of the application
    hive::ConsoleLogger *console_logger = HV_NEW_OOP ConsoleLogger();
    g_hive.logManager.AddLogger(console_logger);

    return true;
}

void hive::Shutdown()
{
    g_hive.lib_manager.Shutdown();
    g_hive.logManager.Shutdown();
    g_hive.tracker.Shutdown();

#ifdef HIVE_PROFILER_ENABLED
    hive::LibraryManager::UnloadLibraryUnmanaged(g_hive.profiler_lib);
#endif
}


bool hive::InitCore()
{
    g_hive.logManager.Init();
    g_hive.tracker.Init();
    g_hive.lib_manager.Init();

    return true;
}

bool hive::InitModules()
{

    //TODO: read from a config file or something like that

    auto glfw_lib = g_hive.lib_manager.LoadLibrary("Glfw");
    if(glfw_lib == -1)
    {
        return false;
    }

    auto vulkan_lib = g_hive.lib_manager.LoadLibrary("Vulkan");
    if(vulkan_lib == -1)
    {
        return false;
    }

    auto microui_lib = g_hive.lib_manager.LoadLibrary("MicroUI");
    if(microui_lib == -1)
    {
        return false;
    }

    return true;
}

bool hive::InitProfiler()
{
    #ifdef HIVE_PROFILER_ENABLED
        //The profiler module is a special case of module and need to be managed manually
        g_hive.profiler_lib = hive::LibraryManager::LoadLibraryUnmanaged("Tracy");
        if (g_hive.profiler_lib == nullptr)
            return false;

        hive::profile_init();
    #endif
    return true;
}

bool hive::InitDisplay()
{
    hive::DisplaySetBackend("glfw");
    return true;

}

bool hive::InitGraphic()
{
    hive::gfx::set_backend_api("vulkan");
    return true;
}
