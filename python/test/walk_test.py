import os

path = '/home/zenki/python'
arg = 'arg'

def visit(arg, dirname, names):
    print arg, dirname, names

os.path.walk(path, visit, arg)
