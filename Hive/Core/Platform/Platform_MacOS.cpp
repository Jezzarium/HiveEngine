#include "Platform.h"

#ifdef HIVE_PLATFORM_MACOS
#include <dlfcn.h>
void* hive::platform_load_library(const char* library_name)
{
    const std::string path = "./"  + std::string(library_name) + ".dylib";
    void *lib = dlopen(path.c_str(), RTLD_NOW | RTLD_GLOBAL);
    if (!lib)
    {
        HIVE_LOG_ERROR("%s", dlerror());
        return nullptr;
    }

    return lib;
}

void hive::platform_unload_library(void* library)
{
    dlclose(library);
}

void* hive::platform_get_symbol(void* library, const char* symbol)
{
    return dlsym(library, symbol);
}
#endif
