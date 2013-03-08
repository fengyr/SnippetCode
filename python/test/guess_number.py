#!/usr/bin/env python
#!encoding=utf8

import sys

if __name__ == '__main__':
    for i in sys.argv:
        print i

runing = True
num = 20

def guess_num(num, runing):
    '''This is a guess game.

    it's ok.'''
    while runing:
        i = int(raw_input("Enter a num:"))
        if i == num:
            print "correct"
            runing = False
        elif i < num:
            print "less"
        else:
            print "large"

    print "done"

if __name__ == '__main__':
    guess_num(num, runing)
    print guess_num.__doc__
    print dir(sys)
