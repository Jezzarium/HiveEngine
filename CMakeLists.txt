cmake_minimum_required(VERSION 3.28)

project(Hive)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)   # Executables & DLLs
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)   # Shared Libraries (.so/.dylib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)   # Static Libraries (.a/.lib)
set(CMAKE_SHARED_LIBRARY_PREFIX "")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


add_subdirectory(Hive)
add_subdirectory(Testbed)
add_subdirectory(Modules/Vulkan)
add_subdirectory(Modules/GLFW)
add_subdirectory(Modules/Tracy)
add_subdirectory(Modules/MicroUI)