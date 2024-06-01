#pragma once
class switcher
{
public:
    bool firstMouse;
    bool mouse_enable;
    bool show_work_window;
    bool show_another_window;
    bool show_hint;
    bool blinn;
    bool blinnKeyPressed;
    bool key_c;
    bool save;
    switcher()
        :firstMouse(true), mouse_enable(true), show_work_window(true),
        show_another_window(false), show_hint(true), blinn(false),
        blinnKeyPressed(false), key_c(false),save(false)
    {
    }
};