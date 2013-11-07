from distutils.core import setup, Extension

setup(name='mixsegment',
        version='0.1.0',
        description='segment interrept with c++',
        author='yuzheng',
        author_email='gandancing@gmail.com',
        packages=['.'],
        ext_modules=[Extension('_mixsegment',
                ['mixsegment.i'],
                language='c++',
                swig_opts=['-c++']),
            ],
        )
