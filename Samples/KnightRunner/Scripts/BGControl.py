import Engine as sph
#from Engine import Logger as sphl


class BGControl(sph.PythonBehavior):

	def __init__(self, parent: sph.GameObject, name: str):
		sph.PythonBehavior.__init__(self, parent, name)
		self.move_scale = 0.5

	def sph_init(self):
		self.player = self.level.get_gameobject('Player')
		self.main_bg = self.parent.get_child('MainBG')
		self.moving_bg = self.parent.get_child('MovingBG')
		self.bg_unit = self.moving_bg.get_child(0).image.width * self.moving_bg.get_child(0).world_trafo_scale_x
		self.prev_player_pos_x = self.player.trafo_pos_x

	def tick(self, delta_time: float):
		self.main_bg.trafo_pos_x = self.player.trafo_pos_x
		self.moving_bg.trafo_pos_x += (self.player.trafo_pos_x - self.prev_player_pos_x) * self.move_scale
		self.prev_player_pos_x = self.player.trafo_pos_x

		diff = self.moving_bg.trafo_pos_x - self.player.trafo_pos_x
		if diff > self.bg_unit // 2:
			self.moving_bg.trafo_pos_x -= self.bg_unit
		elif diff < self.bg_unit // -2:
			self.moving_bg.trafo_pos_x += self.bg_unit


def create(parent: sph.GameObject, name: str):
	return BGControl(parent, name)

def provide_list() -> list[str]:
	return ['move_scale']
