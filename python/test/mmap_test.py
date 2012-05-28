import mmap
import os

map = mmap.mmap(-1, 64)
map.write('hello world')

map.seek(0)
print map.readline()
map.close()

f = open('test1.py', 'r+')
map = mmap.mmap(f.fileno(), 24)
print map.readline()

