import Engine as sph
from Engine import Logger as sphl
import functools


class MenuCtrl(sph.PythonBehavior):

	def __init__(self, parent: sph.GameObject, name: str):
		sph.PythonBehavior.__init__(self, parent, name)

	def sph_init(self):
		self.main_menu = self.level.get_gameobject('MainMenu')
		self.level_select = self.level.get_gameobject('LevelSelect')
		self.subscribe_control_changed(self.level.get_gameobject('PlayBtn'), self.play_pressed)
		self.subscribe_control_changed(self.level.get_gameobject('ExitBtn'), self.exit_pressed)
		self.subscribe_control_changed(self.level.get_gameobject('BackBtn'), self.back_pressed)
		for i in range(1, 4):
			self.subscribe_control_changed(self.level.get_gameobject(f'Level{i}Btn'), functools.partial(self.level_pressed, f'Level{i}'))
		# Using simply lambda e: self.level_pressed(f'Level{i}', e) the lambdas are evaluated at call, and all f'Level{i}' will be evaluated as 'Level3'

	def play_pressed(self, e: sph.ControlChangedEvent):
		self.main_menu.is_visible = False
		self.level_select.is_visible = True

	def exit_pressed(self, e: sph.ControlChangedEvent):
		sph.ProjectControl.exit()

	def back_pressed(self, e: sph.ControlChangedEvent):
		self.main_menu.is_visible = True
		self.level_select.is_visible = False
	
	def level_pressed(self, level_name: str, e: sph.ControlChangedEvent):
		if e.source.is_clicked:
			sph.ProjectControl.load_level(level_name + '.lvl.sharpheus')

#	def tick(self, delta_time: float):
#		Called once every tick


def create(parent: sph.GameObject, name: str):
	return MenuCtrl(parent, name)

#def provide_list() -> list[str]:
#	Return list of providable argument names
