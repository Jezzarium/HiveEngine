#include "NativeHelperGLFW.h"
#include "Core/Platform/Platform.h"

#ifdef HIVE_PLATFORM_LINUX
#define GLFW_EXPOSE_NATIVE_X11
#define GLFW_EXPOSE_NATIVE_WAYLAND
#include <X11/X.h>
#include <wayland-client.h>
#include "GLFW/glfw3native.h"

void* hive::glfw::GetNativeDisplay(GLFWwindow* window)
{
    auto session_type = std::string(std::getenv("XDG_SESSION_TYPE"));
    if (session_type == "x11")
    {
        return glfwGetX11Display();
    }
    if (session_type == "wayland")
    {
        return glfwGetWaylandDisplay();
    }

    return nullptr;
}

void* hive::glfw::GetNativeWindow(GLFWwindow* window)
{
    auto session_type = std::string(std::getenv("XDG_SESSION_TYPE"));
    if (session_type == "x11")
    {
        const auto id = new XID(glfwGetX11Window(window)); //WARNING MEMORY LEAK HERE
        return id;
    }
    if (session_type == "wayland")
    {
        return glfwGetWaylandWindow(window);
    }

    return nullptr;
}
#endif

#ifdef HIVE_PLATFORM_WINDOWS
#define GLFW_EXPOSE_NATIVE_WIN32
#include "GLFW/glfw3native.h"
void* hive::glfw::GetNativeDisplay(GLFWwindow* window)
{
    return GetModuleHandle(nullptr);
}

void* hive::glfw::GetNativeWindow(GLFWwindow* window)
{
    return glfwGetWin32Window(window);
}
#endif


#ifdef HIVE_PLATFORM_MACOS
#define GLFW_EXPOSE_NATIVE_COCOA
#include "GLFW/glfw3native.h"

void* hive::glfw::GetNativeDisplay(GLFWwindow* window)
{
    // not needed for macos as NSWindow contains everything we need
    return nullptr;
}

void* hive::glfw::GetNativeWindow(GLFWwindow* window)
{
    return glfwGetCocoaWindow(window);
}
#endif
