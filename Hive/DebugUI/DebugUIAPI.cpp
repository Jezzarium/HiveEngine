#pragma once

#include "DebugUIAPI.h"

std::vector<hive::DebugUIAPI*> g_debug_ui_api;


#ifdef DEBUG_UI_ENABLE
void test() {
    //TODO
}
#else
void test() {}
#endif