from kofty_gui import gui_add_button, gui_add_label, gui_add_textinput
from kofty_gui import gui_set_size, gui_set_pos, gui_get_text
#from kofty_gui import gui_set_button_text_color, gui_set_button_bg_color
#from kofty_gui import gui_set_label_color, gui_set_label_bg_color
#from kofty_gui import gui_set_textinput_text_color, gui_set_textinput_bg_color

from kofty_gui import widgets
from kofty_gui import widgets
from widgets import Label, TextInput, Button

class Widgets:
    widget_id: int
    def __init__(self):
        self.widget_id = -1

class Label:
    def __init__(self, text, parent):
        super().__init__()   # <--- MUST BE HERE
        self.widget_id = gui_add_label(text, parent)
    def set_size(self, w, h):
        gui_set_size(self.widget_id, w, h)
    def set_pos(self, x, y):
        gui_set_pos(self.widget_id, x, y)
    #def set_color(self, r, g, b, a):
        #gui_set_label_color(self.widget_id, r, g, b, a)

class TextInput:
    def __init__(self, text, parent):
        super().__init__()   # <--- MUST BE HERE
        self.widget_id = gui_add_textinput(text, parent)
    def set_size(self, w, h):
        gui_set_size(self.widget_id, w, h)
    def set_pos(self, x, y):
        gui_set_pos(self.widget_id, x, y)
    def get_text(self):
        return gui_get_text(self.widget_id)

class Button:
    def __init__(self, text, parent):
        super().__init__()   # <--- MUST BE HERE
        self.widget_id = gui_add_button(text, parent)
    def set_size(self, w, h):
        gui_set_size(self.widget_id, w, h)
    def set_pos(self, x, y):
        gui_set_pos(self.widget_id, x, y)
    #def set_bg_color(self, r, g, b, a):
        #gui_set_button_bg_color(self.widget_id, r, g, b, a)