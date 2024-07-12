#include "win32_wnd.h"

Win32Wnd::Win32Wnd(const LPCSTR& class_name, const LPCSTR& window_title)
    : class_name_(class_name), window_title_(window_title), width_(0),
      height_(0), hinstance_(GetModuleHandle(nullptr)), hwnd_(nullptr),
      memory_dc_(nullptr), d_i_bitmap_(nullptr), should_shutdown_(true) {
    assert(hinstance_ != nullptr);
}

Win32Wnd::~Win32Wnd() {
    if (hwnd_) DestroyWindow(hwnd_);
    UnregisterClass(class_name_, hinstance_);
}

void Win32Wnd::Open(const int width, const int height) {
    assert(width > 0 && height > 0);
    width_ = width;
    height_ = height;
    RegisterWndClass();
    CreateWnd();
    CreateMemoryDC();

    SetWindowLongPtr(hwnd_, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this)); // Store the pointer to the Win32Wnd object for ProcessMsg
    ShowWindow(hwnd_, SW_SHOW);
    should_shutdown_ = false;
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
    // Adjust the window size to fit the client area
    RECT rect = {0, 0, width_, height_};
    AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);
    const int adjusted_width = rect.right - rect.left;
    const int adjusted_height = rect.bottom - rect.top;
    hwnd_ = CreateWindow(
        class_name_,
        window_title_,
        WS_OVERLAPPEDWINDOW,
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
    memory_dc_ = CreateCompatibleDC(hdc);
    ReleaseDC(hwnd_, hdc);
    assert(memory_dc_ != nullptr);
}

void Win32Wnd::CreateDeviceIndependentBitmap() {
    BITMAPINFO bitmap_info = {};
    bitmap_info.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bitmap_info.bmiHeader.biWidth = width_;
    bitmap_info.bmiHeader.biHeight = -height_;    // Negative height to make the origin at the top-left corner
    bitmap_info.bmiHeader.biPlanes = 1;
    bitmap_info.bmiHeader.biBitCount = 32;
    bitmap_info.bmiHeader.biCompression = BI_RGB;
    d_i_bitmap_ = CreateDIBSection(memory_dc_, &bitmap_info, DIB_RGB_COLORS, nullptr, nullptr, 0);
    assert(d_i_bitmap_ != nullptr);
    SelectObject(memory_dc_, d_i_bitmap_);
}

LRESULT Win32Wnd::ProcessMsg(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    auto* wnd = reinterpret_cast<Win32Wnd*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

    if (wnd == nullptr) return DefWindowProc(hWnd, uMsg, wParam, lParam);

    if (uMsg == WM_DESTROY) {
        wnd->should_shutdown_ = true;
        return 0;
    }
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}
