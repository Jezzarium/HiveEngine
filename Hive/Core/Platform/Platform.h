#pragma once

namespace hive
{
    void *platform_load_library(const char *library_name);
    void platform_unload_library(void* library);

    void* platform_get_symbol(void* library, const char* symbol);
}

#ifdef _WIN32
#define HIVE_PLATFORM_WINDOWS
#endif

#ifdef __linux__
#define HIVE_PLATFORM_LINUX
#endif

#ifdef __APPLE__
#define HIVE_PLATFORM_MACOS
#endif
