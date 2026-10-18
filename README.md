# Kofty GUI Toolkit

`kofty-gui` is a Python package that provides a native C++ GUI toolkit for the **Kofty** programming language. It wraps the powerful [TGUI](https://tgui.eu/) library (running on the raylib backend) behind simple, Python-like functions.

## Installation

If you are developing locally, run:
```bash
cd kofty-gui
python -m pip install -e .

## §72: Interactive GUI (v0.7.0)

**Status:** Achieved.
- Expanded `kofty_gui.hpp` to support interactive widgets: Labels, EditBoxes (text input), Sliders, and Checkboxes.
- Introduced the **Polling Loop** architecture:
  - `while gui_running():`
  - `clicked = gui_check_click()`
  - `gui_update()`
- This allows Kofty programs to use standard `if` statements to react to UI events without needing complex C++ callbacks.
- Dynamic UI updates are handled by returning an integer ID when adding a Label/Button, and using `gui_set_label(id, text)` to update it.

**Example:**
```python
score_label = gui_add_label("Score: 0", 100, 100)
if gui_check_click() == 0:
    score += 1
    gui_set_label(score_label, f"Score: {score}")