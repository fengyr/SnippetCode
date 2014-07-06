#!/usr/bin/env python
#!encoding=utf8

'''
File: perm.py
Author: Zenki (Zenki.J.Zha), zenki2001cn@163.com
Description: 全排列算法
Version:
Last Modified: 七月 07, 2014
'''

def perm1():
    """去除重复排列"""
    # a = ['1', '2', '3', '4', '5', '6', '7', '8', '9', '10']
    a = ['1', '2', '3']

    # n-1
    inc = 2
    for i in a:
        # b = a[:]
        b = a
        b.remove(i)
        for j in range(0, len(b)-inc+1):
            c = b[j:j+inc]
            # c.append(i)
            c.insert(0, i)
            print c

def perm2():
    """遍历一次"""
    # a = ['1', '2', '3', '4', '5', '6', '7', '8', '9', '10']
    a = ['1', '2', '3']

    # n-1
    inc = 2
    for i in a:
        b = a[:]
        b.remove(i)
        for j in range(0, len(b)-inc+1):
            c = b[j:j+inc]
            # c.append(i)
            c.insert(0, i)
            print c

def perm3(l):
    """完整排列"""

    if (len(l) <= 1):
        return [l]

    r = []
    for i in range(len(l)):
        s = l[:i] + l[i+1:]
        p = perm3(s)
        for x in p:
            r.append(l[i:i+1]+x)

    return r

if __name__ == '__main__':
    print '去除重复'
    perm1()
    print '遍历一次'
    perm2()
    print '完整排列'
    a = ['1', '2', '3']
    perm3(a)
