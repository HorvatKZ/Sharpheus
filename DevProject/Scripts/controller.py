import Engine as sph
from Engine import Logger as sphl


class PlayerController(sph.PythonBehavior):

    def __init__(self, parent: sph.GameObject, name: str):
        sph.PythonBehavior.__init__(self, parent, name)
        self.can_jump = False
        self.jump_force = 500
        self.speed = 300
    
    def sph_init(self):
        self.subscribe_key_pressed(self.key_pressed)
        self.subscribe_key_hold(self.key_hold)
        self.subscribe_key_released(self.key_released)
        self.subscribe_collision(self.parent.parent.get_first_child_of_type(sph.GameObject.Type.CapsuleCollider), self.on_collision)
        self.anim = self.parent.parent.get_child('PlayerAnim')
        self.default_sprite_scale = self.anim.trafo.scale.x

    def key_pressed(self, e: sph.KeyPressedEvent):
        match e.code:
            case sph.KeyCode.W:
                if self.can_jump:
                    self.parent.parent.velocity.y = -self.jump_force
                    self.can_jump = False
            case sph.KeyCode.A:
                self.parent.parent.velocity.x = -self.speed
                self.anim.trafo.scale.x = -self.default_sprite_scale
                self.anim.current_name = 'Walk'
            case sph.KeyCode.D:
                self.parent.parent.velocity.x = self.speed
                self.anim.trafo.scale.x = self.default_sprite_scale
                self.anim.current_name = 'Walk'

    def key_hold(self, e: sph.KeyHoldEvent):
        match e.code:
            case sph.KeyCode.A:
                self.parent.parent.velocity.x = -self.speed
            case sph.KeyCode.D:
                self.parent.parent.velocity.x = self.speed

    def key_released(self, e: sph.KeyReleasedEvent):
        match e.code:
            case sph.KeyCode.A:
                self.parent.parent.velocity.x = 0
                self.anim.current_name = 'Idle'
            case sph.KeyCode.D:
                self.parent.parent.velocity.x = 0
                self.anim.current_name = 'Idle'

    def on_collision(self, e: sph.CollisionEvent):
        if e.cd.geom.normal.get_angle_with(sph.Point.DOWN) < 10:
            self.can_jump = True


def create(parent: sph.GameObject, name: str):
    return PlayerController(parent, name)

def provide_list():
    return ['jump_force', 'speed']
