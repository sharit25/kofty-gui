# kofty-gui

GUI toolkit for [Kofty](https://github.com/sharit25/kofty) — retained-mode
widgets backed by TGUI + raylib.

**Status:** alpha. Requires `kofty >= 0.5.0` (user-module import support).

## Install

    pip install kofty kofty-gui

You also need TGUI (with the raylib backend) and raylib itself on your system.
See [BUILDING_TGUI.md](BUILDING_TGUI.md) for one-time setup instructions.

## Quick start

`hello_gui.kfy`:

    import kofty_gui as gui

    panel = gui.Panel("Hello", 600, 400)
    vbox = gui.VBox(panel)

    title = gui.Label(vbox, "Hello, Kofty GUI!")
    title.set_font_size(24)

    name_input = gui.TextInput(vbox, "Enter your name...")
    greeting = gui.Label(vbox, "")

    def on_greet():
        name = name_input.get_text()
        if name != "":
            greeting.set_text("Hello, " + name + "!")
        else:
            greeting.set_text("Hello, stranger!")

    greet_btn = gui.Button(vbox, "Greet")
    greet_btn.on_click(on_greet)

    gui.run()

Run:

    kofty hello_gui.kfy

## Two-file pattern (Kivy-style split)

For larger apps, split logic and widgets into two `.kfy` files:

**`main.kfy`** — behavior:

    import kofty_gui as gui
    from calculator_ui import CalculatorUI

    class Calculator:
        def __init__(self):
            self.ui = CalculatorUI()
            self.ui.on_add(self.handle_add)
            self.ui.on_clear(self.handle_clear)

        def handle_add(self):
            a = float(self.ui.input_a.get_text())
            b = float(self.ui.input_b.get_text())
            self.ui.result.set_text(str(a + b))

        def handle_clear(self):
            self.ui.input_a.set_text("")
            self.ui.input_b.set_text("")
            self.ui.result.set_text("")

        def run(self):
            self.ui.show()
            gui.run()

    app = Calculator()
    app.run()

**`calculator_ui.kfy`** — widgets only:

    import kofty_gui as gui

    class CalculatorUI:
        def __init__(self):
            self.panel = gui.Panel("Calculator", 400, 300)
            self.vbox = gui.VBox(self.panel)

            self.input_a = gui.TextInput(self.vbox, "First number")
            self.input_b = gui.TextInput(self.vbox, "Second number")
            self.result = gui.Label(self.vbox, "")

            self.add_btn = gui.Button(self.vbox, "Add")
            self.clear_btn = gui.Button(self.vbox, "Clear")

        def on_add(self, cb):
            self.add_btn.on_click(cb)

        def on_clear(self, cb):
            self.clear_btn.on_click(cb)

        def show(self):
            self.panel.show()

This mirrors Kivy's `main.py` + `app.kv` split, without needing a separate
`.kv`-style file format.

## What works (v0.1)

- Panel, VBox, Label, Button, TextInput
- `on_click(callback)` on buttons
- `get_text()` / `set_text()` on text inputs
- `set_text()` / `set_font_size()` on labels
- Retained-mode: widgets persist between frames

## What's not there yet

- Multiple windows
- Layout containers beyond VBox (HBox, Grid, Scroll)
- Real-time validation on text inputs
- Methods as callbacks (`button.on_click(self.method)` requires a free function)
- Kivy-style `.kui` declarative files

## Requirements

- `kofty >= 0.5.0`
- Python 3.10+
- g++ with C++20 support
- TGUI with raylib backend (see [BUILDING_TGUI.md](BUILDING_TGUI.md))
- raylib

## License

MIT — see [LICENSE](LICENSE).