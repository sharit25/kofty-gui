import re
import pathlib

def parse_gui_source(source: str, source_path: pathlib.Path) -> str:
    """
    Intercepts the Kofty source code and prepares it for the C++ emitter.
    1. Reads the UI file (gui_load_ui).
    2. Inlines `widgets.py` classes.
    3. Replaces `super().__init__()` with `pass` for C++ compatibility.
    """
    # 1. Read the UI file
    ui_matches = re.findall(r'gui_load_ui\s*\(\s*["\']([^"\']+)["\']\s*\)', source)
    for ui_file in ui_matches:
        ui_path = source_path.parent / ui_file
        if ui_path.exists():
            from kofty.kv_parser import load_ui_file
            ui_code = load_ui_file(str(ui_path))
            source = source.replace(f'gui_load_ui("{ui_file}")', ui_code)
            source = source.replace(f"gui_load_ui('{ui_file}')", ui_code)
            
    # 2. Inline `widgets.py` cleanly
    lines = source.split("\n")
    new_lines = []
    hoisted_imports = []
    
    for line in lines:
        m = re.match(r'^from\s+([\w\.]+)\s+import\s+(.*)$', line)
        if m:
            mod = m.group(1)
            if mod in ("kofty_gui", "raylib", "math", "time", "random"):
                hoisted_imports.append(line)
                continue
            
            # Look for widgets.py / widgets.kfy
            found = False
            for p in [source_path.parent / f"{mod}.py", source_path.parent / f"{mod}.kfy"]:
                if p.exists():
                    content = p.read_text(encoding="utf-8-sig")
                    new_lines.append(f"# INLINED FROM {mod}")
                    
                    # Hoist any kofty_gui imports from widgets.py
                    for w_line in content.split("\n"):
                        if w_line.startswith("from kofty_gui import"):
                            if w_line not in hoisted_imports:
                                hoisted_imports.append(w_line)
                        else:
                            new_lines.append(w_line)
                    new_lines.append("# END INLINED")
                    found = True
                    break
            
            if not found:
                new_lines.append(line)
        else:
            new_lines.append(line)
            
    # 3. Combine: Hoisted Imports + Inlined Classes + Main App
    final_source = "\n".join(hoisted_imports) + "\n" + "\n".join(new_lines)
    
    # 4. Replace `super().__init__()` with `pass` to avoid C++ errors,
    #    but keep the indentation intact.
    final_source = final_source.replace("        super().__init__()\n", "        pass\n")
    final_source = final_source.replace("    super().__init__()\n", "    pass\n")
    
    return final_source