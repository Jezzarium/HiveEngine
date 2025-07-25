add_library(Hive.Engine.Profiling STATIC)

#file(GLOB_RECURSE MODULE_FILES *.ixx)
file(GLOB_RECURSE SOURCE_FILES *.cpp)

target_sources(Hive.Engine.Profiling PRIVATE ${SOURCE_FILES})
target_sources(Hive.Engine.Profiling PUBLIC FILE_SET public_module TYPE CXX_MODULES FILES Profiling.ixx)

target_link_libraries(Hive.Engine.Profiling PRIVATE Hive.Engine.Core HiveTracy)
target_compile_definitions(Hive.Engine.Profiling PRIVATE PROFILING_ENABLED )

