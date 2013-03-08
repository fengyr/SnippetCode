#!/usr/bin/env python
#!encoding=utf8

import curses

scr = curses.initscr()

#scr.noecho()
scr.cbreak()
scr.keypad(1)
scr.endwin()
