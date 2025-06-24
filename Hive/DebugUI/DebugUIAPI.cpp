#pragma once

#include "DebugUIAPI.h"

std::vector<hive::DebugUIAPI*> g_debug_ui_api;


#ifdef DEBUG_UI_ENABLE
void hive::DebugUICreateWindow() {
}

void hive::DebugUIUpdate() {
}

void hive::DebugUICreateButton() {
}

void hive::DebugUIDestroyWindow() {
}

void test() {
    //TODO
}
#else
void hive::DebugUICreateWindow() {}
void hive::DebugUIDrawWindow() {}
void hive::DebugUICreateButton() {}
void hive::DebugUIDestroyWindow() {}
void test() {}
#endif

