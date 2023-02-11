"""Sharpheus Game Engine Python Interface [using pybind11]"""
from __future__ import annotations
import Engine
import typing

__all__ = [
    "AnimatedSprite",
    "Animation",
    "AnimationPlayer",
    "Audio",
    "AudioPlayer",
    "AxisGameObject",
    "Behavior",
    "BoxCollider",
    "Button",
    "ButtonBase",
    "Camera",
    "Capsule",
    "CapsuleCollider",
    "CheckBase",
    "CheckBox",
    "Circle",
    "CircleCollider",
    "CollData",
    "Collection",
    "Collider",
    "CollisionEvent",
    "Color",
    "Control",
    "ControlChangedEvent",
    "EngineVersion",
    "Event",
    "EventHandler",
    "EventListener",
    "FileLoader",
    "FileSaver",
    "Font",
    "GameObject",
    "Image",
    "ImageButton",
    "KeyCode",
    "KeyEvent",
    "KeyHoldEvent",
    "KeyPressedEvent",
    "KeyReleasedEvent",
    "Level",
    "Logger",
    "MouseButton",
    "MouseEvent",
    "MouseMovedEvent",
    "MousePressedEvent",
    "MouseReleasedEvent",
    "MouseScrolledEvent",
    "MusicPlayer",
    "OSPaths",
    "OnEnterEvent",
    "OnExitEvent",
    "Oval",
    "ParticleEmitter",
    "PhysicsObject",
    "Point",
    "ProjectControl",
    "PythonBehavior",
    "PythonRunnerBehavior",
    "Quad",
    "RadioButton",
    "Rect",
    "RenderableGameObject",
    "Renderer",
    "Resource",
    "ResourceManager",
    "Shape",
    "ShapedGameObject",
    "SoundEffects",
    "Sprite",
    "Text",
    "TileMap",
    "TileSet",
    "Transform",
    "WindowClosedEvent",
    "WindowResizedEvent"
]


class EventListener():
    def subscribe_key_hold(self, func: typing.Callable[[KeyHoldEvent], None]) -> None: ...
    def subscribe_key_pressed(self, func: typing.Callable[[KeyPressedEvent], None]) -> None: ...
    def subscribe_key_released(self, func: typing.Callable[[KeyReleasedEvent], None]) -> None: ...
    def subscribe_mouse_moved(self, func: typing.Callable[[MouseMovedEvent], None]) -> None: ...
    def subscribe_mouse_pressed(self, func: typing.Callable[[MousePressedEvent], None]) -> None: ...
    def subscribe_mouse_released(self, func: typing.Callable[[MouseReleasedEvent], None]) -> None: ...
    def subscribe_mouse_scrolled(self, func: typing.Callable[[MouseScrolledEvent], None]) -> None: ...
    @typing.overload
    def subscribe_window_closed(self) -> None: ...
    @typing.overload
    def subscribe_window_closed(self, func: typing.Callable[[WindowClosedEvent], None]) -> None: ...
    def subscribe_window_resized(self, func: typing.Callable[[WindowResizedEvent], None]) -> None: ...
    def unsubscribe_all(self) -> None: ...
    def unsubscribe_key_hold(self) -> None: ...
    def unsubscribe_key_pressed(self) -> None: ...
    def unsubscribe_key_released(self) -> None: ...
    def unsubscribe_mouse_moved(self) -> None: ...
    def unsubscribe_mouse_pressed(self) -> None: ...
    def unsubscribe_mouse_released(self) -> None: ...
    def unsubscribe_mouse_scrolled(self) -> None: ...
    def unsubscribe_window_resized(self) -> None: ...
    @property
    def id(self) -> int:
        """
        :type: int
        """
    pass
class Resource():
    @property
    def full_path(self) -> str:
        """
        :type: str
        """
    @property
    def has_path(self) -> bool:
        """
        :type: bool
        """
    @property
    def is_valid(self) -> bool:
        """
        :type: bool
        """
    @property
    def path(self) -> str:
        """
        :type: str
        """
    pass
class GameObject(EventListener):
    class Type():
        """
        Members:

          None

          Collection

          CppBehavior

          PythonRunnerBehavior

          PythonBehavior

          Camera

          Sprite

          Quad

          Text

          AnimatedSprite

          AnimationPlayer

          TileMap

          ParticleEmitter

          PhysicsObject

          BoxCollider

          CircleCollider

          CapsuleCollider

          MusicPlayer

          SoundEffects

          Button

          ImageButton

          CheckBox

          RadioButton
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
        AnimatedSprite: Engine.GameObject.Type # value = <Type.AnimatedSprite: 35>
        AnimationPlayer: Engine.GameObject.Type # value = <Type.AnimationPlayer: 36>
        BoxCollider: Engine.GameObject.Type # value = <Type.BoxCollider: 64>
        Button: Engine.GameObject.Type # value = <Type.Button: 96>
        Camera: Engine.GameObject.Type # value = <Type.Camera: 16>
        CapsuleCollider: Engine.GameObject.Type # value = <Type.CapsuleCollider: 66>
        CheckBox: Engine.GameObject.Type # value = <Type.CheckBox: 98>
        CircleCollider: Engine.GameObject.Type # value = <Type.CircleCollider: 65>
        Collection: Engine.GameObject.Type # value = <Type.Collection: 1>
        CppBehavior: Engine.GameObject.Type # value = <Type.CppBehavior: 112>
        ImageButton: Engine.GameObject.Type # value = <Type.ImageButton: 97>
        MusicPlayer: Engine.GameObject.Type # value = <Type.MusicPlayer: 80>
        None: Engine.GameObject.Type # value = <Type.None: 0>
        ParticleEmitter: Engine.GameObject.Type # value = <Type.ParticleEmitter: 38>
        PhysicsObject: Engine.GameObject.Type # value = <Type.PhysicsObject: 48>
        PythonBehavior: Engine.GameObject.Type # value = <Type.PythonBehavior: 114>
        PythonRunnerBehavior: Engine.GameObject.Type # value = <Type.PythonRunnerBehavior: 113>
        Quad: Engine.GameObject.Type # value = <Type.Quad: 33>
        RadioButton: Engine.GameObject.Type # value = <Type.RadioButton: 99>
        SoundEffects: Engine.GameObject.Type # value = <Type.SoundEffects: 81>
        Sprite: Engine.GameObject.Type # value = <Type.Sprite: 32>
        Text: Engine.GameObject.Type # value = <Type.Text: 34>
        TileMap: Engine.GameObject.Type # value = <Type.TileMap: 37>
        __members__: dict # value = {'None': <Type.None: 0>, 'Collection': <Type.Collection: 1>, 'CppBehavior': <Type.CppBehavior: 112>, 'PythonRunnerBehavior': <Type.PythonRunnerBehavior: 113>, 'PythonBehavior': <Type.PythonBehavior: 114>, 'Camera': <Type.Camera: 16>, 'Sprite': <Type.Sprite: 32>, 'Quad': <Type.Quad: 33>, 'Text': <Type.Text: 34>, 'AnimatedSprite': <Type.AnimatedSprite: 35>, 'AnimationPlayer': <Type.AnimationPlayer: 36>, 'TileMap': <Type.TileMap: 37>, 'ParticleEmitter': <Type.ParticleEmitter: 38>, 'PhysicsObject': <Type.PhysicsObject: 48>, 'BoxCollider': <Type.BoxCollider: 64>, 'CircleCollider': <Type.CircleCollider: 65>, 'CapsuleCollider': <Type.CapsuleCollider: 66>, 'MusicPlayer': <Type.MusicPlayer: 80>, 'SoundEffects': <Type.SoundEffects: 81>, 'Button': <Type.Button: 96>, 'ImageButton': <Type.ImageButton: 97>, 'CheckBox': <Type.CheckBox: 98>, 'RadioButton': <Type.RadioButton: 99>}
        pass
    class TypeMasks():
        """
        Members:

          Basic

          Camera

          Renderables

          Physics

          Collider

          Sounds

          Control
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
        Basic: Engine.GameObject.TypeMasks # value = <TypeMasks.Basic: 0>
        Camera: Engine.GameObject.TypeMasks # value = <TypeMasks.Camera: 16>
        Collider: Engine.GameObject.TypeMasks # value = <TypeMasks.Collider: 64>
        Control: Engine.GameObject.TypeMasks # value = <TypeMasks.Control: 96>
        Physics: Engine.GameObject.TypeMasks # value = <TypeMasks.Physics: 48>
        Renderables: Engine.GameObject.TypeMasks # value = <TypeMasks.Renderables: 32>
        Sounds: Engine.GameObject.TypeMasks # value = <TypeMasks.Sounds: 80>
        __members__: dict # value = {'Basic': <TypeMasks.Basic: 0>, 'Camera': <TypeMasks.Camera: 16>, 'Renderables': <TypeMasks.Renderables: 32>, 'Physics': <TypeMasks.Physics: 48>, 'Collider': <TypeMasks.Collider: 64>, 'Sounds': <TypeMasks.Sounds: 80>, 'Control': <TypeMasks.Control: 96>}
        pass
    def __eq__(self, arg0: GameObject) -> bool: ...
    def __ne__(self, arg0: GameObject) -> bool: ...
    def copy_from(self, arg0: GameObject) -> None: ...
    @typing.overload
    def get_child(self, ind: int) -> GameObject: ...
    @typing.overload
    def get_child(self, name: str) -> GameObject: ...
    def get_first_child_of_mask(self, mask: GameObject.TypeMasks) -> GameObject: ...
    def get_first_child_of_type(self, type: GameObject.Type) -> GameObject: ...
    def get_last_child(self) -> GameObject: ...
    def get_last_child_of_mask(self, mask: GameObject.TypeMasks) -> GameObject: ...
    def get_last_child_of_type(self, type: GameObject.Type) -> GameObject: ...
    @staticmethod
    def is(*args, **kwargs) -> typing.Any: ...
    def is_all_visible(self) -> bool: ...
    def is_descendant_of(self, other: GameObject) -> bool: ...
    def is_parent_of_current_camera(self) -> bool: ...
    def move(self, new_parent: GameObject) -> bool: ...
    def render_as_selected(self) -> None: ...
    def switch_visibility(self) -> None: ...
    def type(self) -> GameObject.Type: ...
    @property
    def children(self) -> typing.List[GameObject]:
        """
        :type: typing.List[GameObject]
        """
    @property
    def is_visible(self) -> bool:
        """
        :type: bool
        """
    @is_visible.setter
    def is_visible(self, arg1: bool) -> None:
        pass
    @property
    def level(self) -> Engine.Level:
        """
        :type: Sharpheus::Level
        """
    @property
    def name(self) -> str:
        """
        :type: str
        """
    @name.setter
    def name(self, arg1: str) -> None:
        pass
    @property
    def parent(self) -> GameObject:
        """
        :type: GameObject
        """
    @property
    def root(self) -> GameObject:
        """
        :type: GameObject
        """
    @property
    def trafo(self) -> Transform:
        """
        :type: Transform
        """
    @trafo.setter
    def trafo(self, arg1: Transform) -> None:
        pass
    @property
    def world_trafo(self) -> Transform:
        """
        :type: Transform
        """
    @world_trafo.setter
    def world_trafo(self, arg1: Transform) -> None:
        pass
    __hash__ = None
    pass
class Audio(Resource):
    def __repr__(self) -> str: ...
    @property
    def length(self) -> float:
        """
        :type: float
        """
    @property
    def name(self) -> str:
        """
        :type: str
        """
    @property
    def noncopy_cast_address(self) -> int:
        """
        :type: int
        """
    pass
class ShapedGameObject(GameObject, EventListener):
    @property
    def shape(self) -> Shape:
        """
        :type: Shape
        """
    pass
class Behavior(GameObject, EventListener):
    def init(self) -> None: ...
    def sub_type(self) -> int: ...
    pass
class Collider(ShapedGameObject, GameObject, EventListener):
    @property
    def is_collider_visible(self) -> bool:
        """
        :type: bool
        """
    @property
    def is_dynamic(self) -> bool:
        """
        :type: bool
        """
    @property
    def is_trigger(self) -> bool:
        """
        :type: bool
        """
    @is_trigger.setter
    def is_trigger(self, arg1: bool) -> None:
        pass
    pass
class RenderableGameObject(ShapedGameObject, GameObject, EventListener):
    def render(self) -> None: ...
    def render_if_visible(self) -> None: ...
    @property
    def layer(self) -> str:
        """
        :type: str
        """
    @layer.setter
    def layer(self, arg1: str) -> None:
        pass
    pass
class AxisGameObject(RenderableGameObject, ShapedGameObject, GameObject, EventListener):
    @property
    def x_axis(self) -> Point:
        """
        :type: Point
        """
    @property
    def y_axis(self) -> Point:
        """
        :type: Point
        """
    pass
class Camera(ShapedGameObject, GameObject, EventListener):
    def __init__(self, parent: GameObject, name: str) -> None: ...
    def gamepos2screenpos(self, pos: Point) -> Point: ...
    def move_by(self, delta: Point) -> None: ...
    def move_by_screen(self, delta: Point) -> None: ...
    def screenpos2gamepos(self, pos: Point) -> Point: ...
    def zoom(self, scale: float) -> None: ...
    def zoom_to(self, scale: float, keep_in_place: Point) -> None: ...
    def zoom_to_screen(self, scale: float, keep_in_place: Point) -> None: ...
    @property
    def height(self) -> float:
        """
        :type: float
        """
    @property
    def is_current(self) -> bool:
        """
        :type: bool
        """
    @is_current.setter
    def is_current(self, arg1: bool) -> None:
        pass
    @property
    def original_height(self) -> float:
        """
        :type: float
        """
    @property
    def original_width(self) -> float:
        """
        :type: float
        """
    @property
    def shape(self) -> Shape:
        """
        :type: Shape
        """
    @property
    def width(self) -> float:
        """
        :type: float
        """
    @property
    def x_max(self) -> float:
        """
        :type: float
        """
    @property
    def x_min(self) -> float:
        """
        :type: float
        """
    @property
    def y_max(self) -> float:
        """
        :type: float
        """
    @property
    def y_min(self) -> float:
        """
        :type: float
        """
    pass
class Capsule():
    def __init__(self, pos: Point, dim: Point, rot: float) -> None: ...
    def __repr__(self) -> str: ...
    @property
    def dome_height(self) -> float:
        """
        :type: float
        """
    @dome_height.setter
    def dome_height(self, arg1: float) -> None:
        pass
    @property
    def inner_corners(self) -> typing.List[Point]:
        """
        :type: typing.List[Point]
        """
    pass
class CapsuleCollider(Collider, ShapedGameObject, GameObject, EventListener):
    def __init__(self, parent: GameObject, name: str) -> None: ...
    @property
    def height(self) -> float:
        """
        :type: float
        """
    @height.setter
    def height(self, arg1: float) -> None:
        pass
    @property
    def width(self) -> float:
        """
        :type: float
        """
    @width.setter
    def width(self, arg1: float) -> None:
        pass
    pass
class Control(AxisGameObject, RenderableGameObject, ShapedGameObject, GameObject, EventListener):
    @typing.overload
    def set_font_by_name(self, fontfile: str, imgfile: str) -> None: ...
    @typing.overload
    def set_font_by_name(self, name: str) -> None: ...
    def set_font_style(self, **kwargs) -> None: ...
    def subscribe_for_change(self, subscriber_id: int, func: typing.Callable[[ControlChangedEvent], None]) -> None: ...
    def unsubscribe_for_change(self, subscriber_id: int) -> None: ...
    @property
    def font(self) -> Font:
        """
        :type: Font
        """
    @font.setter
    def font(self, arg1: Font) -> None:
        pass
    @property
    def font_color(self) -> Color:
        """
        :type: Color
        """
    @font_color.setter
    def font_color(self, arg1: Color) -> None:
        pass
    @property
    def font_size(self) -> float:
        """
        :type: float
        """
    @font_size.setter
    def font_size(self, arg1: float) -> None:
        pass
    @property
    def font_style(self) -> int:
        """
        :type: int
        """
    @font_style.setter
    def font_style(self, arg1: int) -> None:
        pass
    @property
    def is_clicked(self) -> bool:
        """
        :type: bool
        """
    @property
    def state(self) -> bool:
        """
        :type: bool
        """
    @property
    def text(self) -> str:
        """
        :type: str
        """
    @text.setter
    def text(self, arg1: str) -> None:
        pass
    pass
class CheckBase(Control, AxisGameObject, RenderableGameObject, ShapedGameObject, GameObject, EventListener):
    def set_checked_img_from_path(self, path: str, filtered: bool) -> None: ...
    def set_unchecked_img_from_path(self, path: str, filtered: bool) -> None: ...
    @property
    def checked_img(self) -> Image:
        """
        :type: Image
        """
    @checked_img.setter
    def checked_img(self, arg1: Image) -> None:
        pass
    @property
    def is_checked(self) -> bool:
        """
        :type: bool
        """
    @property
    def is_text_on_right(self) -> bool:
        """
        :type: bool
        """
    @is_text_on_right.setter
    def is_text_on_right(self, arg1: bool) -> None:
        pass
    @property
    def tint(self) -> Color:
        """
        :type: Color
        """
    @tint.setter
    def tint(self, arg1: Color) -> None:
        pass
    @property
    def unchecked_img(self) -> Image:
        """
        :type: Image
        """
    @unchecked_img.setter
    def unchecked_img(self, arg1: Image) -> None:
        pass
    pass
class Circle(RenderableGameObject, ShapedGameObject, GameObject, EventListener):
    def __init__(self, parent: GameObject, name: str) -> None: ...
    @property
    def color(self) -> Color:
        """
        :type: Color
        """
    @color.setter
    def color(self, arg1: Color) -> None:
        pass
    @property
    def radius(self) -> float:
        """
        :type: float
        """
    @radius.setter
    def radius(self, arg1: float) -> None:
        pass
    pass
class CircleCollider(Collider, ShapedGameObject, GameObject, EventListener):
    def __init__(self, parent: GameObject, name: str) -> None: ...
    @property
    def radius(self) -> float:
        """
        :type: float
        """
    @radius.setter
    def radius(self, arg1: float) -> None:
        pass
    pass
class CollData():
    def __init__(self, geom: Shape.Intersection, is_dynamic: bool, v_other: Point, m_other: float) -> None: ...
    @property
    def geom(self) -> Shape.Intersection:
        """
        :type: Shape.Intersection
        """
    @property
    def is(self) -> bool:
        """
        :type: bool
        """
    @property
    def is_dynamic(self) -> bool:
        """
        :type: bool
        """
    @property
    def m_other(self) -> float:
        """
        :type: float
        """
    @property
    def v_other(self) -> Point:
        """
        :type: Point
        """
    pass
class Collection(GameObject, EventListener):
    def __init__(self, parent: GameObject, name: str) -> None: ...
    pass
class BoxCollider(Collider, ShapedGameObject, GameObject, EventListener):
    def __init__(self, parent: GameObject, name: str) -> None: ...
    @property
    def height(self) -> float:
        """
        :type: float
        """
    @height.setter
    def height(self, arg1: float) -> None:
        pass
    @property
    def width(self) -> float:
        """
        :type: float
        """
    @width.setter
    def width(self, arg1: float) -> None:
        pass
    pass
class Event():
    class Type():
        """
        Members:

          None

          Local

          WindowClosed

          WindowResized

          KeyPressed

          KeyHold

          KeyReleased

          MousePressed

          MouseReleased

          MouseMoved

          MouseScrolled
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
        KeyHold: Engine.Event.Type # value = <Type.KeyHold: 5>
        KeyPressed: Engine.Event.Type # value = <Type.KeyPressed: 4>
        KeyReleased: Engine.Event.Type # value = <Type.KeyReleased: 6>
        Local: Engine.Event.Type # value = <Type.Local: 1>
        MouseMoved: Engine.Event.Type # value = <Type.MouseMoved: 9>
        MousePressed: Engine.Event.Type # value = <Type.MousePressed: 7>
        MouseReleased: Engine.Event.Type # value = <Type.MouseReleased: 8>
        MouseScrolled: Engine.Event.Type # value = <Type.MouseScrolled: 10>
        None: Engine.Event.Type # value = <Type.None: 0>
        WindowClosed: Engine.Event.Type # value = <Type.WindowClosed: 2>
        WindowResized: Engine.Event.Type # value = <Type.WindowResized: 3>
        __members__: dict # value = {'None': <Type.None: 0>, 'Local': <Type.Local: 1>, 'WindowClosed': <Type.WindowClosed: 2>, 'WindowResized': <Type.WindowResized: 3>, 'KeyPressed': <Type.KeyPressed: 4>, 'KeyHold': <Type.KeyHold: 5>, 'KeyReleased': <Type.KeyReleased: 6>, 'MousePressed': <Type.MousePressed: 7>, 'MouseReleased': <Type.MouseReleased: 8>, 'MouseMoved': <Type.MouseMoved: 9>, 'MouseScrolled': <Type.MouseScrolled: 10>}
        pass
    def __str__(self) -> str: ...
    @staticmethod
    def static_type() -> Event.Type: ...
    def type(self) -> Event.Type: ...
    pass
class Color():
    @typing.overload
    def __init__(self) -> None: ...
    @typing.overload
    def __init__(self, r: int, g: int, b: int, a: int = 255) -> None: ...
    def __repr__(self) -> str: ...
    def get_alpha(self) -> float: ...
    def get_blue(self) -> float: ...
    def get_green(self) -> float: ...
    def get_red(self) -> float: ...
    def make_opaque(self, percent: float) -> None: ...
    @property
    def a(self) -> int:
        """
        :type: int
        """
    @a.setter
    def a(self, arg0: int) -> None:
        pass
    @property
    def b(self) -> int:
        """
        :type: int
        """
    @b.setter
    def b(self, arg0: int) -> None:
        pass
    @property
    def g(self) -> int:
        """
        :type: int
        """
    @g.setter
    def g(self, arg0: int) -> None:
        pass
    @property
    def r(self) -> int:
        """
        :type: int
        """
    @r.setter
    def r(self, arg0: int) -> None:
        pass
    BLACK: Engine.Color # value = <Sharpheus.Color (0, 0, 0, 255)>
    BLUE: Engine.Color # value = <Sharpheus.Color (0, 0, 255, 255)>
    CYAN: Engine.Color # value = <Sharpheus.Color (0, 255, 255, 255)>
    GREEN: Engine.Color # value = <Sharpheus.Color (0, 255, 0, 255)>
    MAGENTA: Engine.Color # value = <Sharpheus.Color (255, 0, 255, 255)>
    RED: Engine.Color # value = <Sharpheus.Color (255, 0, 0, 255)>
    TRANSPARENT: Engine.Color # value = <Sharpheus.Color (0, 0, 0, 0)>
    WHITE: Engine.Color # value = <Sharpheus.Color (255, 255, 255, 255)>
    YELLOW: Engine.Color # value = <Sharpheus.Color (255, 255, 0, 255)>
    pass
class ButtonBase(Control, AxisGameObject, RenderableGameObject, ShapedGameObject, GameObject, EventListener):
    @property
    def click_num(self) -> int:
        """
        :type: int
        """
    @property
    def clicked_color(self) -> Color:
        """
        :type: Color
        """
    @clicked_color.setter
    def clicked_color(self, arg1: Color) -> None:
        pass
    @property
    def clicked_font_color(self) -> Color:
        """
        :type: Color
        """
    @clicked_font_color.setter
    def clicked_font_color(self, arg1: Color) -> None:
        pass
    @property
    def color(self) -> Color:
        """
        :type: Color
        """
    @color.setter
    def color(self, arg1: Color) -> None:
        pass
    @property
    def use_clicked_colors(self) -> bool:
        """
        :type: bool
        """
    @use_clicked_colors.setter
    def use_clicked_colors(self, arg1: bool) -> None:
        pass
    pass
class ControlChangedEvent(Event):
    @staticmethod
    def __init__(*args, **kwargs) -> typing.Any: ...
    @property
    def source(self) -> Sharpheus::Control:
        """
        :type: Sharpheus::Control
        """
    pass
class EngineVersion():
    def __eq__(self, arg0: EngineVersion) -> bool: ...
    def __ge__(self, arg0: EngineVersion) -> bool: ...
    def __gt__(self, arg0: EngineVersion) -> bool: ...
    def __le__(self, arg0: EngineVersion) -> bool: ...
    def __lt__(self, arg0: EngineVersion) -> bool: ...
    def __ne__(self, arg0: EngineVersion) -> bool: ...
    @staticmethod
    def get_current() -> EngineVersion: ...
    @staticmethod
    def get_vnum_of(vname: str) -> int: ...
    @typing.overload
    def is_at_least(self, vname: str) -> bool: ...
    @typing.overload
    def is_at_least(self, vnum: int) -> bool: ...
    @typing.overload
    def is_at_least(self, vother: EngineVersion) -> bool: ...
    @property
    def is_latest(self) -> bool:
        """
        :type: bool
        """
    @property
    def is_unknown(self) -> bool:
        """
        :type: bool
        """
    @property
    def is_valid(self) -> bool:
        """
        :type: bool
        """
    @property
    def vname(self) -> str:
        """
        :type: str
        """
    @property
    def vnum(self) -> int:
        """
        :type: int
        """
    __hash__ = None
    latest: Engine.EngineVersion
    pass
class CollisionEvent(Event):
    @staticmethod
    def __init__(*args, **kwargs) -> typing.Any: ...
    @property
    def cd(self) -> CollData:
        """
        :type: CollData
        """
    @property
    def source(self) -> Sharpheus::Collider:
        """
        :type: Sharpheus::Collider
        """
    @property
    def with(self) -> Sharpheus::Collider:
        """
        :type: Sharpheus::Collider
        """
    pass
class AnimatedSprite(RenderableGameObject, ShapedGameObject, GameObject, EventListener):
    def __init__(self, parent: GameObject, name: str) -> None: ...
    def set_animation_from_path(self, path: str) -> None: ...
    @property
    def animation(self) -> Animation:
        """
        :type: Animation
        """
    @animation.setter
    def animation(self, arg1: Animation) -> None:
        pass
    @property
    def speed(self) -> float:
        """
        :type: float
        """
    @speed.setter
    def speed(self, arg1: float) -> None:
        pass
    @property
    def tint(self) -> Color:
        """
        :type: Color
        """
    @tint.setter
    def tint(self, arg1: Color) -> None:
        pass
    pass
class FileLoader():
    def __init__(self, path: str) -> None: ...
    @typing.overload
    def read(self, data: Color) -> bool: ...
    @typing.overload
    def read(self, data: Point) -> bool: ...
    @typing.overload
    def read(self, data: Transform) -> bool: ...
    @typing.overload
    def read(self, data: bool) -> bool: ...
    @typing.overload
    def read(self, data: float) -> bool: ...
    @typing.overload
    def read(self, data: int) -> bool: ...
    @typing.overload
    def read(self, data: str) -> bool: ...
    def read_line(self) -> bool: ...
    def try_reading_end(self) -> bool: ...
    @property
    def status(self) -> bool:
        """
        :type: bool
        """
    pass
class FileSaver():
    def __init__(self, path: str) -> None: ...
    @typing.overload
    def read(self, data: Color) -> bool: ...
    @typing.overload
    def read(self, data: Point) -> bool: ...
    @typing.overload
    def read(self, data: Transform) -> bool: ...
    @typing.overload
    def read(self, data: bool) -> bool: ...
    @typing.overload
    def read(self, data: float) -> bool: ...
    @typing.overload
    def read(self, data: int) -> bool: ...
    @typing.overload
    def read(self, data: str) -> bool: ...
    def write_end(self) -> bool: ...
    @property
    def status(self) -> bool:
        """
        :type: bool
        """
    pass
class Font(Resource):
    def __repr__(self) -> str: ...
    def get_extent(self, text: str, size: float, **kwargs) -> None: ...
    @staticmethod
    def render(*args, **kwargs) -> typing.Any: ...
    @property
    def image(self) -> Image:
        """
        :type: Image
        """
    @property
    def name(self) -> str:
        """
        :type: str
        """
    @property
    def noncopy_cast_address(self) -> int:
        """
        :type: int
        """
    pass
class AnimationPlayer(RenderableGameObject, ShapedGameObject, GameObject, EventListener):
    def __init__(self, parent: GameObject, name: str) -> None: ...
    def add_animation(self, anim: Animation) -> bool: ...
    def add_animation_from_path(self, path: str) -> bool: ...
    def get_animation(self, ind: int) -> Animation: ...
    def get_animation_count(self) -> int: ...
    def get_animation_name(self, ind: int) -> str: ...
    @typing.overload
    def play_once(self, ind: int) -> None: ...
    @typing.overload
    def play_once(self, name: str) -> None: ...
    def remove_animation(self, anim: Animation) -> bool: ...
    def remove_animation_by_index(self, ind: int) -> bool: ...
    def set_animation(self, ind: int, anim: Animation) -> bool: ...
    def set_animation_from_path(self, ind: int, path: str) -> bool: ...
    @property
    def current(self) -> int:
        """
        :type: int
        """
    @current.setter
    def current(self, arg1: int) -> None:
        pass
    @property
    def current_animation(self) -> Animation:
        """
        :type: Animation
        """
    @property
    def current_name(self) -> str:
        """
        :type: str
        """
    @current_name.setter
    def current_name(self, arg1: str) -> None:
        pass
    @property
    def speed(self) -> float:
        """
        :type: float
        """
    @speed.setter
    def speed(self, arg1: float) -> None:
        pass
    @property
    def tint(self) -> Color:
        """
        :type: Color
        """
    @tint.setter
    def tint(self, arg1: Color) -> None:
        pass
    pass
class Image(Resource):
    def __repr__(self) -> str: ...
    @staticmethod
    def render(*args, **kwargs) -> typing.Any: ...
    @staticmethod
    def render_part(*args, **kwargs) -> typing.Any: ...
    @property
    def height(self) -> int:
        """
        :type: int
        """
    @property
    def is_filtered(self) -> bool:
        """
        :type: bool
        """
    @property
    def noncopy_cast_address(self) -> int:
        """
        :type: int
        """
    @property
    def width(self) -> int:
        """
        :type: int
        """
    pass
class ImageButton(ButtonBase, Control, AxisGameObject, RenderableGameObject, ShapedGameObject, GameObject, EventListener):
    def __init__(self, parent: GameObject, name: str) -> None: ...
    def set_clicked_img_from_path(self, path: str, filtered: bool) -> None: ...
    def set_img_from_path(self, path: str, filtered: bool) -> None: ...
    @property
    def clicked_img(self) -> Image:
        """
        :type: Image
        """
    @clicked_img.setter
    def clicked_img(self, arg1: Image) -> None:
        pass
    @property
    def img(self) -> Image:
        """
        :type: Image
        """
    @img.setter
    def img(self, arg1: Image) -> None:
        pass
    pass
class KeyCode():
    """
    Members:

      UNKNOWN

      RIGHT

      LEFT

      DOWN

      UP

      A

      B

      C

      D

      E

      F

      G

      H

      I

      J

      K

      L

      M

      N

      O

      P

      Q

      R

      S

      T

      U

      V

      W

      X

      Y

      Z

      SPACE

      APOSTROPHE

      COMMA

      MINUS

      PERIOD

      SLASH

      SEMICOLON

      EQUAL

      LEFT_BRACKET

      BACKSLASH

      RIGHT_BRACKET

      BACKTICK

      KEY_0

      KEY_1

      KEY_2

      KEY_3

      KEY_4

      KEY_5

      KEY_6

      KEY_7

      KEY_8

      KEY_9

      NP_0

      NP_1

      NP_2

      NP_3

      NP_4

      NP_5

      NP_6

      NP_7

      NP_8

      NP_9

      NP_DECIMAL

      NP_DIVIDE

      NP_MULTIPLY

      NP_SUBTRACT

      NP_ADD

      NP_ENTER

      NP_EQUAL

      ESCAPE

      ENTER

      TAB

      BACKSPACE

      INSERT

      DEL

      PAGE_UP

      PAGE_DOWN

      HOME

      END

      CAPS_LOCK

      SCROLL_LOCK

      NUM_LOCK

      PRINT_SCREEN

      PAUSE

      LEFT_SHIFT

      LEFT_CONTROL

      LEFT_ALT

      LEFT_SUPER

      RIGHT_SHIFT

      RIGHT_CONTROL

      RIGHT_ALT

      RIGHT_SUPER

      MENU

      F1

      F2

      F3

      F4

      F5

      F6

      F7

      F8

      F9

      F10

      F11

      F12

      F13

      F14

      F15

      F16

      F17

      F18

      F19

      F20

      F21

      F22

      F23

      F24

      F25

      WORLD_1

      WORLD_2
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
    A: Engine.KeyCode # value = <KeyCode.A: 65>
    APOSTROPHE: Engine.KeyCode # value = <KeyCode.APOSTROPHE: 39>
    B: Engine.KeyCode # value = <KeyCode.B: 66>
    BACKSLASH: Engine.KeyCode # value = <KeyCode.BACKSLASH: 92>
    BACKSPACE: Engine.KeyCode # value = <KeyCode.BACKSPACE: 259>
    BACKTICK: Engine.KeyCode # value = <KeyCode.BACKTICK: 96>
    C: Engine.KeyCode # value = <KeyCode.C: 67>
    CAPS_LOCK: Engine.KeyCode # value = <KeyCode.CAPS_LOCK: 280>
    COMMA: Engine.KeyCode # value = <KeyCode.COMMA: 44>
    D: Engine.KeyCode # value = <KeyCode.D: 68>
    DEL: Engine.KeyCode # value = <KeyCode.DEL: 261>
    DOWN: Engine.KeyCode # value = <KeyCode.DOWN: 264>
    E: Engine.KeyCode # value = <KeyCode.E: 69>
    END: Engine.KeyCode # value = <KeyCode.END: 269>
    ENTER: Engine.KeyCode # value = <KeyCode.ENTER: 257>
    EQUAL: Engine.KeyCode # value = <KeyCode.EQUAL: 61>
    ESCAPE: Engine.KeyCode # value = <KeyCode.ESCAPE: 256>
    F: Engine.KeyCode # value = <KeyCode.F: 70>
    F1: Engine.KeyCode # value = <KeyCode.F1: 290>
    F10: Engine.KeyCode # value = <KeyCode.F10: 299>
    F11: Engine.KeyCode # value = <KeyCode.F11: 300>
    F12: Engine.KeyCode # value = <KeyCode.F12: 301>
    F13: Engine.KeyCode # value = <KeyCode.F13: 302>
    F14: Engine.KeyCode # value = <KeyCode.F14: 303>
    F15: Engine.KeyCode # value = <KeyCode.F15: 304>
    F16: Engine.KeyCode # value = <KeyCode.F16: 305>
    F17: Engine.KeyCode # value = <KeyCode.F17: 306>
    F18: Engine.KeyCode # value = <KeyCode.F18: 307>
    F19: Engine.KeyCode # value = <KeyCode.F19: 308>
    F2: Engine.KeyCode # value = <KeyCode.F2: 291>
    F20: Engine.KeyCode # value = <KeyCode.F20: 309>
    F21: Engine.KeyCode # value = <KeyCode.F21: 310>
    F22: Engine.KeyCode # value = <KeyCode.F22: 311>
    F23: Engine.KeyCode # value = <KeyCode.F23: 312>
    F24: Engine.KeyCode # value = <KeyCode.F24: 313>
    F25: Engine.KeyCode # value = <KeyCode.F25: 314>
    F3: Engine.KeyCode # value = <KeyCode.F3: 292>
    F4: Engine.KeyCode # value = <KeyCode.F4: 293>
    F5: Engine.KeyCode # value = <KeyCode.F5: 294>
    F6: Engine.KeyCode # value = <KeyCode.F6: 295>
    F7: Engine.KeyCode # value = <KeyCode.F7: 296>
    F8: Engine.KeyCode # value = <KeyCode.F8: 297>
    F9: Engine.KeyCode # value = <KeyCode.F9: 298>
    G: Engine.KeyCode # value = <KeyCode.G: 71>
    H: Engine.KeyCode # value = <KeyCode.H: 72>
    HOME: Engine.KeyCode # value = <KeyCode.HOME: 268>
    I: Engine.KeyCode # value = <KeyCode.I: 73>
    INSERT: Engine.KeyCode # value = <KeyCode.INSERT: 260>
    J: Engine.KeyCode # value = <KeyCode.J: 74>
    K: Engine.KeyCode # value = <KeyCode.K: 75>
    KEY_0: Engine.KeyCode # value = <KeyCode.KEY_0: 48>
    KEY_1: Engine.KeyCode # value = <KeyCode.KEY_1: 49>
    KEY_2: Engine.KeyCode # value = <KeyCode.KEY_2: 50>
    KEY_3: Engine.KeyCode # value = <KeyCode.KEY_3: 51>
    KEY_4: Engine.KeyCode # value = <KeyCode.KEY_4: 52>
    KEY_5: Engine.KeyCode # value = <KeyCode.KEY_5: 53>
    KEY_6: Engine.KeyCode # value = <KeyCode.KEY_6: 54>
    KEY_7: Engine.KeyCode # value = <KeyCode.KEY_7: 55>
    KEY_8: Engine.KeyCode # value = <KeyCode.KEY_8: 56>
    KEY_9: Engine.KeyCode # value = <KeyCode.KEY_9: 57>
    L: Engine.KeyCode # value = <KeyCode.L: 76>
    LEFT: Engine.KeyCode # value = <KeyCode.LEFT: 263>
    LEFT_ALT: Engine.KeyCode # value = <KeyCode.LEFT_ALT: 342>
    LEFT_BRACKET: Engine.KeyCode # value = <KeyCode.LEFT_BRACKET: 91>
    LEFT_CONTROL: Engine.KeyCode # value = <KeyCode.LEFT_CONTROL: 341>
    LEFT_SHIFT: Engine.KeyCode # value = <KeyCode.LEFT_SHIFT: 340>
    LEFT_SUPER: Engine.KeyCode # value = <KeyCode.LEFT_SUPER: 343>
    M: Engine.KeyCode # value = <KeyCode.M: 77>
    MENU: Engine.KeyCode # value = <KeyCode.MENU: 348>
    MINUS: Engine.KeyCode # value = <KeyCode.MINUS: 45>
    N: Engine.KeyCode # value = <KeyCode.N: 78>
    NP_0: Engine.KeyCode # value = <KeyCode.NP_0: 320>
    NP_1: Engine.KeyCode # value = <KeyCode.NP_1: 321>
    NP_2: Engine.KeyCode # value = <KeyCode.NP_2: 322>
    NP_3: Engine.KeyCode # value = <KeyCode.NP_3: 323>
    NP_4: Engine.KeyCode # value = <KeyCode.NP_4: 324>
    NP_5: Engine.KeyCode # value = <KeyCode.NP_5: 325>
    NP_6: Engine.KeyCode # value = <KeyCode.NP_6: 326>
    NP_7: Engine.KeyCode # value = <KeyCode.NP_7: 327>
    NP_8: Engine.KeyCode # value = <KeyCode.NP_8: 328>
    NP_9: Engine.KeyCode # value = <KeyCode.NP_9: 329>
    NP_ADD: Engine.KeyCode # value = <KeyCode.NP_ADD: 334>
    NP_DECIMAL: Engine.KeyCode # value = <KeyCode.NP_DECIMAL: 330>
    NP_DIVIDE: Engine.KeyCode # value = <KeyCode.NP_DIVIDE: 331>
    NP_ENTER: Engine.KeyCode # value = <KeyCode.NP_ENTER: 335>
    NP_EQUAL: Engine.KeyCode # value = <KeyCode.NP_EQUAL: 336>
    NP_MULTIPLY: Engine.KeyCode # value = <KeyCode.NP_MULTIPLY: 332>
    NP_SUBTRACT: Engine.KeyCode # value = <KeyCode.NP_SUBTRACT: 333>
    NUM_LOCK: Engine.KeyCode # value = <KeyCode.NUM_LOCK: 282>
    O: Engine.KeyCode # value = <KeyCode.O: 79>
    P: Engine.KeyCode # value = <KeyCode.P: 80>
    PAGE_DOWN: Engine.KeyCode # value = <KeyCode.PAGE_DOWN: 267>
    PAGE_UP: Engine.KeyCode # value = <KeyCode.PAGE_UP: 266>
    PAUSE: Engine.KeyCode # value = <KeyCode.PAUSE: 284>
    PERIOD: Engine.KeyCode # value = <KeyCode.PERIOD: 46>
    PRINT_SCREEN: Engine.KeyCode # value = <KeyCode.NP_EQUAL: 336>
    Q: Engine.KeyCode # value = <KeyCode.Q: 81>
    R: Engine.KeyCode # value = <KeyCode.R: 82>
    RIGHT: Engine.KeyCode # value = <KeyCode.RIGHT: 262>
    RIGHT_ALT: Engine.KeyCode # value = <KeyCode.RIGHT_ALT: 346>
    RIGHT_BRACKET: Engine.KeyCode # value = <KeyCode.RIGHT_BRACKET: 93>
    RIGHT_CONTROL: Engine.KeyCode # value = <KeyCode.RIGHT_CONTROL: 345>
    RIGHT_SHIFT: Engine.KeyCode # value = <KeyCode.RIGHT_SHIFT: 344>
    RIGHT_SUPER: Engine.KeyCode # value = <KeyCode.RIGHT_SUPER: 347>
    S: Engine.KeyCode # value = <KeyCode.S: 83>
    SCROLL_LOCK: Engine.KeyCode # value = <KeyCode.SCROLL_LOCK: 281>
    SEMICOLON: Engine.KeyCode # value = <KeyCode.SEMICOLON: 59>
    SLASH: Engine.KeyCode # value = <KeyCode.SLASH: 47>
    SPACE: Engine.KeyCode # value = <KeyCode.SPACE: 32>
    T: Engine.KeyCode # value = <KeyCode.T: 84>
    TAB: Engine.KeyCode # value = <KeyCode.TAB: 258>
    U: Engine.KeyCode # value = <KeyCode.U: 85>
    UNKNOWN: Engine.KeyCode # value = <KeyCode.UNKNOWN: -1>
    UP: Engine.KeyCode # value = <KeyCode.UP: 265>
    V: Engine.KeyCode # value = <KeyCode.V: 86>
    W: Engine.KeyCode # value = <KeyCode.W: 87>
    WORLD_1: Engine.KeyCode # value = <KeyCode.WORLD_1: 161>
    WORLD_2: Engine.KeyCode # value = <KeyCode.WORLD_2: 162>
    X: Engine.KeyCode # value = <KeyCode.X: 88>
    Y: Engine.KeyCode # value = <KeyCode.Y: 89>
    Z: Engine.KeyCode # value = <KeyCode.Z: 90>
    __members__: dict # value = {'UNKNOWN': <KeyCode.UNKNOWN: -1>, 'RIGHT': <KeyCode.RIGHT: 262>, 'LEFT': <KeyCode.LEFT: 263>, 'DOWN': <KeyCode.DOWN: 264>, 'UP': <KeyCode.UP: 265>, 'A': <KeyCode.A: 65>, 'B': <KeyCode.B: 66>, 'C': <KeyCode.C: 67>, 'D': <KeyCode.D: 68>, 'E': <KeyCode.E: 69>, 'F': <KeyCode.F: 70>, 'G': <KeyCode.G: 71>, 'H': <KeyCode.H: 72>, 'I': <KeyCode.I: 73>, 'J': <KeyCode.J: 74>, 'K': <KeyCode.K: 75>, 'L': <KeyCode.L: 76>, 'M': <KeyCode.M: 77>, 'N': <KeyCode.N: 78>, 'O': <KeyCode.O: 79>, 'P': <KeyCode.P: 80>, 'Q': <KeyCode.Q: 81>, 'R': <KeyCode.R: 82>, 'S': <KeyCode.S: 83>, 'T': <KeyCode.T: 84>, 'U': <KeyCode.U: 85>, 'V': <KeyCode.V: 86>, 'W': <KeyCode.W: 87>, 'X': <KeyCode.X: 88>, 'Y': <KeyCode.Y: 89>, 'Z': <KeyCode.Z: 90>, 'SPACE': <KeyCode.SPACE: 32>, 'APOSTROPHE': <KeyCode.APOSTROPHE: 39>, 'COMMA': <KeyCode.COMMA: 44>, 'MINUS': <KeyCode.MINUS: 45>, 'PERIOD': <KeyCode.PERIOD: 46>, 'SLASH': <KeyCode.SLASH: 47>, 'SEMICOLON': <KeyCode.SEMICOLON: 59>, 'EQUAL': <KeyCode.EQUAL: 61>, 'LEFT_BRACKET': <KeyCode.LEFT_BRACKET: 91>, 'BACKSLASH': <KeyCode.BACKSLASH: 92>, 'RIGHT_BRACKET': <KeyCode.RIGHT_BRACKET: 93>, 'BACKTICK': <KeyCode.BACKTICK: 96>, 'KEY_0': <KeyCode.KEY_0: 48>, 'KEY_1': <KeyCode.KEY_1: 49>, 'KEY_2': <KeyCode.KEY_2: 50>, 'KEY_3': <KeyCode.KEY_3: 51>, 'KEY_4': <KeyCode.KEY_4: 52>, 'KEY_5': <KeyCode.KEY_5: 53>, 'KEY_6': <KeyCode.KEY_6: 54>, 'KEY_7': <KeyCode.KEY_7: 55>, 'KEY_8': <KeyCode.KEY_8: 56>, 'KEY_9': <KeyCode.KEY_9: 57>, 'NP_0': <KeyCode.NP_0: 320>, 'NP_1': <KeyCode.NP_1: 321>, 'NP_2': <KeyCode.NP_2: 322>, 'NP_3': <KeyCode.NP_3: 323>, 'NP_4': <KeyCode.NP_4: 324>, 'NP_5': <KeyCode.NP_5: 325>, 'NP_6': <KeyCode.NP_6: 326>, 'NP_7': <KeyCode.NP_7: 327>, 'NP_8': <KeyCode.NP_8: 328>, 'NP_9': <KeyCode.NP_9: 329>, 'NP_DECIMAL': <KeyCode.NP_DECIMAL: 330>, 'NP_DIVIDE': <KeyCode.NP_DIVIDE: 331>, 'NP_MULTIPLY': <KeyCode.NP_MULTIPLY: 332>, 'NP_SUBTRACT': <KeyCode.NP_SUBTRACT: 333>, 'NP_ADD': <KeyCode.NP_ADD: 334>, 'NP_ENTER': <KeyCode.NP_ENTER: 335>, 'NP_EQUAL': <KeyCode.NP_EQUAL: 336>, 'ESCAPE': <KeyCode.ESCAPE: 256>, 'ENTER': <KeyCode.ENTER: 257>, 'TAB': <KeyCode.TAB: 258>, 'BACKSPACE': <KeyCode.BACKSPACE: 259>, 'INSERT': <KeyCode.INSERT: 260>, 'DEL': <KeyCode.DEL: 261>, 'PAGE_UP': <KeyCode.PAGE_UP: 266>, 'PAGE_DOWN': <KeyCode.PAGE_DOWN: 267>, 'HOME': <KeyCode.HOME: 268>, 'END': <KeyCode.END: 269>, 'CAPS_LOCK': <KeyCode.CAPS_LOCK: 280>, 'SCROLL_LOCK': <KeyCode.SCROLL_LOCK: 281>, 'NUM_LOCK': <KeyCode.NUM_LOCK: 282>, 'PRINT_SCREEN': <KeyCode.NP_EQUAL: 336>, 'PAUSE': <KeyCode.PAUSE: 284>, 'LEFT_SHIFT': <KeyCode.LEFT_SHIFT: 340>, 'LEFT_CONTROL': <KeyCode.LEFT_CONTROL: 341>, 'LEFT_ALT': <KeyCode.LEFT_ALT: 342>, 'LEFT_SUPER': <KeyCode.LEFT_SUPER: 343>, 'RIGHT_SHIFT': <KeyCode.RIGHT_SHIFT: 344>, 'RIGHT_CONTROL': <KeyCode.RIGHT_CONTROL: 345>, 'RIGHT_ALT': <KeyCode.RIGHT_ALT: 346>, 'RIGHT_SUPER': <KeyCode.RIGHT_SUPER: 347>, 'MENU': <KeyCode.MENU: 348>, 'F1': <KeyCode.F1: 290>, 'F2': <KeyCode.F2: 291>, 'F3': <KeyCode.F3: 292>, 'F4': <KeyCode.F4: 293>, 'F5': <KeyCode.F5: 294>, 'F6': <KeyCode.F6: 295>, 'F7': <KeyCode.F7: 296>, 'F8': <KeyCode.F8: 297>, 'F9': <KeyCode.F9: 298>, 'F10': <KeyCode.F10: 299>, 'F11': <KeyCode.F11: 300>, 'F12': <KeyCode.F12: 301>, 'F13': <KeyCode.F13: 302>, 'F14': <KeyCode.F14: 303>, 'F15': <KeyCode.F15: 304>, 'F16': <KeyCode.F16: 305>, 'F17': <KeyCode.F17: 306>, 'F18': <KeyCode.F18: 307>, 'F19': <KeyCode.F19: 308>, 'F20': <KeyCode.F20: 309>, 'F21': <KeyCode.F21: 310>, 'F22': <KeyCode.F22: 311>, 'F23': <KeyCode.F23: 312>, 'F24': <KeyCode.F24: 313>, 'F25': <KeyCode.F25: 314>, 'WORLD_1': <KeyCode.WORLD_1: 161>, 'WORLD_2': <KeyCode.WORLD_2: 162>}
    pass
class KeyEvent(Event):
    @property
    def code(self) -> KeyCode:
        """
        :type: KeyCode
        """
    pass
class KeyHoldEvent(KeyEvent, Event):
    def __init__(self, code: KeyCode) -> None: ...
    pass
class KeyPressedEvent(KeyEvent, Event):
    def __init__(self, code: KeyCode) -> None: ...
    pass
class KeyReleasedEvent(KeyEvent, Event):
    def __init__(self, code: KeyCode) -> None: ...
    pass
class Level():
    def attach(self, obj: GameObject) -> None: ...
    def attach_scene_to(self, obj: GameObject, path: str) -> bool: ...
    @typing.overload
    def create(self, other: GameObject, parent: GameObject, new_name: str) -> GameObject: ...
    @typing.overload
    def create(self, type: GameObject.Type, parent: GameObject, new_name: str) -> GameObject: ...
    @typing.overload
    def create_layer(self, name: str) -> bool: ...
    @typing.overload
    def create_layer(self, name: str, ind: int) -> bool: ...
    def delete(self, obj: GameObject, force_now: bool = False) -> None: ...
    def get_gameobject(self, name: str) -> GameObject: ...
    @staticmethod
    def is_deletable_layer(layer: str) -> bool: ...
    @typing.overload
    def is_layer_visible(self, ind: int) -> bool: ...
    @typing.overload
    def is_layer_visible(self, name: str) -> bool: ...
    def move(self, obj: GameObject, new_parent: GameObject) -> bool: ...
    @typing.overload
    def remove_layer(self, ind: int, move2layer: int) -> bool: ...
    @typing.overload
    def remove_layer(self, name: str, move2layer: str) -> bool: ...
    @typing.overload
    def rename_layer(self, ind: int, new_name: str) -> bool: ...
    @typing.overload
    def rename_layer(self, old_name: str, new_name: str) -> bool: ...
    def save_as_scene(self, obj: GameObject, path: str) -> bool: ...
    @typing.overload
    def set_layer_visible(self, ind: int, visibility: bool) -> None: ...
    @typing.overload
    def set_layer_visible(self, name: str, visibility: bool) -> None: ...
    @typing.overload
    def swap_layer(self, first_layer: int, second_layer: int) -> bool: ...
    @typing.overload
    def swap_layer(self, first_layer: str, second_layer: str) -> bool: ...
    @property
    def full_path(self) -> str:
        """
        :type: str
        """
    @property
    def layer_names(self) -> typing.List[str]:
        """
        :type: typing.List[str]
        """
    @property
    def layer_num(self) -> int:
        """
        :type: int
        """
    @property
    def name(self) -> str:
        """
        :type: str
        """
    @property
    def path(self) -> str:
        """
        :type: str
        """
    @property
    def project_file_name(self) -> str:
        """
        :type: str
        """
    @property
    def root(self) -> GameObject:
        """
        :type: GameObject
        """
    pass
class MouseButton():
    """
    Members:

      LEFT

      RIGHT

      MIDDLE

      BUTTON_4

      BUTTON_5

      BUTTON_6

      BUTTON_7

      BUTTON_8
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
    BUTTON_4: Engine.MouseButton # value = <MouseButton.BUTTON_4: 4>
    BUTTON_5: Engine.MouseButton # value = <MouseButton.BUTTON_5: 5>
    BUTTON_6: Engine.MouseButton # value = <MouseButton.BUTTON_6: 6>
    BUTTON_7: Engine.MouseButton # value = <MouseButton.BUTTON_7: 7>
    BUTTON_8: Engine.MouseButton # value = <MouseButton.BUTTON_8: 8>
    LEFT: Engine.MouseButton # value = <MouseButton.LEFT: 1>
    MIDDLE: Engine.MouseButton # value = <MouseButton.MIDDLE: 3>
    RIGHT: Engine.MouseButton # value = <MouseButton.RIGHT: 2>
    __members__: dict # value = {'LEFT': <MouseButton.LEFT: 1>, 'RIGHT': <MouseButton.RIGHT: 2>, 'MIDDLE': <MouseButton.MIDDLE: 3>, 'BUTTON_4': <MouseButton.BUTTON_4: 4>, 'BUTTON_5': <MouseButton.BUTTON_5: 5>, 'BUTTON_6': <MouseButton.BUTTON_6: 6>, 'BUTTON_7': <MouseButton.BUTTON_7: 7>, 'BUTTON_8': <MouseButton.BUTTON_8: 8>}
    pass
class MouseEvent(Event):
    @property
    def game_pos(self) -> Point:
        """
        :type: Point
        """
    @property
    def screen_pos(self) -> Point:
        """
        :type: Point
        """
    pass
class MouseMovedEvent(MouseEvent, Event):
    def __init__(self, screen_pos: Point, game_pos: Point) -> None: ...
    pass
class MousePressedEvent(MouseEvent, Event):
    def __init__(self, screen_pos: Point, game_pos: Point, button: MouseButton) -> None: ...
    @property
    def button(self) -> MouseButton:
        """
        :type: MouseButton
        """
    pass
class MouseReleasedEvent(MouseEvent, Event):
    def __init__(self, screen_pos: Point, game_pos: Point, button: MouseButton) -> None: ...
    @property
    def button(self) -> MouseButton:
        """
        :type: MouseButton
        """
    pass
class MouseScrolledEvent(MouseEvent, Event):
    def __init__(self, screen_pos: Point, game_pos: Point, is_up: bool) -> None: ...
    @property
    def is_up(self) -> bool:
        """
        :type: bool
        """
    pass
class MusicPlayer(GameObject, EventListener):
    def __init__(self, parent: GameObject, name: str) -> None: ...
    def pause(self) -> None: ...
    def play(self) -> None: ...
    def resume(self) -> None: ...
    def set_music_from_path(self, musicPath: str) -> None: ...
    def stop(self) -> None: ...
    @property
    def is_looping(self) -> bool:
        """
        :type: bool
        """
    @is_looping.setter
    def is_looping(self, arg1: bool) -> None:
        pass
    @property
    def is_playing(self) -> bool:
        """
        :type: bool
        """
    @property
    def music(self) -> Audio:
        """
        :type: Audio
        """
    @music.setter
    def music(self, arg1: Audio) -> None:
        pass
    @property
    def play_speed(self) -> float:
        """
        :type: float
        """
    @play_speed.setter
    def play_speed(self, arg1: float) -> None:
        pass
    @property
    def starts_by_default(self) -> bool:
        """
        :type: bool
        """
    @starts_by_default.setter
    def starts_by_default(self, arg1: bool) -> None:
        pass
    @property
    def volume(self) -> int:
        """
        :type: int
        """
    @volume.setter
    def volume(self, arg1: int) -> None:
        pass
    pass
class OnEnterEvent(Event):
    @staticmethod
    def __init__(*args, **kwargs) -> typing.Any: ...
    @property
    def obj(self) -> GameObject:
        """
        :type: GameObject
        """
    @property
    def source(self) -> Sharpheus::Collider:
        """
        :type: Sharpheus::Collider
        """
    pass
class OnExitEvent(Event):
    @staticmethod
    def __init__(*args, **kwargs) -> typing.Any: ...
    @property
    def obj(self) -> GameObject:
        """
        :type: GameObject
        """
    @property
    def obj_destroyed(self) -> bool:
        """
        :type: bool
        """
    @property
    def source(self) -> Sharpheus::Collider:
        """
        :type: Sharpheus::Collider
        """
    pass
class Oval():
    def __init__(self, pos: Point, dim: Point, rot: float) -> None: ...
    def __repr__(self) -> str: ...
    @property
    def is_circle(self) -> bool:
        """
        :type: bool
        """
    pass
class ParticleEmitter(AxisGameObject, RenderableGameObject, ShapedGameObject, GameObject, EventListener):
    class ParticleState():
        def __init__(self, direction: Point, pos: Point, start_time: float, speed: float) -> None: ...
        @property
        def direction(self) -> Point:
            """
            :type: Point
            """
        @direction.setter
        def direction(self, arg0: Point) -> None:
            pass
        @property
        def pos(self) -> Point:
            """
            :type: Point
            """
        @pos.setter
        def pos(self, arg0: Point) -> None:
            pass
        @property
        def speed(self) -> float:
            """
            :type: float
            """
        @speed.setter
        def speed(self, arg0: float) -> None:
            pass
        @property
        def start_time(self) -> float:
            """
            :type: float
            """
        @start_time.setter
        def start_time(self, arg0: float) -> None:
            pass
        pass
    def __init__(self, parent: GameObject, name: str) -> None: ...
    def clear_custom_func(self) -> None: ...
    def set_custom_func(self, customFunc: typing.Callable[[ParticleEmitter, ParticleEmitter.ParticleState, float, float], None]) -> None: ...
    def set_particle_from_path(self, path: str, filtered: bool) -> None: ...
    @property
    def burst_n(self) -> int:
        """
        :type: int
        """
    @burst_n.setter
    def burst_n(self, arg1: int) -> None:
        pass
    @property
    def emission_time(self) -> float:
        """
        :type: float
        """
    @emission_time.setter
    def emission_time(self, arg1: float) -> None:
        pass
    @property
    def friction(self) -> float:
        """
        :type: float
        """
    @friction.setter
    def friction(self, arg1: float) -> None:
        pass
    @property
    def gravity(self) -> float:
        """
        :type: float
        """
    @gravity.setter
    def gravity(self, arg1: float) -> None:
        pass
    @property
    def is_burst(self) -> bool:
        """
        :type: bool
        """
    @is_burst.setter
    def is_burst(self, arg1: bool) -> None:
        pass
    @property
    def is_fade(self) -> bool:
        """
        :type: bool
        """
    @is_fade.setter
    def is_fade(self, arg1: bool) -> None:
        pass
    @property
    def is_shrink(self) -> bool:
        """
        :type: bool
        """
    @is_shrink.setter
    def is_shrink(self, arg1: bool) -> None:
        pass
    @property
    def lifetime(self) -> float:
        """
        :type: float
        """
    @lifetime.setter
    def lifetime(self, arg1: float) -> None:
        pass
    @property
    def particle(self) -> Image:
        """
        :type: Image
        """
    @particle.setter
    def particle(self, arg1: Image) -> None:
        pass
    @property
    def particle_scale(self) -> Point:
        """
        :type: Point
        """
    @particle_scale.setter
    def particle_scale(self, arg1: Point) -> None:
        pass
    @property
    def speed(self) -> float:
        """
        :type: float
        """
    @speed.setter
    def speed(self, arg1: float) -> None:
        pass
    @property
    def tint(self) -> Color:
        """
        :type: Color
        """
    @tint.setter
    def tint(self, arg1: Color) -> None:
        pass
    pass
class PhysicsObject(GameObject, EventListener):
    def __init__(self, parent: GameObject, name: str) -> None: ...
    def add_velocity(self, velocity: Point) -> None: ...
    @property
    def bounce(self) -> float:
        """
        :type: float
        """
    @bounce.setter
    def bounce(self, arg1: float) -> None:
        pass
    @property
    def friction(self) -> float:
        """
        :type: float
        """
    @friction.setter
    def friction(self, arg1: float) -> None:
        pass
    @property
    def gravity(self) -> float:
        """
        :type: float
        """
    @gravity.setter
    def gravity(self, arg1: float) -> None:
        pass
    @property
    def mass(self) -> float:
        """
        :type: float
        """
    @mass.setter
    def mass(self, arg1: float) -> None:
        pass
    @property
    def velocity(self) -> Point:
        """
        :type: Point
        """
    @velocity.setter
    def velocity(self, arg1: Point) -> None:
        pass
    pass
class Point():
    def __add__(self, arg0: Point) -> Point: ...
    def __eq__(self, arg0: Point) -> bool: ...
    def __iadd__(self, arg0: Point) -> Point: ...
    def __imul__(self, arg0: float) -> Point: ...
    def __init__(self, x: float = 0.0, y: float = 0.0) -> None: ...
    def __isub__(self, arg0: Point) -> Point: ...
    def __itruediv__(self, arg0: float) -> Point: ...
    @typing.overload
    def __mul__(self, arg0: Point) -> float: ...
    @typing.overload
    def __mul__(self, arg0: float) -> Point: ...
    def __ne__(self, arg0: Point) -> bool: ...
    def __repr__(self) -> str: ...
    def __rmul__(self, arg0: float) -> Point: ...
    def __rtruediv__(self, arg0: float) -> Point: ...
    def __sub__(self, arg0: Point) -> Point: ...
    def __truediv__(self, arg0: float) -> Point: ...
    def distance(self, other: Point) -> float: ...
    def distance_squared(self, other: Point) -> float: ...
    def get_angle(self) -> float: ...
    def get_angle_with(self, other: Point) -> float: ...
    @staticmethod
    def get_unit(angle: float) -> Point: ...
    def length(self) -> float: ...
    def length_squared(self) -> float: ...
    def normalize(self) -> Point: ...
    def rotate(self, angle: float) -> Point: ...
    @property
    def x(self) -> float:
        """
        :type: float
        """
    @x.setter
    def x(self, arg0: float) -> None:
        pass
    @property
    def y(self) -> float:
        """
        :type: float
        """
    @y.setter
    def y(self, arg0: float) -> None:
        pass
    DOWN: Engine.Point # value = <Sharpheus.Point (0.000000, 1.000000)>
    LEFT: Engine.Point # value = <Sharpheus.Point (-1.000000, 0.000000)>
    RIGHT: Engine.Point # value = <Sharpheus.Point (1.000000, 0.000000)>
    UP: Engine.Point # value = <Sharpheus.Point (0.000000, -1.000000)>
    ZERO: Engine.Point # value = <Sharpheus.Point (0.000000, 0.000000)>
    __hash__ = None
    pass
class PythonBehavior(Behavior, GameObject, EventListener):
    def __init__(self, parent: GameObject, name: str) -> None: ...
    @staticmethod
    def subscribe_collision(*args, **kwargs) -> typing.Any: ...
    @staticmethod
    def subscribe_control_changed(*args, **kwargs) -> typing.Any: ...
    def subscribe_for_render(self, layer_name: str, func: typing.Callable[[], None]) -> None: ...
    @staticmethod
    def subscribe_trigger_enter(*args, **kwargs) -> typing.Any: ...
    @staticmethod
    def subscribe_trigger_exit(*args, **kwargs) -> typing.Any: ...
    @staticmethod
    def unsubscribe_collision(*args, **kwargs) -> typing.Any: ...
    @staticmethod
    def unsubscribe_control_changed(*args, **kwargs) -> typing.Any: ...
    def unsubscribe_for_render(self) -> None: ...
    @staticmethod
    def unsubscribe_trigger_enter(*args, **kwargs) -> typing.Any: ...
    @staticmethod
    def unsubscribe_trigger_exit(*args, **kwargs) -> typing.Any: ...
    pass
class PythonRunnerBehavior(Behavior, GameObject, EventListener):
    def __init__(self, parent: GameObject, name: str) -> None: ...
    pass
class Quad(RenderableGameObject, ShapedGameObject, GameObject, EventListener):
    def __init__(self, parent: GameObject, name: str) -> None: ...
    @property
    def color(self) -> Color:
        """
        :type: Color
        """
    @color.setter
    def color(self, arg1: Color) -> None:
        pass
    @property
    def height(self) -> float:
        """
        :type: float
        """
    @height.setter
    def height(self, arg1: float) -> None:
        pass
    @property
    def width(self) -> float:
        """
        :type: float
        """
    @width.setter
    def width(self, arg1: float) -> None:
        pass
    pass
class RadioButton(CheckBase, Control, AxisGameObject, RenderableGameObject, ShapedGameObject, GameObject, EventListener):
    def __init__(self, parent: GameObject, name: str) -> None: ...
    @staticmethod
    def get_value_of(id: int) -> str: ...
    @property
    def group_id(self) -> int:
        """
        :type: int
        """
    @group_id.setter
    def group_id(self, arg1: int) -> None:
        pass
    @property
    def value(self) -> str:
        """
        :type: str
        """
    @value.setter
    def value(self, arg1: str) -> None:
        pass
    pass
class Rect():
    def __init__(self, pos: Point, dim: Point, rot: float) -> None: ...
    def __repr__(self) -> str: ...
    pass
class Button(ButtonBase, Control, AxisGameObject, RenderableGameObject, ShapedGameObject, GameObject, EventListener):
    def __init__(self, parent: GameObject, name: str) -> None: ...
    @property
    def height(self) -> float:
        """
        :type: float
        """
    @height.setter
    def height(self, arg1: float) -> None:
        pass
    @property
    def width(self) -> float:
        """
        :type: float
        """
    @width.setter
    def width(self, arg1: float) -> None:
        pass
    pass
class Animation(Resource):
    def __repr__(self) -> str: ...
    @staticmethod
    def render(*args, **kwargs) -> typing.Any: ...
    @property
    def atlas(self) -> Image:
        """
        :type: Image
        """
    @property
    def end_frame(self) -> int:
        """
        :type: int
        """
    @property
    def frame_cols(self) -> int:
        """
        :type: int
        """
    @property
    def frame_height(self) -> int:
        """
        :type: int
        """
    @property
    def frame_rows(self) -> int:
        """
        :type: int
        """
    @property
    def frame_time(self) -> float:
        """
        :type: float
        """
    @property
    def frame_width(self) -> int:
        """
        :type: int
        """
    @property
    def full_time(self) -> float:
        """
        :type: float
        """
    @property
    def name(self) -> str:
        """
        :type: str
        """
    @property
    def noncopy_cast_address(self) -> int:
        """
        :type: int
        """
    @property
    def num_of_all_frames(self) -> int:
        """
        :type: int
        """
    @property
    def num_of_frames(self) -> int:
        """
        :type: int
        """
    @property
    def start_frame(self) -> int:
        """
        :type: int
        """
    pass
class Shape():
    class Intersection():
        def __init__(self, contact: Point, normal: Point, depth: float) -> None: ...
        @property
        def contact(self) -> Point:
            """
            :type: Point
            """
        @contact.setter
        def contact(self, arg0: Point) -> None:
            pass
        @property
        def depth(self) -> float:
            """
            :type: float
            """
        @depth.setter
        def depth(self, arg0: float) -> None:
            pass
        @property
        def normal(self) -> Point:
            """
            :type: Point
            """
        @normal.setter
        def normal(self, arg0: Point) -> None:
            pass
        pass
    class Type():
        """
        Members:

          OVAL

          RECT

          CAPSULE
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
        CAPSULE: Engine.Shape.Type # value = <Type.CAPSULE: 2>
        OVAL: Engine.Shape.Type # value = <Type.OVAL: 0>
        RECT: Engine.Shape.Type # value = <Type.RECT: 1>
        __members__: dict # value = {'OVAL': <Type.OVAL: 0>, 'RECT': <Type.RECT: 1>, 'CAPSULE': <Type.CAPSULE: 2>}
        pass
    def force_refresh(self) -> None: ...
    def get_intersection_with(self, arg0: Shape) -> Shape.Intersection: ...
    def get_local_closest_to(self, point: Point) -> Point: ...
    def get_local_perpendicular_at(self, surface_point: Point) -> Point: ...
    def get_priority(self) -> int: ...
    def get_sat_corner_num(self) -> int: ...
    def is_inside(self, pos: Point) -> bool: ...
    def is_sat_symmetrical(self) -> bool: ...
    def is_too_far_from(self, other: Shape) -> bool: ...
    def type(self) -> Shape.Type: ...
    @property
    def corners(self) -> typing.List[Point]:
        """
        :type: typing.List[Point]
        """
    @property
    def dim(self) -> Point:
        """
        :type: Point
        """
    @dim.setter
    def dim(self, arg1: Point) -> None:
        pass
    @property
    def pos(self) -> Point:
        """
        :type: Point
        """
    @pos.setter
    def pos(self, arg1: Point) -> None:
        pass
    @property
    def rot(self) -> float:
        """
        :type: float
        """
    @rot.setter
    def rot(self, arg1: float) -> None:
        pass
    @property
    def sat_corners(self) -> typing.List[Point]:
        """
        :type: typing.List[Point]
        """
    @property
    def x_axis(self) -> Point:
        """
        :type: Point
        """
    @property
    def y_axis(self) -> Point:
        """
        :type: Point
        """
    pass
class CheckBox(CheckBase, Control, AxisGameObject, RenderableGameObject, ShapedGameObject, GameObject, EventListener):
    def __init__(self, parent: GameObject, name: str) -> None: ...
    pass
class SoundEffects(GameObject, EventListener):
    def __init__(self, parent: GameObject, name: str) -> None: ...
    def add_audio(self, audio: Audio) -> bool: ...
    def add_audio_from_path(self, path: str) -> bool: ...
    def get_audio(self, ind: int) -> Audio: ...
    def get_audio_name(self, ind: int) -> str: ...
    def get_num_of_audios(self) -> int: ...
    @typing.overload
    def play(self, ind: int, volume: int = 100, playSpeed: float = 1.0) -> None: ...
    @typing.overload
    def play(self, name: str, volume: int = 100, playSpeed: float = 1.0) -> None: ...
    def remove_audio(self, audio: Audio) -> bool: ...
    def remove_audio_by_ind(self, ind: int) -> bool: ...
    def set_audio(self, ind: int, audio: Audio) -> bool: ...
    def set_audio_from_path(self, ind: int, path: str) -> bool: ...
    def stop_all(self) -> None: ...
    pass
class Sprite(RenderableGameObject, ShapedGameObject, GameObject, EventListener):
    def __init__(self, parent: GameObject, name: str) -> None: ...
    def set_image_from_path(self, path: str, filtered: bool) -> None: ...
    @property
    def image(self) -> Image:
        """
        :type: Image
        """
    @image.setter
    def image(self, arg1: Image) -> None:
        pass
    @property
    def tint(self) -> Color:
        """
        :type: Color
        """
    @tint.setter
    def tint(self, arg1: Color) -> None:
        pass
    pass
class Text(RenderableGameObject, ShapedGameObject, GameObject, EventListener):
    def __init__(self, parent: GameObject, name: str) -> None: ...
    @typing.overload
    def set_font_by_name(self, fontfile: str, imgfile: str) -> None: ...
    @typing.overload
    def set_font_by_name(self, name: str) -> None: ...
    def set_style(self, **kwargs) -> None: ...
    @property
    def color(self) -> Color:
        """
        :type: Color
        """
    @color.setter
    def color(self, arg1: Color) -> None:
        pass
    @property
    def content(self) -> str:
        """
        :type: str
        """
    @content.setter
    def content(self, arg1: str) -> None:
        pass
    @property
    def font(self) -> Font:
        """
        :type: Font
        """
    @font.setter
    def font(self, arg1: Font) -> None:
        pass
    @property
    def is_bold(self) -> bool:
        """
        :type: bool
        """
    @is_bold.setter
    def is_bold(self, arg1: bool) -> None:
        pass
    @property
    def is_italic(self) -> bool:
        """
        :type: bool
        """
    @is_italic.setter
    def is_italic(self, arg1: bool) -> None:
        pass
    @property
    def is_underlined(self) -> bool:
        """
        :type: bool
        """
    @is_underlined.setter
    def is_underlined(self, arg1: bool) -> None:
        pass
    @property
    def size(self) -> float:
        """
        :type: float
        """
    @size.setter
    def size(self, arg1: float) -> None:
        pass
    @property
    def style(self) -> int:
        """
        :type: int
        """
    @style.setter
    def style(self, arg1: int) -> None:
        pass
    pass
class TileMap(AxisGameObject, RenderableGameObject, ShapedGameObject, GameObject, EventListener):
    class IntPoint():
        def __eq__(self, arg0: TileMap.IntPoint) -> bool: ...
        def __init__(self, x: int, y: int) -> None: ...
        @staticmethod
        def div_by_chunk_size(a: int) -> int: ...
        @staticmethod
        def mod_by_chunk_size(a: int) -> int: ...
        def to_chunk(self) -> TileMap.IntPoint: ...
        def to_rel_ind(self) -> int: ...
        def to_rel_pos(self) -> TileMap.IntPoint: ...
        @property
        def x(self) -> int:
            """
            :type: int
            """
        @x.setter
        def x(self, arg0: int) -> None:
            pass
        @property
        def y(self) -> int:
            """
            :type: int
            """
        @y.setter
        def y(self, arg0: int) -> None:
            pass
        __hash__ = None
        pass
    def __init__(self, parent: GameObject, name: str) -> None: ...
    def clear_tile(self, coord: TileMap.IntPoint) -> None: ...
    def get_tile(self, coord: TileMap.IntPoint) -> int: ...
    def set_tile(self, coord: TileMap.IntPoint, value: int) -> None: ...
    def set_tile_set_from_path(self, path: str) -> None: ...
    def to_tile_coord(self, pos: Point) -> TileMap.IntPoint: ...
    @property
    def tile_set(self) -> TileSet:
        """
        :type: TileSet
        """
    @tile_set.setter
    def tile_set(self, arg1: TileSet) -> None:
        pass
    @property
    def tint(self) -> Color:
        """
        :type: Color
        """
    @tint.setter
    def tint(self, arg1: Color) -> None:
        pass
    pass
class TileSet(Resource):
    def __repr__(self) -> str: ...
    @staticmethod
    def render(*args, **kwargs) -> typing.Any: ...
    @property
    def atlas(self) -> Image:
        """
        :type: Image
        """
    @property
    def frame_cols(self) -> int:
        """
        :type: int
        """
    @property
    def frame_height(self) -> int:
        """
        :type: int
        """
    @property
    def frame_rows(self) -> int:
        """
        :type: int
        """
    @property
    def frame_width(self) -> int:
        """
        :type: int
        """
    @property
    def name(self) -> str:
        """
        :type: str
        """
    @property
    def noncopy_cast_address(self) -> int:
        """
        :type: int
        """
    @property
    def num_of_tiles(self) -> int:
        """
        :type: int
        """
    pass
class Transform():
    def __add__(self, arg0: Transform) -> Transform: ...
    def __eq__(self, arg0: Transform) -> bool: ...
    @staticmethod
    def __init__(*args, **kwargs) -> typing.Any: ...
    def __ne__(self, arg0: Transform) -> bool: ...
    def __repr__(self) -> str: ...
    def substract_first(self, other: Transform) -> Transform: ...
    def substract_second(self, other: Transform) -> Transform: ...
    @property
    def pos(self) -> Point:
        """
        :type: Point
        """
    @pos.setter
    def pos(self, arg0: Point) -> None:
        pass
    @property
    def rot(self) -> float:
        """
        :type: float
        """
    @rot.setter
    def rot(self, arg0: float) -> None:
        pass
    @property
    def scale(self) -> Point:
        """
        :type: Point
        """
    @scale.setter
    def scale(self, arg0: Point) -> None:
        pass
    __hash__ = None
    pass
class WindowClosedEvent(Event):
    def __init__(self) -> None: ...
    pass
class WindowResizedEvent(Event):
    def __init__(self, new_width: int, new_height: int) -> None: ...
    @property
    def new_height(self) -> int:
        """
        :type: int
        """
    @property
    def new_width(self) -> int:
        """
        :type: int
        """
    pass
