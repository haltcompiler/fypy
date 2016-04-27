from distutils.core import setup, Extension

module1 = Extension('containerblock',
			libraries = ['ssl', 'crypto'],
			sources = ['containerblockmodule.c', 'containers.c'])

setup(name = 'containerblock', 
		version = '1.0',
		description = 'Retrieves credentials from a secure container block',
		author = 'yinsain',
		url = 'https://www.github.com',
		long_description = '''
			Retrieves credentials from a secure container block
		''',
		ext_modules = [module1])