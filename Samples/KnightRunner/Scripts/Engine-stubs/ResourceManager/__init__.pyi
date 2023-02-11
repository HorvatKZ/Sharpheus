"""Module for managing assets and resources for Sharpheus"""
from __future__ import annotations
import Engine.ResourceManager
import typing
import Engine

__all__ = [
    "fullpath2path",
    "get_animation",
    "get_assets_root",
    "get_audio",
    "get_font",
    "get_image",
    "get_script_path",
    "get_scripts_root",
    "get_shader_path",
    "get_tileset",
    "path2fullpath"
]


def fullpath2path(fullpath: str) -> str:
    pass
def get_animation(path: str) -> Engine.Animation:
    pass
def get_assets_root() -> str:
    pass
def get_audio(path: str) -> Engine.Audio:
    pass
@typing.overload
def get_font(fontfile: str, imgfile: str) -> Engine.Font:
    pass
@typing.overload
def get_font(name: str) -> Engine.Font:
    pass
def get_image(path: str, filtered: bool = False) -> Engine.Image:
    pass
def get_script_path(fname: str) -> str:
    pass
def get_scripts_root() -> str:
    pass
def get_shader_path(fname: str) -> str:
    pass
def get_tileset(path: str) -> Engine.TileSet:
    pass
def path2fullpath(path: str) -> str:
    pass
