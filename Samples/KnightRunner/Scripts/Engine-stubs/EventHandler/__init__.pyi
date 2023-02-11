"""Event handler module for Sharpheus"""
from __future__ import annotations
import Engine.EventHandler
import typing
import Engine

__all__ = [
    "handle",
    "subscribe_key_hold",
    "subscribe_key_pressed",
    "subscribe_key_released",
    "subscribe_mouse_moved",
    "subscribe_mouse_pressed",
    "subscribe_mouse_released",
    "subscribe_mouse_scrolled",
    "subscribe_window_closed",
    "subscribe_window_resized",
    "unsubscribe_all",
    "unsubscribe_key_hold",
    "unsubscribe_key_pressed",
    "unsubscribe_key_released",
    "unsubscribe_mouse_moved",
    "unsubscribe_mouse_pressed",
    "unsubscribe_mouse_released",
    "unsubscribe_mouse_scrolled",
    "unsubscribe_window_resized"
]


def handle(event: Engine.Event) -> None:
    pass
def subscribe_key_hold(listener_id: int, func: typing.Callable[[Engine.KeyHoldEvent], None]) -> None:
    pass
def subscribe_key_pressed(listener_id: int, func: typing.Callable[[Engine.KeyPressedEvent], None]) -> None:
    pass
def subscribe_key_released(listener_id: int, func: typing.Callable[[Engine.KeyReleasedEvent], None]) -> None:
    pass
def subscribe_mouse_moved(listener_id: int, func: typing.Callable[[Engine.MouseMovedEvent], None]) -> None:
    pass
def subscribe_mouse_pressed(listener_id: int, func: typing.Callable[[Engine.MousePressedEvent], None]) -> None:
    pass
def subscribe_mouse_released(listener_id: int, func: typing.Callable[[Engine.MouseReleasedEvent], None]) -> None:
    pass
def subscribe_mouse_scrolled(listener_id: int, func: typing.Callable[[Engine.MouseScrolledEvent], None]) -> None:
    pass
@typing.overload
def subscribe_window_closed(listener_id: int) -> None:
    pass
@typing.overload
def subscribe_window_closed(listener_id: int, func: typing.Callable[[Engine.WindowClosedEvent], None]) -> None:
    pass
def subscribe_window_resized(listener_id: int, func: typing.Callable[[Engine.WindowResizedEvent], None]) -> None:
    pass
def unsubscribe_all(listener_id: int) -> None:
    pass
def unsubscribe_key_hold(listener_id: int) -> None:
    pass
def unsubscribe_key_pressed(listener_id: int) -> None:
    pass
def unsubscribe_key_released(listener_id: int) -> None:
    pass
def unsubscribe_mouse_moved(listener_id: int) -> None:
    pass
def unsubscribe_mouse_pressed(listener_id: int) -> None:
    pass
def unsubscribe_mouse_released(listener_id: int) -> None:
    pass
def unsubscribe_mouse_scrolled(listener_id: int) -> None:
    pass
def unsubscribe_window_resized(listener_id: int) -> None:
    pass
