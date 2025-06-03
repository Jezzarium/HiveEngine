#pragma once

namespace hive {
    struct DebugUIAPI {
        void (*createWindow)();
        void (*drawWindow)();
        void (*createButton)();
        void (*destroyWindow)();
    };
}
