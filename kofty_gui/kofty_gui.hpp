#ifndef KOFTY_GUI_HPP
#define KOFTY_GUI_HPP

#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/raylib.hpp>
#include <TGUI/Widgets/EditBox.hpp> 
#include <string>
#include <vector>

namespace kofty_gui_detail {
    inline tgui::Gui* g_gui = nullptr;
    
    inline std::vector<tgui::Button::Ptr> g_buttons;
    inline std::vector<tgui::Label::Ptr> g_labels;
    inline std::vector<tgui::EditBox::Ptr> g_textboxes;
    inline std::vector<tgui::Slider::Ptr> g_sliders;
    inline std::vector<tgui::Group::Ptr> g_layouts;
    
    inline int g_last_clicked_index = -1; 
}

inline tgui::Container* get_container(int parent_id) {
    if (parent_id >= 0 && parent_id < (int)kofty_gui_detail::g_layouts.size()) {
        return kofty_gui_detail::g_layouts[parent_id].get();
    }
    return reinterpret_cast<tgui::Container*>(kofty_gui_detail::g_gui);
}

inline void gui_window(int w, int h, const std::string& title) {
    printf("1. Before InitWindow\n"); fflush(stdout);
    SetTraceLogLevel(LOG_ERROR);
    InitWindow(w, h, title.c_str());
    printf("2. After InitWindow, IsWindowReady=%d\n", IsWindowReady()); fflush(stdout);
    SetTargetFPS(60);
    kofty_gui_detail::g_gui = new tgui::Gui();
    printf("3. After Gui creation\n"); fflush(stdout);
    
    kofty_gui_detail::g_buttons.clear();
    kofty_gui_detail::g_labels.clear();
    kofty_gui_detail::g_textboxes.clear();
    kofty_gui_detail::g_sliders.clear();
    kofty_gui_detail::g_layouts.clear();
    kofty_gui_detail::g_last_clicked_index = -1;
}
inline int gui_add_boxlayout(int orientation, int x, int y, int w, int h, int parent_id) {
    auto group = tgui::Group::create();
    group->setPosition(x, y);
    group->setSize(w, h);
    int index = kofty_gui_detail::g_layouts.size();
    kofty_gui_detail::g_layouts.push_back(group);
    get_container(parent_id)->add(group);
    return index;
}

inline int gui_add_gridlayout(int cols, int x, int y, int w, int h, int parent_id) {
    auto group = tgui::Group::create();
    group->setPosition(x, y);
    group->setSize(w, h);
    int index = kofty_gui_detail::g_layouts.size();
    kofty_gui_detail::g_layouts.push_back(group);
    get_container(parent_id)->add(group);
    return index;
}

inline int gui_add_label(const std::string& text, int x, int y, int parent_id) {
    auto label = tgui::Label::create(text);
    label->setPosition(x, y);
    label->setTextSize(20);
    int index = kofty_gui_detail::g_labels.size();
    kofty_gui_detail::g_labels.push_back(label);
    kofty_gui_detail::g_gui->add(label);
    return index;
}

inline void gui_set_label(int index, const std::string& text) {
    if (index >= 0 && index < (int)kofty_gui_detail::g_labels.size()) {
        kofty_gui_detail::g_labels[index]->setText(text);
    }
}

inline int gui_add_button(const std::string& label, int x, int y, int bw, int bh, int parent_id) {
    auto button = tgui::Button::create(label);
    button->setPosition(x, y);
    button->setSize(bw, bh);
    int index = kofty_gui_detail::g_buttons.size();
    kofty_gui_detail::g_buttons.push_back(button);
    kofty_gui_detail::g_gui->add(button);
    button->onPress([index]() {
        kofty_gui_detail::g_last_clicked_index = index;
    });
    return index;
}

inline void gui_set_button_text(int index, const std::string& text) {
    if (index >= 0 && index < (int)kofty_gui_detail::g_buttons.size()) {
        kofty_gui_detail::g_buttons[index]->setText(text);
    }
}

inline int gui_add_textbox(const std::string& placeholder, int x, int y, int w, int h, int parent_id) {
    auto box = tgui::EditBox::create();
    box->setPosition(x, y);
    box->setSize(w, h);
    box->setDefaultText(placeholder);
    int index = kofty_gui_detail::g_textboxes.size();
    kofty_gui_detail::g_textboxes.push_back(box);
    get_container(parent_id)->add(box);
    return index;
}

inline const char* gui_get_textbox_text(int index) {
    if (index >= 0 && index < (int)kofty_gui_detail::g_textboxes.size()) {
        static std::string temp;
        temp = kofty_gui_detail::g_textboxes[index]->getText().toStdString();
        return temp.c_str();
    }
    return "";
}

inline int gui_add_slider(int x, int y, int w, int h, int min_val, int max_val, int parent_id) {
    auto slider = tgui::Slider::create();
    slider->setPosition(x, y);
    slider->setSize(w, h);
    slider->setMinimum(min_val);
    slider->setMaximum(max_val);
    int index = kofty_gui_detail::g_sliders.size();
    kofty_gui_detail::g_sliders.push_back(slider);
    get_container(parent_id)->add(slider);
    return index;
}

inline int gui_get_slider_value(int index) {
    if (index >= 0 && index < (int)kofty_gui_detail::g_sliders.size()) {
        return (int)kofty_gui_detail::g_sliders[index]->getValue();
    }
    return 0;
}

inline bool gui_running() {
    return !WindowShouldClose();
}

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

inline int gui_check_click() {
    int clicked = kofty_gui_detail::g_last_clicked_index;
    kofty_gui_detail::g_last_clicked_index = -1;
    return clicked;
}

inline void gui_close() {
    delete kofty_gui_detail::g_gui;
    kofty_gui_detail::g_gui = nullptr;
    CloseWindow();
}

#endif