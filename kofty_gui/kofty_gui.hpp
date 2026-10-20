#ifndef KOFTY_GUI_HPP
#define KOFTY_GUI_HPP

#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/raylib.hpp>
#include <TGUI/Widgets/EditBox.hpp>
#include <string>
#include <vector>
#include <map>

namespace kofty_gui_detail {
    inline tgui::Gui* g_gui = nullptr;

    inline std::map<int, tgui::Widget::Ptr> g_widgets;
    inline std::map<int, tgui::Container::Ptr> g_layouts;
    inline std::map<int, tgui::Grid::Ptr> g_grids;

    inline int g_next_id = 0;
    inline int g_last_clicked_id = -1;
}

// --- WINDOW & LIFECYCLE ---------------------------------------------
inline void gui_window(int w, int h, const std::string& title) {
    SetTraceLogLevel(LOG_ERROR);
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(w, h, title.c_str());
    SetTargetFPS(60);

    kofty_gui_detail::g_gui = new tgui::Gui();
    kofty_gui_detail::g_widgets.clear();
    kofty_gui_detail::g_layouts.clear();
    kofty_gui_detail::g_grids.clear();
    kofty_gui_detail::g_next_id = 0;
    kofty_gui_detail::g_last_clicked_id = -1;
}

inline void gui_close() {
    //delete kofty_gui_detail::g_gui;
    kofty_gui_detail::g_gui = nullptr;
    CloseWindow();
}

inline bool gui_running() {
    return !WindowShouldClose();
}

inline void gui_update() {
    auto* gui = kofty_gui_detail::g_gui;
    gui->setAbsoluteViewport(tgui::FloatRect(
        {0.f, 0.f}, {(float)GetScreenWidth(), (float)GetScreenHeight()}));
    gui->handleEvents();
    while (int ch = GetCharPressed()) gui->handleCharPressed(ch);
    while (int key = GetKeyPressed())  gui->handleKeyPressed(key);

    BeginDrawing();
    ClearBackground({240, 240, 240, 255});
    gui->draw();
    EndDrawing();
}

// --- INTERNAL HELPERS -----------------------------------------------
inline tgui::Container::Ptr _kofty_parent(int parent_id) {
    if (parent_id >= 0 && kofty_gui_detail::g_layouts.count(parent_id)) {
        return kofty_gui_detail::g_layouts[parent_id];
    }
    // Root window
    return nullptr;
}

inline void _kofty_attach_container(int layout_id, int parent_id) {
    auto layout = kofty_gui_detail::g_layouts[layout_id];

    if (parent_id >= 0 && kofty_gui_detail::g_grids.count(parent_id)) {
        kofty_gui_detail::g_grids[parent_id]->addWidget(layout, 0, 0);
        return;
    }
    if (parent_id >= 0 && kofty_gui_detail::g_layouts.count(parent_id)) {
        kofty_gui_detail::g_layouts[parent_id]->add(layout);
        return;
    }
    kofty_gui_detail::g_gui->add(layout);
}

inline void _kofty_attach_widget(int widget_id, int parent_id, int row, int col) {
    auto w = kofty_gui_detail::g_widgets[widget_id];

    if (parent_id >= 0 && kofty_gui_detail::g_grids.count(parent_id)) {
        kofty_gui_detail::g_grids[parent_id]->addWidget(w, row, col);
        return;
    }
    if (parent_id >= 0 && kofty_gui_detail::g_layouts.count(parent_id)) {
        kofty_gui_detail::g_layouts[parent_id]->add(w);
        return;
    }
    kofty_gui_detail::g_gui->add(w);
}

// --- LAYOUTS --------------------------------------------------------
// orientation: 0 = Vertical, 1 = Horizontal
inline int gui_add_boxlayout(int orientation, int spacing, int parent_id) {
    tgui::Container::Ptr layout;
    if (orientation == 0) {
        auto v = tgui::VerticalLayout::create();
        v->getRenderer()->setSpaceBetweenWidgets(spacing);
        layout = v;
    } else {
        auto h = tgui::HorizontalLayout::create();
        h->getRenderer()->setSpaceBetweenWidgets(spacing);
        layout = h;
    }
    int id = kofty_gui_detail::g_next_id++;
    kofty_gui_detail::g_layouts[id] = layout;
    layout->setSize("100%", "100%");
    _kofty_attach_container(id, parent_id);
    return id;
}

inline int gui_add_gridlayout(int cols, int spacing, int parent_id) {
    auto grid = tgui::Grid::create();
    int id = kofty_gui_detail::g_next_id++;
    kofty_gui_detail::g_layouts[id] = grid;
    kofty_gui_detail::g_grids[id] = grid;
    grid->setSize("100%", "100%");
    _kofty_attach_container(id, parent_id);
    return id;
}

// --- WIDGETS --------------------------------------------------------
// row/col only used when parent is a grid
inline int gui_add_button(const std::string& text,
                          int parent_id,
                          int row = 0, int col = 0) {
    auto btn = tgui::Button::create(text);
    int id = kofty_gui_detail::g_next_id++;
    kofty_gui_detail::g_widgets[id] = btn;
    _kofty_attach_widget(id, parent_id, row, col);
    btn->onPress([id]() {
        kofty_gui_detail::g_last_clicked_id = id;
    });
    return id;
}

inline int gui_add_label(const std::string& text,
                         int parent_id,
                         int row = 0, int col = 0) {
    auto lbl = tgui::Label::create(text);
    lbl->setTextSize(20);
    int id = kofty_gui_detail::g_next_id++;
    kofty_gui_detail::g_widgets[id] = lbl;
    _kofty_attach_widget(id, parent_id, row, col);
    return id;
}

// --- PROPERTY SETTERS -----------------------------------------------
// text
inline void gui_set_text(int widget_id, const std::string& text) {
    if (!kofty_gui_detail::g_widgets.count(widget_id)) return;
    auto w = kofty_gui_detail::g_widgets[widget_id];
    if (auto b = std::dynamic_pointer_cast<tgui::Button>(w)) b->setText(text);
    else if (auto l = std::dynamic_pointer_cast<tgui::Label>(w)) l->setText(text);
}

// size (optional — Kivy-style: layouts decide if omitted)
inline void gui_set_size(int widget_id, int w, int h) {
    if (!kofty_gui_detail::g_widgets.count(widget_id)) return;
    kofty_gui_detail::g_widgets[widget_id]->setSize((float)w, (float)h);
}

// position (absolute — only meaningful when widget is directly on root)
inline void gui_set_pos(int widget_id, int x, int y) {
    if (!kofty_gui_detail::g_widgets.count(widget_id)) return;
    kofty_gui_detail::g_widgets[widget_id]->setPosition((float)x, (float)y);
}

// button color
inline void gui_set_button_color(int widget_id, int r, int g, int b) {
    if (!kofty_gui_detail::g_widgets.count(widget_id)) return;
    auto w = kofty_gui_detail::g_widgets[widget_id];
    if (auto btn = std::dynamic_pointer_cast<tgui::Button>(w)) {
        btn->getRenderer()->setBackgroundColor(tgui::Color(r, g, b));
        btn->getRenderer()->setBackgroundColorHover(
            tgui::Color(r > 20 ? r - 20 : 0, g > 20 ? g - 20 : 0, b > 20 ? b - 20 : 0));
    }
}

// label color
inline void gui_set_label_color(int widget_id, int r, int g, int b) {
    if (!kofty_gui_detail::g_widgets.count(widget_id)) return;
    auto w = kofty_gui_detail::g_widgets[widget_id];
    if (auto lbl = std::dynamic_pointer_cast<tgui::Label>(w)) {
        lbl->getRenderer()->setTextColor(tgui::Color(r, g, b));
    }
}

// --- EVENT POLLING --------------------------------------------------
inline int gui_check_click() {
    int clicked = kofty_gui_detail::g_last_clicked_id;
    kofty_gui_detail::g_last_clicked_id = -1;
    return clicked;
}

#endif