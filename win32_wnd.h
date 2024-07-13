#ifndef WIN32WND_H
#define WIN32WND_H

#include <windows.h>
#include <string>
#include <cassert>
#include "color_buffer.h"

class Win32Wnd {
public:
    Win32Wnd(const LPCSTR &class_name, const LPCSTR &window_title);
    ~Win32Wnd();

    void Open(int width, int height);
    void Draw(const ColorBuffer &buffer) const;
    static void HandleMsg();
    [[nodiscard]] bool IsRunning() const { return is_running_; }
private:
    void RegisterWndClass() const;
    void CreateWnd();
    void CreateMemoryDC();
    void CreateDeviceIndependentBitmap();

    static LRESULT CALLBACK ProcessMsg(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    LPCSTR class_name_;
    LPCSTR window_title_;
    int width_, height_;
    HINSTANCE hinstance_;
    HWND hwnd_;
    HDC memory_dc_;
    uint8_t* pixels_;
    HBITMAP d_i_bitmap_;
    bool is_running_;
};

#endif //WIN32WND_H
