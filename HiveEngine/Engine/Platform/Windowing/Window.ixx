export module Platform.Window;
import Core;
namespace hive
{
    export class Window
    {
    public:
        explicit Window(unsigned int width, unsigned int height, const char* title);
        ~Window();
    private:
        struct WindowBackendData;
        ImplOwner<WindowBackendData> m_data;
    };
}
