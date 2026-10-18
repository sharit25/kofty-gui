#ifndef KOFTY_GUI_HPP
#define KOFTY_GUI_HPP

#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/raylib.hpp>
#include <string>
#include <vector>

namespace kofty_gui_detail {
    inline tgui::Gui* g_gui = nullptr;
    inline std::vector<tgui::Button::Ptr> g_buttons;
}

inline void gui_window(int w, int h, const std::string& title) {
    SetTraceLogLevel(LOG_ERROR);
    InitWindow(w, h, title.c_str());
    SetTargetFPS(60);
    kofty_gui_detail::g_gui = new tgui::Gui();
    kofty_gui_detail::g_buttons.clear();
}

inline int gui_add_button(const std::string& label, int x, int y, int bw, int bh) {
    auto button = tgui::Button::create(label);
    button->setPosition(x, y);
    button->setSize(bw, bh);
    
    int index = kofty_gui_detail::g_buttons.size();
    kofty_gui_detail::g_buttons.push_back(button);
    kofty_gui_detail::g_gui->add(button);
    
    return index;
}

inline void gui_run() {
    auto* gui = kofty_gui_detail::g_gui;
    while (!WindowShouldClose()) {
        gui->handleEvents();
        while (int ch = GetCharPressed()) gui->handleCharPressed(ch);
        while (int key = GetKeyPressed())  gui->handleKeyPressed(key);

        BeginDrawing();
        ClearBackground({240, 240, 240, 255});
        gui->draw();
        EndDrawing();
    }
    delete gui;
    kofty_gui_detail::g_gui = nullptr;
    CloseWindow();

}

#endif