from _internal import execute as _e

def script(author, mt=False, v='0.1'):
	''' Declares a library extension script. '''
	def decorator(f):
		print 'registering function', f.func_name
		print '  author:', author
		print '  version:', v
		print '  multithreaded:', mt
		
		from inspect import getargspec
		argspec = getargspec(f)
		
		args = {}
		if argspec.args:
			if not argspec.defaults or len(argspec.args) is not len(argspec.defaults):
				raise TypeError('a script function should have default values for all arguments')
			args = dict(zip(argspec.args, argspec.defaults))
		print '  arguments:', args
		
		return f
	return decorator
	
def execute(name, **kwargs):
	return _e(name, kwargs)
