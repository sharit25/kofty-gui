"""
kofty-gui — GUI toolkit for Kofty.

Provides `kofty_gui.hpp` (TGUI + raylib retained-mode wrapper)
and install paths so `kofty` can find them via `import kofty_gui`.

Usage:
    import kofty_gui
    print(kofty_gui.header_path())      # absolute path to kofty_gui.hpp
    print(kofty_gui.include_dirs())     # for g++ -I
    print(kofty_gui.libs())             # for g++ -l
"""
from pathlib import Path

def package_dir():
    return Path(__file__).resolve().parent

def header_path():
    return package_dir() / "kofty_gui.hpp"

def include_dirs():
    """Return a list of -I include directories."""
    return [
        str(package_dir()),
        # Plus TGUI/raylib include paths once resolved
    ]

def libs():
    """Return a list of -l libraries."""
    return ["TGUI", "raylib", "opengl32", "gdi32", "winmm"]

def lib_dirs():
    """Return a list of -L library directories."""
    return [
        # Resolved TGUI/raylib paths
    ]