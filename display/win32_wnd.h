#ifndef WIN32WND_H
#define WIN32WND_H

#include <windows.h>
#include <string>
#include <cassert>
#include <cstdint>
#include <functional>

#include "../core/color_buffer.h"

typedef enum { A, D, W, S, Q, E, SPACE, ESC } KeyCode;
typedef enum { L, R, NUM } MouseCode;

inline std::function<void(KeyCode)> key_callback = nullptr;
inline std::function<void(MouseCode)> mouse_callback = nullptr;
inline std::function<void(double)> scroll_callback = nullptr;

class Win32Wnd {
public:
    Win32Wnd(const LPCSTR &class_name, const LPCSTR &window_title);
    ~Win32Wnd();

    void openWnd(int width, int height);
    void drawBuffer(const ColorBuffer &buffer) const;
    void drawText(const std::string &text);
    void updateWnd() const;

    [[nodiscard]] bool isRunning() const { return is_running_; }

    static void HandleMsg();
private:
    void registerWndClass() const;
    void createWnd();
    void createMemoryDC();
    void createDeviceIndependentBitmap();

    static LRESULT CALLBACK ProcessMsg(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    static void ProcessKey(WPARAM wParam);
    static void ProcessMouse(MouseCode mouse_code);
    static void ProcessScroll(WPARAM wParam);

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
