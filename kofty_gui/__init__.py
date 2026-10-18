from pathlib import Path

def package_dir():
    return Path(__file__).resolve().parent

def header_path():
    return package_dir() / "kofty_gui.hpp"

def include_dirs():
    return [str(package_dir())]

def libs():
    # Stub build: no external libraries needed.
    # When TGUI + raylib are installed, change to:
    #   return ["TGUI", "raylib", "opengl32", "gdi32", "winmm"]
    return []

def lib_dirs():
    return []