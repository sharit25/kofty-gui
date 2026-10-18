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


def register():
    """Batch 40.3: entry point for kofty's `kofty.modules` plugin discovery.

    Kofty calls this function once per compilation to ask for g++ flags.
    Returns a dict with optional include_dirs / lib_dirs / libs lists.
    """
    return {
        "name": "kofty_gui",
        "header": str(header_path()),
        "include_dirs": include_dirs(),
        "lib_dirs": lib_dirs(),
        "libs": libs(),
    }