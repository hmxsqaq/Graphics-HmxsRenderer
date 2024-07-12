#include "win32_wnd.h"

int main() {
    Win32Wnd window("Hmxs", "HmxsRenderer");

    window.Open(500, 500);
    while (!window.ShouldShutdown()) {
        Win32Wnd::HandleMsg();
    }
    return 0;
}
