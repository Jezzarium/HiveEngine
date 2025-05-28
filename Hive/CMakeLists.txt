option(HIVE_TRACK_PROFILE "Set the tracking profile (FULL, DEBUG, RELEASE)" "DEBUG")
option(HIVE_DEBUG_UI "Enable the debug ui" "OFF")

add_library(HiveCore SHARED
        Core/Event/EventManager.cpp
        Core/Libraries/LibraryManager.cpp
        Core/Logging/LogManager.cpp
        Core/Logging/ConsoleLogger.cpp
        Core/Memory/MemoryAllocator.cpp
        Core/Memory/MemoryTracker.cpp
        Core/Object/Object.cpp
        Core/Platform/Platform_Linux.cpp
        Core/Platform/Platform_Windows.cpp
        Core/Profiling/Profiler.cpp

        Graphic/GFX.cpp

        Display/DisplayAPI.cpp


        hive.cpp
        DebugUI/DebugUIAPI.cpp

)
target_precompile_headers(HiveCore PUBLIC hvpch.h)
target_compile_definitions(HiveCore PRIVATE HIVE_EXPORTS)
target_compile_definitions(HiveCore PUBLIC HIVE_IMPORTS)

target_include_directories(HiveCore PUBLIC ${PROJECT_SOURCE_DIR}/Hive)

# Define the option with a default value

# Set the corresponding compile definition based on the option
if(HIVE_TRACK_PROFILE STREQUAL "FULL")
    # Profiling enabled, Memory allocation tracking,
    target_compile_definitions(HiveCore PUBLIC HIVE_TRACK_PROFILE_FULL)
elseif(HIVE_TRACK_PROFILE STREQUAL "DEBUG")
    # Memory allocation tracking only
    target_compile_definitions(HiveCore PUBLIC HIVE_TRACK_PROFILE_DEBUG)
elseif(HIVE_TRACK_PROFILE STREQUAL "RELEASE")
    # No tracking at all
    target_compile_definitions(HiveCore PUBLIC HIVE_TRACK_PROFILE_RELEASE)
else()
    message(FATAL_ERROR "Invalid value for HIVE_TRACK_PROFILE. Valid options are FULL, DEBUG, or RELEASE.")
endif()

if(HIVE_DEBUG_UI STREQUAL "ON")
    target_compile_definitions(HiveCore PUBLIC DEBUG_UI_ENABLE)
endif()

if(NOT MSVC)
    target_compile_options(HiveCore PRIVATE -fvisibility=hidden -fvisibility-inlines-hidden)
endif ()

