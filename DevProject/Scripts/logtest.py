import Engine as sph
from Engine import Logger as sphl


class logtest(sph.PythonBehavior):

	def __init__(self, parent: sph.GameObject, name: str):
		sph.PythonBehavior.__init__(self, parent, name)

	def sph_init(self):
		sphl.log('Its a simple log')
		sphl.info('Its an info log')
		sphl.warn('Its an warning')
		sphl.error('Its an error')
		sphl.fatal('Its a fatal error')

#	def tick(self, delta_time: float):
#		Called once every tick


def create(parent: sph.GameObject, name: str):
	return logtest(parent, name)

#def provide_list() -> list[str]:
#	Return list of providable argument names
