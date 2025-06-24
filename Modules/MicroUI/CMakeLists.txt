add_library(MicroUI SHARED
    Module.cpp
        DebugUIMicroUI.cpp
        DebugUIMicroUI.h
)

target_link_libraries(MicroUI PRIVATE HiveCore)
target_compile_definitions(MicroUI PRIVATE HIVE_EXPORTS)
target_include_directories(MicroUI
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/microui/src
)