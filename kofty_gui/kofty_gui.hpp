#ifndef KOFTY_GUI_HPP
#define KOFTY_GUI_HPP

#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/raylib.hpp>
#include <TGUI/Widgets/EditBox.hpp> 
#include <string>
#include <vector>

namespace kofty_gui_detail {
    inline tgui::Gui* g_gui = nullptr;
    
    inline std::vector<tgui::ComboBox::Ptr> g_comboboxes;
    inline std::vector<tgui::ProgressBar::Ptr> g_progressbars;
    inline std::vector<tgui::Button::Ptr> g_buttons;
    inline std::vector<tgui::Label::Ptr> g_labels;
    inline std::vector<tgui::EditBox::Ptr> g_textboxes;
    inline std::vector<tgui::Slider::Ptr> g_sliders;
    
    inline int g_last_clicked_index = -1; 
}

inline void gui_window(int w, int h, const std::string& title) {
    SetTraceLogLevel(LOG_ERROR);
    InitWindow(w, h, title.c_str());
    SetTargetFPS(60);
    kofty_gui_detail::g_gui = new tgui::Gui();
    
    kofty_gui_detail::g_comboboxes.clear();
    kofty_gui_detail::g_progressbars.clear();
    kofty_gui_detail::g_buttons.clear();
    kofty_gui_detail::g_labels.clear();
    kofty_gui_detail::g_textboxes.clear();
    kofty_gui_detail::g_sliders.clear();
    kofty_gui_detail::g_last_clicked_index = -1;
}

inline int gui_add_label(const std::string& text, int x, int y) {
    auto label = tgui::Label::create(text);
    label->setPosition(x, y);
    label->setTextSize(20);
    int index = kofty_gui_detail::g_labels.size();
    kofty_gui_detail::g_labels.push_back(label);
    kofty_gui_detail::g_gui->add(label);
    return index;
}

inline void gui_set_label(int index, const std::string& text) {
    if (index >= 0 && index < kofty_gui_detail::g_labels.size()) {
        kofty_gui_detail::g_labels[index]->setText(text);
    }
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
    button->onPress([index]() {
        kofty_gui_detail::g_last_clicked_index = index;
    });
    return index;
}

inline void gui_set_button_text(int index, const std::string& text) {
    if (index >= 0 && index < kofty_gui_detail::g_buttons.size()) {
        kofty_gui_detail::g_buttons[index]->setText(text);
    }
}

inline int gui_add_textbox(const std::string& placeholder, int x, int y, int w, int h) {
    auto box = tgui::EditBox::create();
    box->setPosition(x, y);
    box->setSize(w, h);
    box->setDefaultText(placeholder);
    int index = kofty_gui_detail::g_textboxes.size();
    kofty_gui_detail::g_textboxes.push_back(box);
    kofty_gui_detail::g_gui->add(box);
    return index;
}

inline const char* gui_get_textbox_text(int index) {
    if (index >= 0 && index < kofty_gui_detail::g_textboxes.size()) {
        static std::string temp;
        temp = kofty_gui_detail::g_textboxes[index]->getText().toStdString();
        return temp.c_str();
    }
    return "";
}

inline int gui_add_slider(int x, int y, int w, int h, int min_val, int max_val) {
    auto slider = tgui::Slider::create();
    slider->setPosition(x, y);
    slider->setSize(w, h);
    slider->setMinimum(min_val);
    slider->setMaximum(max_val);
    int index = kofty_gui_detail::g_sliders.size();
    kofty_gui_detail::g_sliders.push_back(slider);
    kofty_gui_detail::g_gui->add(slider);
    return index;
}

inline int gui_get_slider_value(int index) {
    if (index >= 0 && index < kofty_gui_detail::g_sliders.size()) {
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

// --- COMBOBOX (Dropdown) ---
inline int gui_add_dropdown(int x, int y, int w) {
    auto box = tgui::ComboBox::create();
    box->setPosition(x, y);
    box->setSize(w, 30);
    int index = kofty_gui_detail::g_comboboxes.size();
    kofty_gui_detail::g_comboboxes.push_back(box);
    kofty_gui_detail::g_gui->add(box);
    return index;
}

inline void gui_add_dropdown_item(int index, const std::string& item) {
    if (index >= 0 && index < kofty_gui_detail::g_comboboxes.size()) {
        kofty_gui_detail::g_comboboxes[index]->addItem(item);
    }
}

inline int gui_get_dropdown_index(int index) {
    if (index >= 0 && index < kofty_gui_detail::g_comboboxes.size()) {
        return kofty_gui_detail::g_comboboxes[index]->getSelectedItemIndex();
    }
    return -1;
}

// --- PROGRESS BAR ---
inline int gui_add_progressbar(int x, int y, int w, int h, int max_val) {
    auto bar = tgui::ProgressBar::create();
    bar->setPosition(x, y);
    bar->setSize(w, h);
    bar->setMaximum(max_val);
    int index = kofty_gui_detail::g_progressbars.size();
    kofty_gui_detail::g_progressbars.push_back(bar);
    kofty_gui_detail::g_gui->add(bar);
    return index;
}

inline void gui_set_progressbar_value(int index, int value) {
    if (index >= 0 && index < kofty_gui_detail::g_progressbars.size()) {
        kofty_gui_detail::g_progressbars[index]->setValue(value);
    }
}

#endif