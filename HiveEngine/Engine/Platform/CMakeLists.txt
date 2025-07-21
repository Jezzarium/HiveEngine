add_library(Hive.Engine.Platform OBJECT)

add_subdirectory(Windowing)
#file(GLOB_RECURSE MODULE_FILES *.ixx)
#file(GLOB_RECURSE SOURCE_FILES *.cpp)

target_sources(Hive.Engine.Platform PRIVATE ${SOURCE_FILES})
target_sources(Hive.Engine.Platform PUBLIC FILE_SET public_module TYPE CXX_MODULES FILES Platform.ixx)

target_link_libraries(Hive.Engine.Platform PRIVATE Hive.Engine.Core)
target_link_libraries(Hive.Engine.Platform PUBLIC Hive.Engine.Platform.Windowing)

