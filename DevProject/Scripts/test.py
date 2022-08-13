import Engine as sph
from Engine import Logger as sphl

image = sph.ResourceManager.get_image('knight.png', False)
fnt = sph.ResourceManager.get_font('Fonts/arial.fnt', 'Fonts/arial.png')
music = sph.ResourceManager.get_audio('Sounds/menu.mp3')

render_layer = 'HUD'
def render():
    sph.Renderer.draw_circle([
            sph.Point(-100, -100),
            sph.Point(100, -100),
            sph.Point(100, 100),
            sph.Point(-100, 100)
        ], sph.Color(240, 240, 255))
    image.render([
        sph.Point(-64, -64),
        sph.Point(64, -64),
        sph.Point(64, 64),
        sph.Point(-64, 64)
    ])
    fnt.render('test1', sph.Point(0, -100), 20)
    fnt.render('test2', sph.Point(0, 0), 50, sph.Color.YELLOW, bold=True)
    fnt.render('test3', sph.Point(0, 100), 30, sph.Color.RED, sph.Point.LEFT, sph.Point.UP, bold=False, italic=True, underlined=True)

def tick(delta_time: float):
    tick.all_time += delta_time
    if tick.is_first:
        tick.handle = sph.AudioPlayer.play(music)
        tick.is_first = False
    if tick.all_time > 5:
        sph.AudioPlayer.stop(tick.handle)

tick.is_first = True
tick.all_time = 0
tick.handle = 0
