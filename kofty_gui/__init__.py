from pathlib import Path

def package_dir():
    return Path(__file__).resolve().parent

def header_path():
    return package_dir() / "kofty_gui.hpp"

_TGUI_ROOT = Path(r"C:\TGUI")
_TGUI_INCLUDE = _TGUI_ROOT / "include"
_TGUI_LIB = _TGUI_ROOT / "build" / "lib"

_UCRT64 = Path(r"C:\msys64\ucrt64")
_UCRT64_INCLUDE = _UCRT64 / "include"
_UCRT64_LIB = _UCRT64 / "lib"

def include_dirs():
    return [
        str(package_dir()),
        str(_TGUI_INCLUDE),
        str(_UCRT64_INCLUDE),
    ]

def lib_dirs():
    return [
        str(_TGUI_LIB),
        str(_UCRT64_LIB),
    ]

def libs():
    return ["tgui", "raylib", "opengl32", "gdi32", "winmm"]

def register():
    return {
        "name": "kofty_gui",
        "header": str(header_path()),
        "include_dirs": include_dirs(),
        "lib_dirs": lib_dirs(),
        "libs": libs(),
    }
