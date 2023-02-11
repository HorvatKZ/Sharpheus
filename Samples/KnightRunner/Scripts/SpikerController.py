import Engine as sph
from Engine import Logger as sphl


class SpikerController(sph.PythonBehavior):

	def __init__(self, parent: sph.GameObject, name: str):
		sph.PythonBehavior.__init__(self, parent, name)
		self.attack_speed = 1
		self.max_hp = 100
		self.dmg = 0
		self.speed = 0
		self.range = 0
		self.hp = 100
		self.attack_loop_state = 0
		self.is_player_in = False
		self.player_can_dmg = False
		self.curr_place_in_range = 0
		self.moves_right = True

	def sph_init(self):
		self.hp = self.max_hp
		self.player_controller = self.level.get_gameobject('PlayerController')
		self.hp_bar = self.parent.get_first_child_of_type(sph.GameObject.Type.Quad)
		self.subscribe_trigger_enter(self.parent.get_first_child_of_mask(sph.GameObject.TypeMasks.Collider), self.on_damage_trigger_enter)
		self.subscribe_trigger_exit(self.parent.get_first_child_of_mask(sph.GameObject.TypeMasks.Collider), self.on_damage_trigger_exit)
		self.subscribe_trigger_enter(self.parent.get_last_child_of_mask(sph.GameObject.TypeMasks.Collider), self.on_vulnerability_trigger_enter)
		self.subscribe_trigger_exit(self.parent.get_last_child_of_mask(sph.GameObject.TypeMasks.Collider), self.on_vulnerability_trigger_exit)

	def take_dmg(self, dmg: int):
		self.hp -= dmg
		if self.hp <= 0:
			self.level.delete(self.parent)
		# Resize hp bar
		ratio = self.hp / self.max_hp
		bar_width = self.hp_bar.width
		self.hp_bar.trafo = sph.Transform(sph.Point(-bar_width / 2 * (1 - ratio), self.hp_bar.trafo_pos_y), sph.Point(ratio, 1), 0)

	def tick(self, delta_time: float):
		if self.is_player_in:
			self.attack_loop_state += delta_time
			if self.attack_loop_state > self.attack_speed:
				self.attack_loop_state -= self.attack_speed
				self.player_controller.take_dmg(self.dmg)
		if self.player_can_dmg and self.player_controller.is_attacking():
			self.take_dmg(self.player_controller.dmg)
		# Movement
		movement = self.speed * delta_time
		if not self.moves_right:
			movement *= -1
		self.curr_place_in_range += movement
		self.parent.trafo_pos_x += movement
		if self.curr_place_in_range <= 0:
			self.moves_right = True
		elif self.curr_place_in_range >= self.range:
			self.moves_right = False

	def on_damage_trigger_enter(self, e: sph.OnEnterEvent):
		if e.obj == self.player_controller:
			self.player_controller.take_dmg(self.dmg)
			self.attack_loop_state = 0
			self.is_player_in = True
	
	def on_damage_trigger_exit(self, e: sph.OnExitEvent):
		if e.obj == self.player_controller:
			self.is_player_in = False
	
	def on_vulnerability_trigger_enter(self, e: sph.OnEnterEvent):
		if e.obj == self.player_controller:
			self.player_can_dmg = True
	
	def on_vulnerability_trigger_exit(self, e: sph.OnExitEvent):
		if e.obj == self.player_controller:
			self.player_can_dmg = False


def create(parent: sph.GameObject, name: str):
	return SpikerController(parent, name)

def provide_list():
    return ['max_hp', 'dmg', 'attack_speed', 'speed', 'range']
