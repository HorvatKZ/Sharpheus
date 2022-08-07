import sys
sys.path.insert(0, 'D:\\Programming\\Sharpheus\\bin\\Debug')
import Engine as sph


sph.Renderer.draw_circle([
        sph.Point(-100, -100),
        sph.Point(100, -100),
        sph.Point(100, 100),
        sph.Point(-100, 100)
    ], sph.Color(240, 240, 255))