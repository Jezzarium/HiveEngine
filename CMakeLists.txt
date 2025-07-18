cmake_minimum_required(VERSION 3.30)
project(HiveProject)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


if(MSVC)
    add_compile_options(/experimental:module /Zc:preprocessor /std:c++latest)
else()
    if(CMAKE_VERSION VERSION_LESS "3.28.0")
        if(CMAKE_VERSION VERSION_LESS "3.27.0")
            set(CMAKE_EXPERIMENTAL_CXX_MODULE_CMAKE_API "2182bf5c-ef0d-489a-91da-49dbc3090d2a")
        else()
            set(CMAKE_EXPERIMENTAL_CXX_MODULE_CMAKE_API "aa1f7df0-828a-4fcd-9afc-2dc80491aca7")
        endif()
        set(CMAKE_EXPERIMENTAL_CXX_MODULE_DYNDEP 1)
    else()
        cmake_policy(VERSION 3.28)
    endif()

    include(FetchContent)
    FetchContent_Declare(
            std
            URL "file://${LIBCXX_BUILD}/modules/c++/v1/"
            DOWNLOAD_EXTRACT_TIMESTAMP TRUE
            SYSTEM
    )
    FetchContent_MakeAvailable(std)
    link_libraries(std.compat std)
endif()

add_subdirectory(Extern)
add_subdirectory(HiveEngine)
add_subdirectory(Testbed)