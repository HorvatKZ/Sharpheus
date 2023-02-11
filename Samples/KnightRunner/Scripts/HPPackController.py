import Engine as sph
#from Engine import Logger as sphl


class HPPackController(sph.PythonBehavior):

	def __init__(self, parent: sph.GameObject, name: str):
		sph.PythonBehavior.__init__(self, parent, name)
		self.amount = 10

	def sph_init(self):
		self.player_controller = self.level.get_gameobject('PlayerController')
		self.subscribe_trigger_enter(self.parent.get_first_child_of_mask(sph.GameObject.TypeMasks.Collider), self.player_entered)

	def player_entered(self, e: sph.OnEnterEvent):
		if e.obj == self.player_controller:
			self.player_controller.heal(self.amount)
			self.level.delete(self.parent)

#	def tick(self, delta_time: float):
#		Called once every tick


def create(parent: sph.GameObject, name: str):
	return HPPackController(parent, name)

def provide_list() -> list[str]:
	return ['amount']
