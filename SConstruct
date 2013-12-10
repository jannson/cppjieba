import os, sys

ignores = ['server.cpp','segment.cpp']
sources = ['cppjiebapy/mixsegment.i','cppjiebapy/mixsegment.cpp']
paths = ['./src', './src/Limonp', './src/Husky']
for p in paths:
	for d in os.listdir(p):
		f = os.path.join(p, d)
			if os.path.isfile(f) and d.endswith(".cpp") and d != 'server.cpp':
								            sources.append(f)

env = DefaultEnvironment(variant_dir='buildcpp')
objs = []
for d in src_dirs:
	objs += list(Glob(d+'/*.cpp'))
env.Append(CPPPATH=src_dirs)
env.SharedLibrary('cppjieba', list(objs))

