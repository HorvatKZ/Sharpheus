import Engine as sph

def render():
    sph.Renderer.draw_circle([
            sph.Point(-100, -100),
            sph.Point(100, -100),
            sph.Point(100, 100),
            sph.Point(-100, 100)
        ], sph.Color(240, 240, 255))