add_library(Hive.Engine.Platform.Windowing STATIC)

#Todo: conditional glfw
target_sources(Hive.Engine.Platform.Windowing PRIVATE GLFW/GLFW.cpp)
target_sources(Hive.Engine.Platform.Windowing PUBLIC FILE_SET public_module TYPE CXX_MODULES FILES Window.ixx)

target_link_libraries(Hive.Engine.Platform.Windowing PRIVATE glfw Hive.Engine.Core)