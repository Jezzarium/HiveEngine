add_subdirectory(Core)
add_subdirectory(Platform)
add_subdirectory(Profiling)

add_library(Hive.Engine STATIC)

target_sources(Hive.Engine PUBLIC FILE_SET public_module TYPE CXX_MODULES FILES Engine.ixx)
target_link_libraries(Hive.Engine PRIVATE Hive.Engine.Core Hive.Engine.Platform Hive.Engine.Profiling)