import Engine as sph
from Engine import Logger as sphl


class TestClass(sph.PythonBehavior):

    def __init__(self, parent: sph.GameObject, name: str):
        sph.PythonBehavior.__init__(self, parent, name)
        self.image = sph.ResourceManager.get_image('knight.png', False)
        self.fnt = sph.ResourceManager.get_font('Fonts/arial.fnt', 'Fonts/arial.png')
        self.music = sph.ResourceManager.get_audio('Sounds/menu.mp3')
        self.is_first = True
        self.all_time = 0
        self.handle = 0

    def __del__(self):
        if self.handle != 0:
            sph.AudioPlayer.stop(self.handle)
    
    def sph_init(self):
        self.subscribe_for_render('HUD', self.render)

    def render(self):
        sph.Renderer.draw_circle([
                sph.Point(-100, -100),
                sph.Point(100, -100),
                sph.Point(100, 100),
                sph.Point(-100, 100)
            ], sph.Color(240, 240, 255))
        self.image.render([
            sph.Point(-64, -64),
            sph.Point(64, -64),
            sph.Point(64, 64),
            sph.Point(-64, 64)
        ])
        self.fnt.render('test1', sph.Point(0, -100), 20)
        self.fnt.render('test2', sph.Point(0, 0), 50, sph.Color.YELLOW, bold=True)
        self.fnt.render('test3', sph.Point(0, 100), 30, sph.Color.RED, sph.Point.LEFT, sph.Point.UP, bold=False, italic=True, underlined=True)

    def tick(self, delta_time: float):
        self.all_time += delta_time
        if self.is_first:
            self.handle = sph.AudioPlayer.play(self.music)
            self.is_first = False
        if self.all_time > 5:
            sph.AudioPlayer.stop(self.handle)
            self.handle = 0


def create(parent: sph.GameObject, name: str):
    return TestClass(parent, name)
