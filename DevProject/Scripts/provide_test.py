import Engine as sph
from Engine import Logger as sphl


class ProvideTestClass(sph.PythonBehavior):

    def __init__(self, parent: sph.GameObject, name: str):
        sph.PythonBehavior.__init__(self, parent, name)
        self.int_ = 12
        self.float_ = 34.56
        self.bool_ = True
        self.str_ = 'check this'
        self.point_ = sph.Point(7, 8)
        self.color_ = sph.Color.BLUE
        self.trafo_ = sph.Transform(sph.Point(1, 2), sph.Point(3, 4), 5)
        self.image_ = sph.ResourceManager.get_image('knight.png', False)
        self.font_ = sph.ResourceManager.get_font('Fonts/arial.fnt', 'Fonts/arial.png')
        self.anim_ = sph.ResourceManager.get_animation('Walk.anim.sharpheus')
        self.tiles_ = sph.ResourceManager.get_tileset('mc.tile.sharpheus')
        self.audio_ = sph.ResourceManager.get_audio('Sounds/menu.mp3')


def create(parent: sph.GameObject, name: str):
    return ProvideTestClass(parent, name)

def provide_list() -> list[str]:
    return ['int_', 'float_', 'bool_', 'str_', 'point_', 'color_', 'trafo_', 'image_', 'font_', 'anim_', 'tiles_', 'audio_']