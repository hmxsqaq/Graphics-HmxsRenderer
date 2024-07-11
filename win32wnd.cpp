#include "win32wnd.h"

Win32Wnd::Win32Wnd(std::wstring class_name, std::wstring window_title)
    : is_running_(false), class_name_(std::move(class_name)), window_title_(std::move(window_title)), h_instance_(GetModuleHandle(nullptr)), hwnd_(nullptr) {
    InitWnd();
}

Win32Wnd::~Win32Wnd() {
    if (hwnd_) DestroyWindow(hwnd_);
}

void Win32Wnd::CreateWnd(int width, int height) {
    // Adjust the window size to fit the client area
    RECT rect = {0, 0, width, height};
    AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);
    width = rect.right - rect.left;
    height = rect.bottom - rect.top;
    hwnd_ = CreateWindow(
        reinterpret_cast<LPCSTR>(class_name_.c_str()),
        reinterpret_cast<LPCSTR>(window_title_.c_str()),
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, width, height,
        nullptr,
        nullptr,
        h_instance_,
        this
    );
    assert(hwnd_ != nullptr);
    ShowWindow(hwnd_, SW_SHOW);
    SetWindowLongPtr(hwnd_, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
}

void Win32Wnd::HandleMsg() {
    MSG msg = {};
    while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

void Win32Wnd::InitWnd() {
    RegisterWndClass();
    is_running_ = true;
}

void Win32Wnd::RegisterWndClass() const {
    WNDCLASS window_class = {};
    window_class.style = CS_HREDRAW | CS_VREDRAW;   // Redraw on size change
    window_class.lpfnWndProc = ProcessMsg;  // Window Message Procedure Callback
    window_class.cbClsExtra = 0;    // No extra bytes after the window class
    window_class.cbWndExtra = 0;    // No extra bytes after the window instance
    window_class.hInstance = h_instance_;   // Instance of the application
    window_class.hIcon = LoadIcon(nullptr, IDI_APPLICATION);    // Default application icon
    window_class.hCursor = LoadCursor(nullptr, IDC_ARROW);  // Default arrow cursor
    window_class.hbrBackground = static_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));  // Default white background
    window_class.lpszMenuName = nullptr;    // No menu
    window_class.lpszClassName = reinterpret_cast<LPCSTR>(class_name_.c_str());   // Name of the window class
    const ATOM atom = RegisterClass(&window_class);   // Register the window class
    assert(atom != 0);
}

LRESULT Win32Wnd::ProcessMsg(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    auto* wnd = reinterpret_cast<Win32Wnd*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

    if (wnd == nullptr) return DefWindowProc(hWnd, uMsg, wParam, lParam);

    if (uMsg == WM_DESTROY) {
        wnd->is_running_ = false;
        return 0;
    }
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}
