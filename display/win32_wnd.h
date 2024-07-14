#ifndef WIN32WND_H
#define WIN32WND_H

#include <windows.h>
#include <string>
#include <cassert>
#include <cstdint>

#include "../core/color_buffer.h"

class Win32Wnd {
public:
    Win32Wnd(const LPCSTR &class_name, const LPCSTR &window_title);
    ~Win32Wnd();

    void open(int width, int height);
    void draw(const ColorBuffer &buffer) const;

    [[nodiscard]] bool isRunning() const { return is_running_; }

    static void HandleMsg();

private:
    void registerWndClass() const;
    void createWnd();
    void createMemoryDC();
    void createDeviceIndependentBitmap();

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
