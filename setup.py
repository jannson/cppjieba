import os
from distutils.core import setup, Extension

ignores = ['server.cpp','segment.cpp']
sources = ['cppjiebapy/mixsegment.i','cppjiebapy/mixsegment.cpp']
paths = ['./src', './src/Limonp', './src/Husky']

for p in paths:
    for d in os.listdir(p):
        f = os.path.join(p, d)
        if os.path.isfile(f) and d.endswith(".cpp") and d not in ignores:
            sources.append(f)

module_jieba = Extension('_mixsegment',
                sources,
                language='c++',
                swig_opts=['-c++'],
                #define_macros = [('TEST', 1)],
                #extra_compile_args=['-static'],
                #extra_link_args=['-static'],
                #include_dirs=['/usr/include/CppJieba', 'cppjieba'],
                #libraries=['cppjieba'],
                #library_dirs=['/usr/lib/CppJieba']
                )

setup(name='cppjiebapy',
        version='0.1.0',
        description='segment interrept with c++',
        author='yuzheng',
        author_email='gandancing@gmail.com',
        packages=['cppjiebapy'],
        package_data={'cppjiebapy':['*.py','*.i','*.cpp','*.cxx','dict/*']},
        ext_modules=[module_jieba],
        )
