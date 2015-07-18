#include "minig.h"
struct mouse Mouse;
void update_kbkeys(void) {
    KBstate = SDL_GetKeyboardState(NULL);
}

bool kbk(SDL_Keycode keycode) {
    int key = SDL_GetScancodeFromKey(keycode);
    return KBstate[key];
}

float Accel[3] = {1.f, 1.f, 1.f};

void mouse_click(Uint8 button, bool status) {
    switch(button) {
        case SDL_BUTTON_LEFT :   Mouse.b1 = status; break;
        case SDL_BUTTON_MIDDLE : Mouse.b2 = status; break;
        case SDL_BUTTON_RIGHT :  Mouse.b3 = status; break;
        case SDL_BUTTON_X1 : Mouse.b1 = status;     break;
        case SDL_BUTTON_X2 : Mouse.b1 = status;     break;
        default: break;
    }
}

void mouse_update_pos(void) {
    SDL_GetMouseState(&Mouse.x, &Mouse.y);
}//TODO mouse
void accel_update(void) {}

void poll_events(void) {
    update_kbkeys();
#if defined(ANDROID)
    if (Android_JNI_GetAccelerometerValues(Accel)) accel_update();
#endif
    while(SDL_PollEvent(&Event)) {
        switch(Event.type) {

            case SDL_QUIT: Mi.running = 0; break;
            case SDL_KEYDOWN: break;
            case SDL_KEYUP: break;

            case SDL_MOUSEBUTTONDOWN:
                if(Event.button.which != SDL_TOUCH_MOUSEID) mouse_click(Event.button.button, true); break;//TODO mouse
            case SDL_MOUSEBUTTONUP:
                if(Event.button.which != SDL_TOUCH_MOUSEID) mouse_click(Event.button.button, false); break;
            case SDL_MOUSEMOTION:
                if(Event.button.which != SDL_TOUCH_MOUSEID) mouse_update_pos(); break;
            case SDL_MOUSEWHEEL:
                if(Event.button.which != SDL_TOUCH_MOUSEID) mouse_update_pos(); break;

            case SDL_FINGERDOWN:   touch_down(Event); break;
            case SDL_FINGERUP:     touch_up(Event); break;
            case SDL_FINGERMOTION: touch_motion(Event); break;

            case SDL_JOYAXISMOTION: joy_axismot(Event); break;
            case SDL_JOYBUTTONDOWN: joy_button_down(Event); break;
            case SDL_JOYBUTTONUP:   joy_button_up(Event); break;
            case SDL_JOYDEVICEADDED:   joy_dev_add(Event); break;
            case SDL_JOYDEVICEREMOVED: joy_dev_rem(Event); break;

            default: break;

        }

    }

}
