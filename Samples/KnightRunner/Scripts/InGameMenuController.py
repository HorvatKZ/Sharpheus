import Engine as sph
from Engine import Logger as sphl


class InGameMenuController(sph.PythonBehavior):

	def __init__(self, parent: sph.GameObject, name: str):
		sph.PythonBehavior.__init__(self, parent, name)

	def sph_init(self):
		self.subscribe_buttons(self.parent)
		self.subscribe_control_changed(self.parent.get_child('PauseBtn'), self.on_pause)
		self.subscribe_control_changed(self.parent.get_child('MusicBtn'), self.on_music)
		self.music_player = self.level.get_gameobject('Music')
		self.player_controller = self.level.get_gameobject('PlayerController')
	
	def subscribe_buttons(self, obj: sph.GameObject):
		for child in obj.children:
			if child.type == sph.GameObject.Type.ImageButton:
				self.subscribe_image_button(child)
			self.subscribe_buttons(child)
	
	def subscribe_image_button(self, imgbut: sph.ImageButton):
		match imgbut.text:
			case 'Retry':
				self.subscribe_control_changed(imgbut, self.on_retry)
			case 'Back to Menu':
				self.subscribe_control_changed(imgbut, self.on_back_to_menu)
	
	def on_retry(self, e: sph.ControlChangedEvent):
		sph.ProjectControl.load_level(self.level.path)
	
	def on_back_to_menu(self, e: sph.ControlChangedEvent):
		sph.ProjectControl.load_level('Menu.lvl.sharpheus')

	def on_pause(self, e: sph.ControlChangedEvent):
		if e.source.is_checked:
			self.level.get_gameobject('PauseBg').is_visible = True
			sph.ProjectControl.pause()
		else:
			self.level.get_gameobject('PauseBg').is_visible = False
			sph.ProjectControl.resume()

	def on_music(self, e: sph.ControlChangedEvent):
		if e.source.is_checked:
			self.music_player.pause()
		else:
			self.music_player.resume()

	def tick(self, delta_time: float):
		if self.player_controller.hp <= 0:
			self.level.get_gameobject('GameOverBg').is_visible = True
			sph.ProjectControl.pause()


def create(parent: sph.GameObject, name: str):
	return InGameMenuController(parent, name)

#def provide_list() -> list[str]:
#	Return list of providable argument names
