#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#ifdef HIVE_PLATFORM_MACOS
void* create_metal_layer_for_nswindow(void* window);
#endif

#ifdef __cplusplus
}
#endif
