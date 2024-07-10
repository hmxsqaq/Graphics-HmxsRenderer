#ifndef WIN32WND_H
#define WIN32WND_H

#include <windows.h>
#include <string>
#include <cassert>

class Win32Wnd {
public:
    Win32Wnd(std::wstring class_name, std::wstring window_title);
    ~Win32Wnd();

    void Create(int width, int height);
private:
    HINSTANCE h_instance_;
    std::wstring class_name_;
    std::wstring window_title_;
    HWND hwnd_;

    void RegisterWndClass() const;
    static LRESULT CALLBACK ProcessMsg(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};

#endif //WIN32WND_H
