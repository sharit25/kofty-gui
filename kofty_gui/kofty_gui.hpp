#ifndef KOFTY_GUI_HPP
#define KOFTY_GUI_HPP

#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/raylib.hpp>
#include <string>
#include <vector>

namespace kofty_gui_detail {
    inline tgui::Gui* g_gui = nullptr;
    inline std::vector<tgui::Button::Ptr> g_buttons;
    inline int g_last_clicked_index = -1; 
}

inline void gui_window(int w, int h, const std::string& title) {
    SetTraceLogLevel(LOG_ERROR);
    InitWindow(w, h, title.c_str());
    SetTargetFPS(60);
    kofty_gui_detail::g_gui = new tgui::Gui();
    kofty_gui_detail::g_buttons.clear();
    kofty_gui_detail::g_last_clicked_index = -1;
}

inline void gui_add_label(const std::string& text, int x, int y) {
    auto label = tgui::Label::create(text);
    label->setPosition(x, y);
    label->setTextSize(20);
    kofty_gui_detail::g_gui->add(label);
}

inline void gui_add_checkbox(const std::string& text, int x, int y) {
    auto checkbox = tgui::CheckBox::create(text);
    checkbox->setPosition(x, y);
    kofty_gui_detail::g_gui->add(checkbox);
}

inline int gui_add_button(const std::string& label, int x, int y, int bw, int bh) {
    auto button = tgui::Button::create(label);
    button->setPosition(x, y);
    button->setSize(bw, bh);
    
    int index = kofty_gui_detail::g_buttons.size();
    kofty_gui_detail::g_buttons.push_back(button);
    kofty_gui_detail::g_gui->add(button);
    
    // Tell TGUI: when this button is clicked, save its index!
    button->onPress([index]() {
        kofty_gui_detail::g_last_clicked_index = index;
    });
    
    return index;
}

// Returns true as long as the window is open
inline bool gui_running() {
    return !WindowShouldClose();
}

// Call this every frame to draw the GUI
inline void gui_update() {
    auto* gui = kofty_gui_detail::g_gui;
    
    gui->handleEvents();
    while (int ch = GetCharPressed()) gui->handleCharPressed(ch);
    while (int key = GetKeyPressed())  gui->handleKeyPressed(key);

    BeginDrawing();
    ClearBackground({240, 240, 240, 255});
    gui->draw();
    EndDrawing();
}

// Returns the index of the button clicked THIS frame, then resets it.
inline int gui_check_click() {
    int clicked = kofty_gui_detail::g_last_clicked_index;
    kofty_gui_detail::g_last_clicked_index = -1; // Reset for next frame
    return clicked;
}

// Call this when the while loop ends
inline void gui_close() {
    delete kofty_gui_detail::g_gui;
    kofty_gui_detail::g_gui = nullptr;
    CloseWindow();
}

#endif