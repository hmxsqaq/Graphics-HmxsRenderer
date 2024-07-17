#ifndef WIN32WND_H
#define WIN32WND_H

#include <windows.h>
#include <string>
#include <cassert>
#include <cstdint>
#include <functional>

#include "../core/buffer.h"

typedef enum { A, D, W, S, Q, E, SPACE, ESC } KeyCode;
typedef enum { L, R, NUM } MouseCode;

class Win32Wnd {
public:
    Win32Wnd(const LPCSTR &class_name, const LPCSTR &window_title);
    ~Win32Wnd();

    void OpenWnd(int width, int height);
    void PushBuffer(const ColorBuffer &buffer) const;
    void PushText(const std::string &text);
    void UpdateWnd() const;

    [[nodiscard]] bool is_running() const { return is_running_; }

    static void HandleMsg();

    std::function<void(Win32Wnd*, KeyCode)> key_callback;
    std::function<void(Win32Wnd*, MouseCode)> mouse_callback;
    std::function<void(Win32Wnd*, double)> scroll_callback;

private:
    void RegisterWndClass() const;
    void CreateWnd();
    void CreateMemoryDC();
    void CreateDeviceIndependentBitmap();

    static LRESULT CALLBACK ProcessMsg(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    static void ProcessKey(Win32Wnd* windows, WPARAM wParam);
    static void ProcessMouse(Win32Wnd* windows, MouseCode mouse_code);
    static void ProcessScroll(Win32Wnd* windows, WPARAM wParam);

    LPCSTR class_name_;     // Window class name
    LPCSTR window_title_;   // Window title name
    int width_, height_;    // Window width and height
    HINSTANCE hinstance_;   // Instance handle
    HWND hwnd_;             // Window handle

    HDC pixels_dc_;         // memory device context
    uint8_t* pixels_buffer_;// buffer
    HBITMAP pixel_bitmap_;  // bitmap

    HFONT text_font_;       // text font
    HDC text_dc_;           // text device context
    HBITMAP text_bitmap_;   // text bitmap
    SIZE text_size_;        // text size
    bool is_running_;
};

#endif //WIN32WND_H
