import Engine as sph
#from Engine import Logger as sphl


class CameraController(sph.PythonBehavior):

	def __init__(self, parent: sph.GameObject, name: str):
		sph.PythonBehavior.__init__(self, parent, name)

	def sph_init(self):
		self.player = self.level.get_gameobject('Player')

	def tick(self, delta_time: float):
		self.parent.world_trafo_pos_x = self.player.world_trafo_pos_x


def create(parent: sph.GameObject, name: str):
	return CameraController(parent, name)

#def provide_list() -> list[str]:
#	Return list of providable argument names
