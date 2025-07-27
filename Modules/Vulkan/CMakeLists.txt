find_package(Vulkan REQUIRED)
set(VK_BOOTSTRAP_POSITION_INDEPENDENT_CODE ON)
add_subdirectory(3rdparty/vk-bootstrap)


add_library(Vulkan SHARED Module.cpp
        BufferVulkan.cpp
        CommandVulkan.cpp
        FramebufferVulkan.cpp
        GraphicDeviceVulkan.cpp
        RenderpassVulkan.cpp
        ShaderVulkan.cpp
        SurfaceFactory.cpp
        MetalHelper.mm
        SwapchainVulkan.cpp
        SyncVulkan.cpp
        TextureVulkan.cpp
        UtilsVulkan.cpp
)

target_link_libraries(Vulkan PRIVATE vk-bootstrap HiveCore Vulkan::Vulkan)
target_compile_definitions(Vulkan PRIVATE HIVE_EXPORTS)

# Disable precompiled headers for Objective-C++ MetalHelper
set_source_files_properties(MetalHelper.mm PROPERTIES SKIP_PRECOMPILE_HEADERS ON)

if(APPLE)
    target_link_libraries(Vulkan PRIVATE "-framework Metal" "-framework QuartzCore" "-framework Cocoa")
endif()
