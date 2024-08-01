#ifndef CALLBACKS_H
#define CALLBACKS_H

#include "scene.h"
#include "win32_wnd.h"
#include "utility/log.h"

struct Callbacks {
    static void OnKeyPressed(Win32Wnd* windows, const KeyCode keycode) {
        const auto scene = static_cast<Scene*>(windows->GetUserData().get());
        if (scene == nullptr) {
            LOG_WARNING("Callbacks - cannot get scene object from user data");
            return;
        }
        const auto camera = scene->GetCamera();
        switch (keycode) {
            case A:
                camera->transform.position[0] += 0.1f;
                break;
            case D:
                camera->transform.position[0] -= 0.1f;
                break;
            case W:
                camera->transform.position[2] -= 0.1f;
                break;
            case S:
                camera->transform.position[2] += 0.1f;
                break;
            case Q:
                camera->transform.position[1] += 0.1f;
                break;
            case E:
                camera->transform.position[1] -= 0.1f;
                break;
            case SPACE:
                camera->transform.position = {0, 0, 5};
                break;
            case ESC:
                windows->CloseWnd();
                break;
        }
    }
};



#endif //CALLBACKS_H
