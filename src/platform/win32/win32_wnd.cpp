#include "win32_wnd.h"
#include <algorithm>
#include "utility/log.h"

Win32Wnd::Win32Wnd(const LPCSTR &class_name, const LPCSTR &window_title)
    : class_name_(class_name), window_title_(window_title), width_(0), height_(0), hinstance_(GetModuleHandle(nullptr)),
        hwnd_(nullptr), pixels_dc_(nullptr), pixels_buffer_(nullptr), pixel_bitmap_(nullptr), text_font_(nullptr),
        text_dc_(nullptr), text_bitmap_(nullptr), text_size_({}), text_color_(RGB(255, 255, 255)), text_offset_({10, 10}),
        is_running_(false), user_data_(nullptr) {
    assert(hinstance_ != nullptr);
}

Win32Wnd::~Win32Wnd() {
    if (hwnd_) DestroyWindow(hwnd_);
    UnregisterClass(class_name_, hinstance_);
}

void Win32Wnd::OpenWnd(const int width, const int height) {
    assert(width > 0 && height > 0);
    width_ = width;
    height_ = height;
    RegisterWndClass();
    CreateWnd();
    CreateMemoryDC();
    CreateDeviceIndependentBitmap();
    SetWindowLongPtr(hwnd_, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this)); // Store the pointer to the Win32Wnd object for ProcessMsg
    ShowWindow(hwnd_, SW_SHOW);
    is_running_ = true;
}

void Win32Wnd::PushBuffer(const ColorBuffer &buffer) const {
    assert(buffer.bpp() == ColorType::RGBA || buffer.bpp() == ColorType::GRAYSCALE || buffer.bpp() == ColorType::RGB);

    if (buffer.bpp() == ColorType::RGBA) {
        std::copy_n(buffer.data(), buffer.size(), pixels_buffer_);
    } else if (buffer.bpp() == ColorType::GRAYSCALE) {
        const int pixel_count = width_ * height_;
        uint8_t* pixel_ptr = pixels_buffer_;
        for (int i = 0; i < pixel_count; ++i) {
            pixel_ptr[0] = pixel_ptr[1] = pixel_ptr[2] = buffer[i];
            pixel_ptr[3] = 255;
            pixel_ptr += 4;
        }
    } else if (buffer.bpp() == ColorType::RGB) {
        const int pixel_count = width_ * height_;
        uint8_t* pixel_ptr = pixels_buffer_;
        for (int i = 0; i < pixel_count; ++i) {
            pixel_ptr[0] = buffer[i * 3];
            pixel_ptr[1] = buffer[i * 3 + 1];
            pixel_ptr[2] = buffer[i * 3 + 2];
            pixel_ptr[3] = 255;
            pixel_ptr += 4;
        }
    }
}

void Win32Wnd::PushText(const std::string &text) {
    if (text_font_ == nullptr) SetTextFont("Arial", 16);
    HDC hdc = GetDC(hwnd_);
    text_dc_ = CreateCompatibleDC(hdc);
    SetBkMode(text_dc_, TRANSPARENT);
    SetTextColor(text_dc_, text_color_);
    SelectObject(text_dc_, text_font_);
    RECT rect = {0, 0, 0, 0};
    DrawText(text_dc_, text.c_str(), -1, &rect, DT_CALCRECT);
    text_size_.cx = rect.right - rect.left;
    text_size_.cy = rect.bottom - rect.top;
    text_bitmap_ = CreateCompatibleBitmap(hdc, text_size_.cx, text_size_.cy);
    SelectObject(text_dc_, text_bitmap_);
    RECT draw_rect = {0, 0, text_size_.cx, text_size_.cy};
    DrawText(text_dc_, text.c_str(), -1, &draw_rect, DT_LEFT);
    ReleaseDC(hwnd_, hdc);
}

void Win32Wnd::UpdateWnd() const {
    HDC hdc = GetDC(hwnd_);
    BitBlt(pixels_dc_, text_offset_[0], text_offset_[1], text_size_.cx, text_size_.cy, text_dc_, 0, 0, SRCCOPY);
    BitBlt(hdc, 0, 0, width_, height_, pixels_dc_, 0, 0, SRCCOPY);
    ReleaseDC(hwnd_, hdc);
}

void Win32Wnd::CloseWnd() {
    is_running_ = false;
}

void Win32Wnd::SetTextFont(const std::string &font_name, int font_size) {
    text_font_ = CreateFont(font_size, 0, 0, 0,
                            FW_NORMAL,
                            FALSE, FALSE, FALSE,
                            DEFAULT_CHARSET,
                            OUT_DEFAULT_PRECIS,
                            CLIP_DEFAULT_PRECIS,
                            DEFAULT_QUALITY,
                            DEFAULT_PITCH | FF_DONTCARE,
                            font_name.c_str());
    if (text_font_ == nullptr) LOG_WARNING("Win32Wnd - failed to create font: " + font_name);
}

void Win32Wnd::HandleMsg() {
    MSG msg = {};
    while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

void Win32Wnd::RegisterWndClass() const {
    WNDCLASS window_class = {};
    window_class.style = CS_HREDRAW | CS_VREDRAW;   // Redraw on size change
    window_class.lpfnWndProc = ProcessMsg;  // Window Message Procedure Callback
    window_class.cbClsExtra = 0;    // No extra bytes after the window class
    window_class.cbWndExtra = 0;    // No extra bytes after the window instance
    window_class.hInstance = hinstance_;   // Instance of the application
    window_class.hIcon = LoadIcon(nullptr, IDI_APPLICATION);    // Default application icon
    window_class.hCursor = LoadCursor(nullptr, IDC_ARROW);  // Default arrow cursor
    window_class.hbrBackground = static_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));  // Default white background
    window_class.lpszMenuName = nullptr;    // No menu
    window_class.lpszClassName = class_name_;   // Name of the window class
    const ATOM atom = RegisterClass(&window_class);   // Register the window class
    assert(atom != 0);
}

void Win32Wnd::CreateWnd() {
    constexpr DWORD style = WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
    // Adjust the window size to fit the client area
    RECT rect = {0, 0, width_, height_};
    AdjustWindowRect(&rect, style, FALSE);
    const int adjusted_width = rect.right - rect.left;
    const int adjusted_height = rect.bottom - rect.top;
    hwnd_ = CreateWindow(
        class_name_,
        window_title_,
        style,
        CW_USEDEFAULT, CW_USEDEFAULT, adjusted_width, adjusted_height,
        nullptr,
        nullptr,
        hinstance_,
        this
    );
    assert(hwnd_ != nullptr);
}

void Win32Wnd::CreateMemoryDC() {
    HDC hdc = GetDC(hwnd_);
    pixels_dc_ = CreateCompatibleDC(hdc);
    ReleaseDC(hwnd_, hdc);
    assert(pixels_dc_ != nullptr);
}

void Win32Wnd::CreateDeviceIndependentBitmap() {
    BITMAPINFO bitmap_info = {};
    bitmap_info.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bitmap_info.bmiHeader.biWidth = width_;
    bitmap_info.bmiHeader.biHeight = -height_;    // Negative height to make the origin at the top-left corner
    bitmap_info.bmiHeader.biPlanes = 1;
    bitmap_info.bmiHeader.biBitCount = 32;
    bitmap_info.bmiHeader.biCompression = BI_RGB;
    pixel_bitmap_ = CreateDIBSection(
        pixels_dc_,
        &bitmap_info,
        DIB_RGB_COLORS,
        reinterpret_cast<void **>(&pixels_buffer_),
        nullptr,
        0);
    assert(pixel_bitmap_ != nullptr);
    SelectObject(pixels_dc_, pixel_bitmap_);
}

LRESULT Win32Wnd::ProcessMsg(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    auto* windows = reinterpret_cast<Win32Wnd*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

    if (windows == nullptr) return DefWindowProc(hWnd, uMsg, wParam, lParam);

    switch (uMsg) {
        case WM_DESTROY:        windows->is_running_ = false;   return 0;
        case WM_KEYDOWN:        ProcessKey(windows, wParam);         return 0;
        case WM_LBUTTONDOWN:    ProcessMouse(windows, L);            return 0;
        case WM_RBUTTONDOWN:    ProcessMouse(windows, R);            return 0;
        case WM_MOUSEWHEEL:     ProcessScroll(windows, wParam);      return 0;
        default: return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }
}

void Win32Wnd::ProcessKey(Win32Wnd* windows, const WPARAM wParam) {
    KeyCode key_code;
    switch (wParam) {
        case VK_ESCAPE: key_code = ESC;     break;
        case 'A':       key_code = A;       break;
        case 'D':       key_code = D;       break;
        case 'W':       key_code = W;       break;
        case 'S':       key_code = S;       break;
        case 'Q':       key_code = Q;       break;
        case 'E':       key_code = E;       break;
        case VK_SPACE:  key_code = SPACE;   break;
        default:                            return;
    }
    if (!windows->key_callbacks_.empty()) {
        for (auto& callback: windows->key_callbacks_)
            callback(windows, key_code);
    }
}

void Win32Wnd::ProcessMouse(Win32Wnd* windows, const MouseCode mouse_code) {
    if (!windows->mouse_callbacks_.empty()) {
        for (auto& callback: windows->mouse_callbacks_)
            callback(windows, mouse_code);
    }
}

void Win32Wnd::ProcessScroll(Win32Wnd* windows, const WPARAM wParam) {
    const double offset = GET_WHEEL_DELTA_WPARAM(wParam) / WHEEL_DELTA;
    if (!windows->scroll_callbacks_.empty()) {
        for (auto& callback: windows->scroll_callbacks_)
            callback(windows, offset);
    }
}
