#!/usr/bin/env python
#!encoding=utf8

def foo():
    """foo定义的局部变量"""
    foo_a = 100

    def bar():
        """bar定义的局部变量"""
        foo_a = foo_a + 101
        print foo_a

    bar()
    print foo_a

foo()
