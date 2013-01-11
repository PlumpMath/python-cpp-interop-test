from library import script, execute

@script('John Doe', True)
def foo(x=5, y=8, s='dog'):
	return {'status': 'i\'m doin\' fine', 'a': x+y, 'b': s+'shit'}

@script('Jane Doe', False, '1.0')
def bar():
	execute('foo', y=10)
