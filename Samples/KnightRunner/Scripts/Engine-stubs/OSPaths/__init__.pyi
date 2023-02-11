"""Module for querying OS related paths for Sharpheus"""
from __future__ import annotations
import Engine.OSPaths
import typing

__all__ = [
    "Folder",
    "get"
]


class Folder():
    """
    Members:

      APPDATA_ROAMING

      EXEC_FOLDER
    """
    def __eq__(self, other: object) -> bool: ...
    def __getstate__(self) -> int: ...
    def __hash__(self) -> int: ...
    def __index__(self) -> int: ...
    def __init__(self, value: int) -> None: ...
    def __int__(self) -> int: ...
    def __ne__(self, other: object) -> bool: ...
    def __repr__(self) -> str: ...
    def __setstate__(self, state: int) -> None: ...
    @property
    def name(self) -> str:
        """
        :type: str
        """
    @property
    def value(self) -> int:
        """
        :type: int
        """
    APPDATA_ROAMING: Engine.OSPaths.Folder # value = <Folder.APPDATA_ROAMING: 0>
    EXEC_FOLDER: Engine.OSPaths.Folder # value = <Folder.EXEC_FOLDER: 1>
    __members__: dict # value = {'APPDATA_ROAMING': <Folder.APPDATA_ROAMING: 0>, 'EXEC_FOLDER': <Folder.EXEC_FOLDER: 1>}
    pass
def get(folder: Folder) -> str:
    pass
