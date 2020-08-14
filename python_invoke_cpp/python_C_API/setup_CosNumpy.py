from distutils.core import setup, Extension
import numpy

module1 = Extension('CosNumpy',
             # define_macros = [('MAJOR_VERSION', '1'), ('MINOR_VERSION', '0')],
             include_dirs = [numpy.get_include()],
             # libraries = [''],
             # library_dirs = ['/usr/local/lib'],
             sources = ['CosNumpy.c'])

setup (name = 'CosNumpy',
   version = '1.0',
   description = 'This is a cos_numpy package',
   author = 'Martin v. Loewis',
   author_email = 'martin@v.loewis.de',
   url = 'https://docs.python.org/extending/building',
   long_description = '''This is really just a cos_numpy package.''',
   ext_modules = [module1])
