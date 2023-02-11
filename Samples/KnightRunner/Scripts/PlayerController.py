import Engine as sph
#from Engine import Logger as sphl


class PlayerController(sph.PythonBehavior):

	def __init__(self, parent: sph.GameObject, name: str):
		sph.PythonBehavior.__init__(self, parent, name)
		self.jump_force = 500
		self.speed = 300
		self.max_hp = 100
		self.dmg = 25
		self.hp = 100
		self.attacks = False
		self.can_jump = False
		self.attack_timer = 0

	def sph_init(self):
		self.hp = self.max_hp
		self.lower_anim = self.parent.get_child('PlayerLowerAnim')
		self.upper_anim = self.parent.get_child('PlayerUpperAnim')
		self.default_anim_scale = self.lower_anim.trafo_scale_x

		self.subscribe_collision(self.parent.get_first_child_of_mask(sph.GameObject.TypeMasks.Collider), self.on_collision)

		self.subscribe_key_pressed(self.key_pressed)
		self.subscribe_key_hold(self.key_hold)
		self.subscribe_key_released(self.key_released)

	def take_dmg(self, dmg: int):
		self.hp -= dmg
	
	def heal(self, amount: int):
		self.hp += amount
		if self.hp > self.max_hp:
			self.hp = self.max_hp
	
	def is_attacking(self) -> bool:
		if 0 < self.attack_timer and self.attack_timer < 0.3:
			result = self.attacks
			self.attacks = False
			return result
		return False

	def on_collision(self, e: sph.CollisionEvent):
		if e.cd.geom.normal.get_angle_with(sph.Point.DOWN) < 10:
			self.can_jump = True
	
	def key_pressed(self, e: sph.KeyPressedEvent):
		self.move(e.code)

	def key_hold(self, e: sph.KeyHoldEvent):
		self.move(e.code)

	def key_released(self, e: sph.KeyReleasedEvent):
		match e.code:
			case sph.KeyCode.A:
				self.parent.velocity.x = 0
				self.play_anim('Idle', 0)
			case sph.KeyCode.D:
				self.parent.velocity.x = 0
				self.play_anim('Idle', 0)

	def move(self, code: sph.KeyCode):
		match code:
			case sph.KeyCode.W:
				if self.can_jump:
					self.parent.velocity.y = -self.jump_force
					self.can_jump = False
			case sph.KeyCode.A:
				self.parent.velocity.x = -self.speed
				self.play_anim('Run', -1)
			case sph.KeyCode.D:
				self.parent.velocity.x = self.speed
				self.play_anim('Run', 1)
			case sph.KeyCode.K:
				if self.upper_anim.current_name != 'Attack_Up':
					self.upper_anim.play_once('Attack_Up')
					self.attacks = True
					self.attack_timer = 0.6
	
	def play_anim(self, anim_name: str, scale_mult: float):
		if scale_mult != 0:
			self.lower_anim.trafo_scale_x = scale_mult * self.default_anim_scale
			self.upper_anim.trafo_scale_x = scale_mult * self.default_anim_scale
		if self.upper_anim.current_name != anim_name + '_Up':
			self.upper_anim.current_name = anim_name + '_Up'
		if self.lower_anim.current_name != anim_name + '_Down':
			self.lower_anim.current_name = anim_name + '_Down'

	def tick(self, delta_time: float):
		if self.attacks:
			self.attack_timer -= delta_time
		if self.world_trafo_pos_y > 500:
			self.hp = 0


def create(parent: sph.GameObject, name: str):
	return PlayerController(parent, name)

def provide_list():
    return ['jump_force', 'speed', 'max_hp', 'dmg',]
