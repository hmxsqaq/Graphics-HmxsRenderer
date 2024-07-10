#include "win32wnd.h"

int main() {
    Win32Wnd window(L"SampleWindowClass", L"Sample Window");

    window.Create(100, 100);

    MSG msg = {};
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}
