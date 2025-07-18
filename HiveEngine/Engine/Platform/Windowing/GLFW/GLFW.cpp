module;
#include <GLFW/glfw3.h>

module Platform.Window;
import Core;
import std;
namespace hive
{
    std::atomic<int> g_WindowCount{0};

    bool InitWindowing()
    {
        int expected = 0;
        if (g_WindowCount.compare_exchange_strong(expected, expected + 1))
        {
            LogTrace(LogDefault, "Init GLFW");
            return glfwInit();
        }
        else
        {
            g_WindowCount.fetch_add(1, std::memory_order_relaxed);
            return true;
        }
    }

    void ShutdownWindow()
    {
        int expected = 1;
        if (g_WindowCount.compare_exchange_strong(expected, expected - 1))
        {
            // This thread initializes GLFW
            LogTrace(LogDefault, "Shutdown GLFW");
            glfwTerminate();
        }
        else
        {
            g_WindowCount.fetch_sub(1, std::memory_order_relaxed);
        }
    }

    struct Window::WindowBackendData
    {
        GLFWwindow* m_window{nullptr};
    };

    Window::Window(unsigned int width, unsigned int height, const char* title)
    {
        InitWindowing();
        LogTrace(LogDefault, "Creating a new GLFW window");
        auto window = glfwCreateWindow(width, height, title, nullptr, nullptr);
        m_data->m_window = window;
    }

    Window::~Window()
    {
        LogTrace(LogDefault, "Destroying a new GLFW window");
        glfwDestroyWindow(m_data->m_window);
        ShutdownWindow();
    }
}
