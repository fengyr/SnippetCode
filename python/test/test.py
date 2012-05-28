import sys, os
import types

def func_a(a):
    c = 0
    while a > 0:
        c += 1
        a = a & (a-1)
        print a

    return c

print func_a(0x270f)
