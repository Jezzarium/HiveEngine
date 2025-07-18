add_library(Hive.Engine.Core STATIC)

file(GLOB_RECURSE MODULE_FILES *.ixx)
file(GLOB_RECURSE SOURCE_FILES *.cpp)

target_sources(Hive.Engine.Core PRIVATE ${SOURCE_FILES})
target_sources(Hive.Engine.Core PUBLIC FILE_SET public_module TYPE CXX_MODULES FILES ${MODULE_FILES})
