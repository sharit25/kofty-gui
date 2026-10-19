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
    inline std::vector<tgui::ProgressBar::Ptr> g_progressbars;
    inline std::vector<tgui::ComboBox::Ptr> g_dropdowns;
    inline std::vector<tgui::Group::Ptr> g_layouts;
    
    inline int g_last_clicked_index = -1; 
}

inline tgui::Container::Ptr get_container(int parent_id) {
    if (parent_id >= 0 && parent_id < (int)kofty_gui_detail::g_layouts.size()) {
        return kofty_gui_detail::g_layouts[parent_id];
    }
    return kofty_gui_detail::g_gui->getContainer();
}

inline void gui_window(int w, int h, const std::string& title) {
    SetTraceLogLevel(LOG_ERROR);
    InitWindow(w, h, title.c_str());
    SetTargetFPS(60);
    kofty_gui_detail::g_gui = new tgui::Gui();
    
    kofty_gui_detail::g_buttons.clear();
    kofty_gui_detail::g_labels.clear();
    kofty_gui_detail::g_textboxes.clear();
    kofty_gui_detail::g_sliders.clear();
    kofty_gui_detail::g_progressbars.clear();
    kofty_gui_detail::g_dropdowns.clear();
    kofty_gui_detail::g_layouts.clear();
    kofty_gui_detail::g_last_clicked_index = -1;
}

// --- LAYOUTS ---
// orientation: 0 = Vertical, 1 = Horizontal
inline int gui_add_boxlayout(int orientation, int spacing, int x, int y, int w, int h, int parent_id) {
    tgui::Group::Ptr layout;
    if (orientation == 0) {
        auto v = tgui::VerticalLayout::create();
        v->getRenderer()->setSpaceBetweenWidgets(spacing);
        layout = v;
    } else {
        auto h = tgui::HorizontalLayout::create();
        h->getRenderer()->setSpaceBetweenWidgets(spacing);
        layout = h;
    }
    layout->setPosition(x, y);
    layout->setSize(w, h);
    
    int index = kofty_gui_detail::g_layouts.size();
    kofty_gui_detail::g_layouts.push_back(layout);
    get_container(parent_id)->add(layout);
    return index;
}

inline int gui_add_gridlayout(int cols, int x, int y, int w, int h, int parent_id) {
    // GridLayout still uses Group for now (TGUI has a Grid but it's different)
    auto group = tgui::Group::create();
    group->setPosition(x, y);
    group->setSize(w, h);
    int index = kofty_gui_detail::g_layouts.size();
    kofty_gui_detail::g_layouts.push_back(group);
    get_container(parent_id)->add(group);
    return index;
}

// --- WIDGETS ---
// Note: x and y are IGNORED if parent_id is a layout, because the layout handles positioning.
inline int gui_add_label(const std::string& text, int x, int y, int parent_id) {
    auto label = tgui::Label::create(text);
    label->setTextSize(20);
    int index = kofty_gui_detail::g_labels.size();
    kofty_gui_detail::g_labels.push_back(label);
    get_container(parent_id)->add(label);
    return index;
}

inline void gui_set_label(int index, const std::string& text) {
    if (index >= 0 && index < (int)kofty_gui_detail::g_labels.size()) {
        kofty_gui_detail::g_labels[index]->setText(text);
    }
}

inline int gui_add_button(const std::string& label, int x, int y, int bw, int bh, int parent_id) {
    auto button = tgui::Button::create(label);
    button->setSize(bw, bh);
    int index = kofty_gui_detail::g_buttons.size();
    kofty_gui_detail::g_buttons.push_back(button);
    get_container(parent_id)->add(button);
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

inline int gui_add_progressbar(int x, int y, int w, int h, int max_val, int parent_id) {
    auto bar = tgui::ProgressBar::create();
    bar->setSize(w, h);
    bar->setMaximum(max_val);
    int index = kofty_gui_detail::g_progressbars.size();
    kofty_gui_detail::g_progressbars.push_back(bar);
    get_container(parent_id)->add(bar);
    return index;
}

inline void gui_set_progressbar_value(int index, int value) {
    if (index >= 0 && index < (int)kofty_gui_detail::g_progressbars.size()) {
        kofty_gui_detail::g_progressbars[index]->setValue(value);
    }
}

inline int gui_add_dropdown(int x, int y, int w, int parent_id) {
    auto box = tgui::ComboBox::create();
    box->setSize(w, 30);
    int index = kofty_gui_detail::g_dropdowns.size();
    kofty_gui_detail::g_dropdowns.push_back(box);
    get_container(parent_id)->add(box);
    return index;
}

inline void gui_add_dropdown_item(int index, const std::string& item) {
    if (index >= 0 && index < (int)kofty_gui_detail::g_dropdowns.size()) {
        kofty_gui_detail::g_dropdowns[index]->addItem(item);
    }
}

inline int gui_get_dropdown_index(int index) {
    if (index >= 0 && index < (int)kofty_gui_detail::g_dropdowns.size()) {
        return kofty_gui_detail::g_dropdowns[index]->getSelectedItemIndex();
    }
    return -1;
}

// --- MAIN LOOP ---
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