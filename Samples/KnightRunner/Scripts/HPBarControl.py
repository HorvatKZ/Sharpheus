import Engine as sph
#from Engine import Logger as sphl


class HPBarControl(sph.PythonBehavior):

	def __init__(self, parent: sph.GameObject, name: str):
		sph.PythonBehavior.__init__(self, parent, name)

	def sph_init(self):
		self.player_controller = self.level.get_gameobject('PlayerController')
		self.prev_hp = 0
		self.start_pos_x = self.parent.trafo_pos_x

		self.hp_bar_none = self.parent.get_child('HPBarNone')
		self.hp_bar_full = self.parent.get_child('HPBarFull')

	def tick(self, delta_time: float):
		if self.player_controller.hp != self.prev_hp:
			self.resize(self.player_controller.hp, self.player_controller.max_hp)
			self.prev_hp = self.player_controller.hp
		self.parent.trafo_pos_x = self.start_pos_x + self.player_controller.parent.trafo_pos_x

	def resize(self, hp: int, max_hp: int):
		self.hp_bar_full.width = self.hp_bar_none.width * hp / max_hp
		self.hp_bar_full.trafo_pos_x = -(self.hp_bar_none.width - self.hp_bar_full.width) / 2


def create(parent: sph.GameObject, name: str):
	return HPBarControl(parent, name)

#def provide_list() -> list[str]:
#	Return list of providable argument names
