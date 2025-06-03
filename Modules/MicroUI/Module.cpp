#include "DebugUI/DebugUIAPI.h"
#include "DebugUIMicroUI.h"


hive::DebugUIAPI microui_api {
    hive::microui::debug_ui_create_window,
    hive::microui::debug_ui_draw_window,
    hive::microui::debug_ui_create_button,
    hive::microui::debug_ui_destroy_window,
};

extern "C" HIVE_API void InitModule()
{
    HIVE_LOG_DEBUG("Init microUI module");

}

extern "C" HIVE_API void ShutdownModule()
{
    HIVE_LOG_DEBUG("Shutdown microUI module");
}
