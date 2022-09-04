import Engine as sph
from Engine import Logger as sphl


class LevelTest(sph.PythonBehavior):

    def __init__(self, parent: sph.GameObject, name: str):
        sph.PythonBehavior.__init__(self, parent, name)

    def sph_init(self):
        sphl.log(f'LevelTest\'s level name: "{self.level.name}"')


def create(parent: sph.GameObject, name: str):
    return LevelTest(parent, name)
