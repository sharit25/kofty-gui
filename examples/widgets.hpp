#pragma once
#include "kofty_gui.hpp"
#include <string>

// Base class
struct Widget {
    int widget_id = -1;
};

struct Label : public Widget {
    Label(std::string text, int parent) {
        widget_id = gui_add_label(text, parent);
    }
    void set_size(int w, int h) { gui_set_size(widget_id, w, h); }
    void set_pos(int x, int y) { gui_set_pos(widget_id, x, y); }
    void set_color(int r, int g, int b, int a) { gui_set_label_color(widget_id, r, g, b, a); }
    void set_size_hint(float w, float h) { gui_set_size_hint(widget_id, w, h); }
    void set_pos_hint(std::string key, float pct) { gui_set_pos_hint(widget_id, key, pct); }
};

struct TextInput : public Widget {
    TextInput(std::string text, int parent) {
        widget_id = gui_add_textinput(text, parent);
    }
    void set_size(int w, int h) { gui_set_size(widget_id, w, h); }
    void set_pos(int x, int y) { gui_set_pos(widget_id, x, y); }
    std::string get_text() { return gui_get_text(widget_id); }
    void set_size_hint(float w, float h) { gui_set_size_hint(widget_id, w, h); }
    void set_pos_hint(std::string key, float pct) { gui_set_pos_hint(widget_id, key, pct); }
};

struct Button : public Widget {
    Button(std::string text, int parent) {
        widget_id = gui_add_button(text, parent);
    }
    void set_size(int w, int h) { gui_set_size(widget_id, w, h); }
    void set_pos(int x, int y) { gui_set_pos(widget_id, x, y); }
    void set_bg_color(int r, int g, int b, int a) { gui_set_button_bg_color(widget_id, r, g, b, a); }
    void set_size_hint(float w, float h) { gui_set_size_hint(widget_id, w, h); }
    void set_pos_hint(std::string key, float pct) { gui_set_pos_hint(widget_id, key, pct); }
};

// Layouts (no inheritance from Widget, just hold a layout_id)
struct BoxLayout {
    int layout_id = -1;
    BoxLayout() = default;
    BoxLayout(int orientation, int spacing, int parent) {
        layout_id = gui_add_boxlayout(orientation, spacing, parent);
    }
};

struct GridLayout {
    int layout_id = -1;
    GridLayout() = default;
    GridLayout(int cols, int spacing, int parent) {
        layout_id = gui_add_gridlayout(cols, spacing, parent);
    }
};

struct AnchorLayout {
    int layout_id = -1;
    AnchorLayout() = default;
    explicit AnchorLayout(int parent) {
        layout_id = gui_add_anchorlayout(parent);
    }
};