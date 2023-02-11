"""Audio player module for Sharpheus"""
from __future__ import annotations
import Engine.AudioPlayer
import typing
import Engine

__all__ = [
    "Handle",
    "is_paused",
    "pause",
    "play",
    "resume",
    "set_play_speed",
    "set_volume",
    "stop",
    "stop_all"
]


class Handle():
    def __repr__(self) -> str: ...
    pass
def is_paused(handle: int) -> bool:
    pass
def pause(handle: int) -> None:
    pass
def play(audio: Engine.Audio, looping: bool = False, volume: int = 100, play_speed: float = 1.0) -> int:
    pass
def resume(handle: int) -> None:
    pass
def set_play_speed(handle: int, speed: float) -> None:
    pass
def set_volume(handle: int, volume: int) -> None:
    pass
def stop(handle: int) -> None:
    pass
def stop_all() -> None:
    pass
