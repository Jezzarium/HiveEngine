add_library(MicroUI SHARED
    Module.cpp
        DebugUIMicroUI.cpp
        DebugUIMicroUI.h
)

target_link_libraries(MicroUI PRIVATE HiveCore)
target_compile_definitions(MicroUI PRIVATE HIVE_EXPORTS)