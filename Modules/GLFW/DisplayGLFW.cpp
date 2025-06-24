#include "Core/Logging/LogManager.h"
#include "Display/DisplayType.h"
#include "DisplayGLFW.h"
// #include "NativeHelperGLFW.h"

#include <stdexcept>

#include "NativeHelperGLFW.h"

namespace hive::glfw
{
    Display *display_create(const DisplayCreateInfo &create_info)
    {
        Display *display = static_cast<Display *>(HV_NEW(sizeof(Display), HIVE_ALLOC_CAT_ENGINE));
        display->p_width = create_info.width;
        display->p_height = create_info.height;
        display->p_title = create_info.title.c_str();

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        display->p_window = glfwCreateWindow(display->p_width, display->p_height, display->p_title, nullptr, nullptr);

        if (display->p_window == nullptr)
            HIVE_LOG_ERROR("Failed to create GLFW window");
        else
        {
            glfwSetWindowUserPointer(display->p_window, &create_info.event_manager);
        }

        display_setup_callback(display);

        return display;
    }

    void display_destroy(Display *display)
    {
        glfwDestroyWindow(display->p_window);

        HV_FREE(display);
    }

    void display_setup_callback(const Display *display)
    {
        glfwSetWindowCloseCallback(display->p_window, [](GLFWwindow *window)
        {
            auto *manager = static_cast<EventManager *>(glfwGetWindowUserPointer(window));

            Event event;
            manager->EventPush(event, EVENT_CODE_APPLICATION_QUIT);
        });

        glfwSetFramebufferSizeCallback(display->p_window, [](GLFWwindow *window, int width, int height)
        {
            auto *manager = static_cast<EventManager *>(glfwGetWindowUserPointer(window));
            Event event;
            event.data.u16[0] = width;
            event.data.u16[1] = height;

            manager->EventPush(event, EVENT_CODE_WINDOW_RESIZED);
        });

        glfwSetKeyCallback(display->p_window, [](GLFWwindow *window, int key, int scancode, int action, int mods)
        {
            auto *manager = static_cast<EventManager *>(glfwGetWindowUserPointer(window));
            Event event;

            event.data.u16[0] = key;
            manager->EventPush(event, action == GLFW_PRESS ? EVENT_CODE_KEY_PRESSED : EVENT_CODE_KEY_RELEASED);
        });

        // code for testing with mouse, need to check later because i ai generated it
        glfwSetMouseButtonCallback(display->p_window, [](GLFWwindow *window, int button, int action, int mods)
        {
            auto *manager = static_cast<EventManager *>(glfwGetWindowUserPointer(window));
            Event event;
            double x, y;
            glfwGetCursorPos(window, &x, &y);
            event.data.u16[0] = button;
            event.data.i16[1] = static_cast<int16_t>(x);
            event.data.i16[2] = static_cast<int16_t>(y);

            uint16_t code = (action == GLFW_PRESS) ? EVENT_CODE_BUTTON_PRESSED : EVENT_CODE_BUTTON_RELEASED;
            manager->EventPush(event, code);

            if (action == GLFW_RELEASE)
                manager->EventPush(event, EVENT_CODE_BUTTON_CLICKED);
        });

        glfwSetCursorPosCallback(display->p_window, [](GLFWwindow *window, double xpos, double ypos)
        {
            auto *manager = static_cast<EventManager *>(glfwGetWindowUserPointer(window));
            Event event;
            event.data.i16[0] = static_cast<int16_t>(xpos);
            event.data.i16[1] = static_cast<int16_t>(ypos);
            manager->EventPush(event, EVENT_CODE_MOUSE_MOVED);
        });

        glfwSetScrollCallback(display->p_window, [](GLFWwindow *window, double xoffset, double yoffset)
        {
            auto *manager = static_cast<EventManager *>(glfwGetWindowUserPointer(window));
            Event event;
            event.data.i8[0] = static_cast<int8_t>(yoffset);
            manager->EventPush(event, EVENT_CODE_MOUSE_WHEEL);
        });
    }

    void *display_get_native_display(Display *display)
    {
        return GetNativeDisplay(display->p_window);
    }

    void *display_get_native_window(Display *display)
    {
        return GetNativeWindow(display->p_window);
    }

    void display_poll_event()
    {
        glfwPollEvents();
    }
}
