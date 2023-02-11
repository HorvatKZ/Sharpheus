"""Renderer module for Sharpheus"""
from __future__ import annotations
import Engine.Renderer
import typing
import Engine

__all__ = [
    "draw_circle",
    "draw_circle_part",
    "draw_horizontal_line",
    "draw_line",
    "draw_monocrome_quad",
    "draw_quad",
    "draw_vertical_line",
    "get_API_version",
    "get_background_color",
    "get_camera",
    "get_full_tex_coords",
    "get_invalid_texture",
    "is_inited",
    "is_valid_texture"
]


def draw_circle(coords: typing.List[Engine.Point], tint: Engine.Color) -> None:
    pass
def draw_circle_part(coords: typing.List[Engine.Point], tex_coords: typing.List[Engine.Point], tint: Engine.Color) -> None:
    pass
def draw_horizontal_line(begin: float, end: float, place: float, thickness: float, color: Engine.Color) -> None:
    pass
def draw_line(begin: Engine.Point, end: Engine.Point, thickness: float, color: Engine.Color) -> None:
    pass
def draw_monocrome_quad(coords: typing.List[Engine.Point], color: Engine.Color) -> None:
    pass
def draw_quad(coords: typing.List[Engine.Point], tex_coords: typing.List[Engine.Point], tint: Engine.Color, tex_id: int) -> None:
    pass
def draw_vertical_line(begin: float, end: float, place: float, thickness: float, color: Engine.Color) -> None:
    pass
def get_API_version() -> str:
    pass
def get_background_color() -> Engine.Color:
    pass
def get_camera() -> Engine.Camera:
    pass
def get_full_tex_coords(mirror_x: bool = False) -> typing.List[Engine.Point]:
    pass
def get_invalid_texture() -> int:
    pass
def is_inited() -> bool:
    pass
def is_valid_texture(tex_id: int) -> bool:
    pass
