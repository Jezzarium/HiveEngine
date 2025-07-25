add_library(HiveTracy STATIC)

#file(GLOB_RECURSE MODULE_FILES *.ixx)
file(GLOB SOURCE_FILES HiveTracy.cpp)

target_sources(HiveTracy PRIVATE ${SOURCE_FILES})
target_sources(HiveTracy PUBLIC FILE_SET public_module TYPE CXX_MODULES FILES HiveTracy.ixx)

option(TRACY_ENABLE "" ON)
option(TRACY_DELAYED_INIT "" ON)
option(TRACY_MANUAL_LIFETIME "" ON)

add_subdirectory(3rdparty/tracy)

target_link_libraries(HiveTracy PRIVATE Hive.Engine.Core Tracy::TracyClient)