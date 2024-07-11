#include "win32wnd.h"

int main() {
    Win32Wnd window(L"SampleWindowClass", L"Sample Window");

    window.CreateWnd(500, 500);
    while (window.is_running_) {
        Win32Wnd::HandleMsg();
    }
    return 0;
}
