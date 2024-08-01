#ifndef WIN32WND_H
#define WIN32WND_H

#include <windows.h>
#include <string>
#include <cassert>
#include <cstdint>
#include <functional>

#include "buffer.h"

typedef enum { A, D, W, S, Q, E, SPACE, ESC } KeyCode;
typedef enum { L, R, NUM } MouseCode;

/**
 * @brief window class based on Win32 API using GDI to draw pixels and text.
 */
class Win32Wnd {
public:
    Win32Wnd(const LPCSTR &class_name, const LPCSTR &window_title);
    ~Win32Wnd();

    void OpenWnd(int width, int height);
    void PushBuffer(const ColorBuffer &buffer) const;
    void PushText(const std::string &text);
    void UpdateWnd() const;
    void CloseWnd();

    void SetTextFont(const std::string &font_name, int font_size);
    void SetTextColorRef(const COLORREF &color) { text_color_ = color; }
    void SetTextOffset(const Vector2i &offset) { text_offset_ = offset; }

    void RegisterKeyCallback(const std::function<void(Win32Wnd*, KeyCode)> &callback) { key_callbacks_.push_back(callback); }
    void RegisterMouseCallback(const std::function<void(Win32Wnd*, MouseCode)> &callback) { mouse_callbacks_.push_back(callback); }
    void RegisterScrollCallback(const std::function<void(Win32Wnd*, double)> &callback) { scroll_callbacks_.push_back(callback); }
    void ClearKeyCallbacks() { key_callbacks_.clear(); }
    void ClearMouseCallbacks() { mouse_callbacks_.clear(); }
    void ClearScrollCallbacks() { scroll_callbacks_.clear(); }

    void SetUserData(const std::shared_ptr<void> &data) { user_data_ = data; }
    [[nodiscard]] std::shared_ptr<void> GetUserData() const { return user_data_; }

    [[nodiscard]] bool is_running() const { return is_running_; }

    static void HandleMsg();

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
    COLORREF text_color_;   // text color
    Vector2i text_offset_;  // text offset

    bool is_running_;

    std::shared_ptr<void> user_data_;

    std::vector<std::function<void(Win32Wnd*, KeyCode)>> key_callbacks_;
    std::vector<std::function<void(Win32Wnd*, MouseCode)>> mouse_callbacks_;
    std::vector<std::function<void(Win32Wnd*, double)>> scroll_callbacks_;
};

#endif //WIN32WND_H
