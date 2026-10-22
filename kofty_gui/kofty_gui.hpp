#ifndef KOFTY_GUI_HPP
#define KOFTY_GUI_HPP

#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/raylib.hpp>
#include <TGUI/Widgets/EditBox.hpp>
#include <TGUI/Widgets/Panel.hpp>
#include <string>
#include <vector>
#include <map>

namespace kofty_gui_detail {
    inline tgui::Gui* g_gui = nullptr;

    inline std::map<int, tgui::Widget::Ptr> g_widgets;
    inline std::map<int, tgui::Container::Ptr> g_layouts;
    inline std::map<int, tgui::Grid::Ptr> g_grids;
    inline ::Color g_window_bg = {0, 0, 0, 255}; // Default black (raylib Color)

    inline int g_next_id = 0;
    inline int g_last_clicked_id = -1;

    struct AnchorEntry {
        int anchor_id;
        int widget_id;
        int anchor_side;
        int offset_x;
        int offset_y;
        int width;
        int height;
    };
    inline std::vector<AnchorEntry> g_anchors;
}

// --- WINDOW & LIFECYCLE ---------------------------------------------
// Provide default arguments matching Kivy
inline void gui_window(int w = 800, int h = 600, const std::string& title = "Kofty App") {
    SetTraceLogLevel(LOG_ERROR);
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(w, h, title.c_str());
    SetTargetFPS(60);
    // --- FORCE DEFAULT BACKGROUND TO BLACK ---
    kofty_gui_detail::g_window_bg = {0, 0, 0, 255};
    // ----------------------------------------

    kofty_gui_detail::g_gui = new tgui::Gui();
    kofty_gui_detail::g_widgets.clear();
    kofty_gui_detail::g_layouts.clear();
    kofty_gui_detail::g_grids.clear();
    kofty_gui_detail::g_anchors.clear();
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

inline void _kofty_apply_anchors() {
    for (auto& a : kofty_gui_detail::g_anchors) {
        if (!kofty_gui_detail::g_layouts.count(a.anchor_id)) continue;
        if (!kofty_gui_detail::g_widgets.count(a.widget_id)) continue;

        auto panel = kofty_gui_detail::g_layouts[a.anchor_id];
        auto widget = kofty_gui_detail::g_widgets[a.widget_id];

        widget->setSize((float)a.width, (float)a.height);

        float px = (float)panel->getSize().x;
        float py = (float)panel->getSize().y;
        float wx = (float)a.width;
        float wy = (float)a.height;

        float x = 0.f, y = 0.f;
        switch (a.anchor_side) {
            case 0: x = 0.f;         y = 0.f;         break; // TL
            case 1: x = (px - wx)/2; y = 0.f;         break; // TC
            case 2: x = px - wx;     y = 0.f;         break; // TR
            case 3: x = 0.f;         y = (py - wy)/2; break; // ML
            case 4: x = (px - wx)/2; y = (py - wy)/2; break; // MC
            case 5: x = px - wx;     y = (py - wy)/2; break; // MR
            case 6: x = 0.f;         y = py - wy;     break; // BL
            case 7: x = (px - wx)/2; y = py - wy;     break; // BC
            case 8: x = px - wx;     y = py - wy;     break; // BR
            default: break;
        }
        widget->setPosition(x + (float)a.offset_x, y + (float)a.offset_y);
    }
}

inline void gui_set_window_color(int r, int g, int b) {
    kofty_gui_detail::g_window_bg = { (unsigned char)r, (unsigned char)g, (unsigned char)b, 255 };
}

inline void gui_update() {
    if (!kofty_gui_detail::g_gui) return;
    if (WindowShouldClose()) return;
    auto* gui = kofty_gui_detail::g_gui;
    gui->setAbsoluteViewport(tgui::FloatRect(
        {0.f, 0.f}, {(float)GetScreenWidth(), (float)GetScreenHeight()}));
    _kofty_apply_anchors();
    gui->handleEvents();
    while (int ch = GetCharPressed()) gui->handleCharPressed(ch);
    while (int key = GetKeyPressed())  gui->handleKeyPressed(key);

    BeginDrawing();
    ClearBackground(kofty_gui_detail::g_window_bg);
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
        // --- FIX: Wrap in a Panel so the VerticalLayout doesn't stretch it ---
        auto panel = tgui::Panel::create();
        panel->getRenderer()->setBackgroundColor(tgui::Color::Transparent);
        panel->setSize("100%", w->getSize().y); // Panel takes the height of the widget
        panel->add(w);
        w->setPosition(0, 0);
        
        kofty_gui_detail::g_layouts[parent_id]->add(panel);
        return;
    }
    kofty_gui_detail::g_gui->add(w);
}

// --- LAYOUTS --------------------------------------------------------
// orientation: 0 = Vertical, 1 = Horizontal
inline int gui_add_boxlayout(int orientation, int spacing, int parent_id) {
    // FORCE spacing to 0 for tight Kivy look
    spacing = 0; 

    tgui::Container::Ptr layout;
    if (orientation == 0) {
        auto v = tgui::VerticalLayout::create();
        v->getRenderer()->setSpaceBetweenWidgets(spacing);
        v->getRenderer()->setPadding(0); 
        layout = v;
    } else {
        auto h = tgui::HorizontalLayout::create();
        h->getRenderer()->setSpaceBetweenWidgets(spacing);
        h->getRenderer()->setPadding(0); 
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

// --- ANCHOR LAYOUT --------------------------------------------------
// A Panel whose children are positioned by 3x3 anchor + pixel offset.
// anchor_side: 0=TL, 1=TC, 2=TR, 3=ML, 4=MC, 5=MR, 6=BL, 7=BC, 8=BR
inline int gui_add_anchorlayout(int parent_id) {
    auto panel = tgui::Panel::create();
    panel->getRenderer()->setBackgroundColor(tgui::Color::Transparent);
    int id = kofty_gui_detail::g_next_id++;
    kofty_gui_detail::g_layouts[id] = panel;
    panel->setSize("100%", "100%");
    _kofty_attach_container(id, parent_id);
    return id;
}

inline void gui_anchor_add(int anchor_id, int widget_id,
                           int anchor_side, int offset_x, int offset_y,
                           int width, int height) {
    if (!kofty_gui_detail::g_layouts.count(anchor_id)) return;
    if (!kofty_gui_detail::g_widgets.count(widget_id)) return;

    auto panel = kofty_gui_detail::g_layouts[anchor_id];
    auto widget = kofty_gui_detail::g_widgets[widget_id];

    if (widget->getParent() != nullptr) {
        widget->getParent()->remove(widget);
    }
    panel->add(widget);

    kofty_gui_detail::g_anchors.push_back({
        anchor_id, widget_id, anchor_side, offset_x, offset_y, width, height
    });

    _kofty_apply_anchors();
}

// --- WIDGETS --------------------------------------------------------
// row/col only used when parent is a grid
inline int gui_add_button(const std::string& text, int parent_id) {
    auto btn = tgui::Button::create(text);
    btn->setSize(100, 100); 
    
    // Make it look like Kivy's default grey button
    btn->getRenderer()->setBackgroundColor(tgui::Color(80, 80, 80)); 
    btn->getRenderer()->setBackgroundColorHover(tgui::Color(100, 100, 100));
    btn->getRenderer()->setBackgroundColorDown(tgui::Color(60, 60, 60));
    btn->getRenderer()->setTextColor(tgui::Color(255, 255, 255)); 
    //btn->getRenderer()->setPadding(0); // <--- REMOVE BUTTON MARGINS

    int id = kofty_gui_detail::g_next_id++;
    kofty_gui_detail::g_widgets[id] = btn;
    _kofty_attach_widget(id, parent_id, 0, 0);
    
    float window_height = (float)GetScreenHeight();
    btn->setPosition(0.f, window_height - 100.f); 

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
    lbl->getRenderer()->setTextColor(tgui::Color(255, 255, 255)); // Default white text!
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
    auto w = kofty_gui_detail::g_widgets[widget_id];
    
    // Convert Kivy bottom-left (y=0 is bottom) to TGUI top-left (y=0 is top)
    float window_height = (float)GetScreenHeight();
    float widget_height = w->getSize().y;
    float tgui_y = window_height - y - widget_height;
    
    w->setPosition((float)x, tgui_y);
}

// button color
// Foreground (Text) Colors
inline void gui_set_button_text_color(int widget_id, int r, int g, int b, int a = 255) {
    if (!kofty_gui_detail::g_widgets.count(widget_id)) return;
    auto w = kofty_gui_detail::g_widgets[widget_id];
    if (auto btn = std::dynamic_pointer_cast<tgui::Button>(w)) {
        btn->getRenderer()->setTextColor(tgui::Color(r, g, b, a));
    }
}

inline void gui_set_label_color(int widget_id, int r, int g, int b, int a = 255) {
    if (!kofty_gui_detail::g_widgets.count(widget_id)) return;
    auto w = kofty_gui_detail::g_widgets[widget_id];
    if (auto lbl = std::dynamic_pointer_cast<tgui::Label>(w)) {
        lbl->getRenderer()->setTextColor(tgui::Color(r, g, b, a));
    }
}

// Background Colors
inline void gui_set_button_bg_color(int widget_id, int r, int g, int b, int a = 255) {
        printf("Setting button %d color to %d, %d, %d\n", widget_id, r, g, b);
    fflush(stdout);
    if (!kofty_gui_detail::g_widgets.count(widget_id)) return;
    auto w = kofty_gui_detail::g_widgets[widget_id];
    if (auto btn = std::dynamic_pointer_cast<tgui::Button>(w)) {
        btn->getRenderer()->setBackgroundColor(tgui::Color(r, g, b, a));
        btn->getRenderer()->setBackgroundColorHover(
            tgui::Color(r > 20 ? r - 20 : 0, g > 20 ? g - 20 : 0, b > 20 ? b - 20 : 0, a));
    }
}

inline void gui_set_label_bg_color(int widget_id, int r, int g, int b, int a = 255) {
    if (!kofty_gui_detail::g_widgets.count(widget_id)) return;
    auto w = kofty_gui_detail::g_widgets[widget_id];
    if (auto lbl = std::dynamic_pointer_cast<tgui::Label>(w)) {
        lbl->getRenderer()->setBackgroundColor(tgui::Color(r, g, b, a));
    }
}

// --- EVENT POLLING --------------------------------------------------
inline int gui_check_click() {
    int clicked = kofty_gui_detail::g_last_clicked_id;
    kofty_gui_detail::g_last_clicked_id = -1;
    return clicked;
}

// --- TEXT INPUT (EditBox) ---
inline int gui_add_textinput(const std::string& text, int parent_id) {
    auto edit = tgui::EditBox::create();
    edit->setText(text);
    edit->setSize(200, 40);
    int id = kofty_gui_detail::g_next_id++;
    kofty_gui_detail::g_widgets[id] = edit;
    _kofty_attach_widget(id, parent_id, 0, 0);
    return id;
}

inline std::string gui_get_text(int widget_id) {
    if (!kofty_gui_detail::g_widgets.count(widget_id)) return "";
    auto w = kofty_gui_detail::g_widgets[widget_id];
    if (auto e = std::dynamic_pointer_cast<tgui::EditBox>(w)) return e->getText().toStdString();
    if (auto b = std::dynamic_pointer_cast<tgui::Button>(w)) return b->getText().toStdString();
    if (auto l = std::dynamic_pointer_cast<tgui::Label>(w)) return l->getText().toStdString();
    return "";
}

inline void gui_set_textinput_text_color(int widget_id, int r, int g, int b, int a = 255) {
    if (!kofty_gui_detail::g_widgets.count(widget_id)) return;
    auto w = kofty_gui_detail::g_widgets[widget_id];
    if (auto e = std::dynamic_pointer_cast<tgui::EditBox>(w)) {
        e->getRenderer()->setTextColor(tgui::Color(r, g, b, a));
    }
}

inline void gui_set_textinput_bg_color(int widget_id, int r, int g, int b, int a = 255) {
    if (!kofty_gui_detail::g_widgets.count(widget_id)) return;
    auto w = kofty_gui_detail::g_widgets[widget_id];
    if (auto e = std::dynamic_pointer_cast<tgui::EditBox>(w)) {
        e->getRenderer()->setBackgroundColor(tgui::Color(r, g, b, a));
    }
}

// --- KIVY SIZE & POS HINTS ---
inline void gui_set_size_hint(int widget_id, float w_pct, float h_pct) {
    if (!kofty_gui_detail::g_widgets.count(widget_id)) return;
    auto w = kofty_gui_detail::g_widgets[widget_id];
    
    // We store the hint so we can recalculate it if the window resizes later
    static std::map<int, std::pair<float, float>> size_hints;
    size_hints[widget_id] = {w_pct, h_pct};

    float win_w = (float)GetScreenWidth();
    float win_h = (float)GetScreenHeight();
    
    // Calculate actual pixel sizes
    float px_w = (w_pct >= 0.0f) ? win_w * w_pct : w->getSize().x;
    float px_h = (h_pct >= 0.0f) ? win_h * h_pct : w->getSize().y;
    
    w->setSize(px_w, px_h);
}

inline void gui_set_pos_hint(int widget_id, const std::string& hint_key, float pct) {
    if (!kofty_gui_detail::g_widgets.count(widget_id)) return;
    auto w = kofty_gui_detail::g_widgets[widget_id];
    
    float win_w = (float)GetScreenWidth();
    float win_h = (float)GetScreenHeight();
    float widget_w = w->getSize().x;
    float widget_h = w->getSize().y;
    
    float x = w->getPosition().x;
    float y = w->getPosition().y;
    
    // In Kivy, Y=0 is bottom. In TGUI, Y=0 is top.
    if (hint_key == "x")          x = win_w * pct;
    else if (hint_key == "center_x") x = (win_w * pct) - (widget_w / 2);
    else if (hint_key == "right")    x = (win_w * pct) - widget_w;
    
    else if (hint_key == "y")          y = win_h - (win_h * pct) - widget_h;
    else if (hint_key == "center_y")   y = win_h - (win_h * pct) - (widget_h / 2);
    else if (hint_key == "top")        y = win_h - (win_h * pct);
    
    w->setPosition(x, y);
}

#endif