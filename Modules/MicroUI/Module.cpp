#include "DebugUI/DebugUIAPI.h"
#include "DebugUIMicroUI.h"


hive::DebugUIAPI microui_api {
    hive::microui::debug_ui_create_window,
    hive::microui::debug_ui_update,
    hive::microui::debug_ui_create_button,
    hive::microui::debug_ui_destroy_window,
};

extern "C" HIVE_API void InitModule()
{
    mu_begin(ctx);
    mu_Context *ctx = malloc(sizeof(mu_Context));
    ctx->text_width = text_width;
    ctx->text_height = text_height;
    HIVE_LOG_DEBUG("Init microUI module");

}

extern "C" HIVE_API void ShutdownModule()
{
    HIVE_LOG_DEBUG("Shutdown microUI module");
}
