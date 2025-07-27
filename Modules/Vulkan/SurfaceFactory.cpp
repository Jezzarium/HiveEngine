#include "Core/Platform/Platform.h"
#include "SurfaceFactory.h"
#include "Display/DisplayAPI.h"
#include <vulkan/vulkan.h>


#ifdef HIVE_PLATFORM_LINUX
#include <X11/Xlib.h>
#include <vulkan/vulkan_xlib.h>
#include <vulkan/vulkan_wayland.h>

void hive::vk::vulkan_create_surface(Display* display, VkInstance instance, VkSurfaceKHR *surface)
{
    const auto session_type = std::string(std::getenv("XDG_SESSION_TYPE"));

    if(session_type == "wayland")
    {
        VkWaylandSurfaceCreateInfoKHR create_info{};
        create_info.sType = VK_STRUCTURE_TYPE_WAYLAND_SURFACE_CREATE_INFO_KHR;
        create_info.display = static_cast<wl_display*>(DisplayGetNativeDisplay(display));
        create_info.surface = static_cast<wl_surface*>(DisplayGetNativeWindow(display));

        auto result = vkCreateWaylandSurfaceKHR(instance, &create_info, nullptr, surface);
    }

    if (session_type == "x11")
    {
        const auto x11_display = static_cast<_XDisplay*>(DisplayGetNativeDisplay(display));
        const auto window = static_cast<XID*>(DisplayGetNativeWindow(display));

        VkXlibSurfaceCreateInfoKHR create_info{};
        create_info.sType = VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR;
        create_info.window = *window;
        create_info.dpy = x11_display;

        vkCreateXlibSurfaceKHR(instance, &create_info, nullptr, surface);


    }
}
#endif

#ifdef HIVE_PLATFORM_WINDOWS
#include "Display/DisplayAPI.h"
#include "SurfaceFactory.h"
#include "windows.h"
#include "vulkan/vulkan_win32.h"

void hive::vk::vulkan_create_surface(Display *display, VkInstance instance, VkSurfaceKHR*surface)
{
    VkWin32SurfaceCreateInfoKHR create_info{};
    create_info.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
    create_info.hinstance = static_cast<HMODULE>(DisplayGetNativeDisplay(display));
    create_info.hwnd = static_cast<HWND>(DisplayGetNativeWindow(display));
    create_info.flags = 0;

    auto result = vkCreateWin32SurfaceKHR(instance, &create_info, nullptr, surface);


    if (result != VK_SUCCESS)
        HIVE_LOG_ERROR("Failed to create window surface!");
}
#endif

#ifdef HIVE_PLATFORM_MACOS
#include "Display/DisplayAPI.h"
#include "SurfaceFactory.h"
#include "MetalHelper.h"
#include "vulkan/vulkan_metal.h"

void hive::vk::vulkan_create_surface(Display *display, VkInstance instance, VkSurfaceKHR*surface)
{
    void* window = DisplayGetNativeWindow(display);
    void* metalLayer = create_metal_layer_for_nswindow(window);

    VkMetalSurfaceCreateInfoEXT create_info{};
    create_info.sType = VK_STRUCTURE_TYPE_METAL_SURFACE_CREATE_INFO_EXT;
    create_info.pNext = nullptr;
    create_info.flags = 0;
    create_info.pLayer = metalLayer;

    auto result = vkCreateMetalSurfaceEXT(instance, &create_info, nullptr, surface);

    if (result != VK_SUCCESS)
        HIVE_LOG_ERROR("Failed to create metal surface! Result: %d", result);
}
#endif
