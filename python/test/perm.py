#!/usr/bin/env python
#!encoding=utf8

'''
File: perm.py
Author: Zenki (Zenki.J.Zha), zenki2001cn@163.com
Description: 全排列算法
Version:
Last Modified: 七月 07, 2014
'''

def perm(src, count, head):
    result = []

    if count == 1:
        for j in src:
            head_copy = head[:]
            head_copy.append(j)
            head_copy.sort()
            result.append(''.join(head_copy))
        return result

    for i in src:
        b = src[:]
        b.remove(i)
        hl = list(head)
        hl.append(i)
        result.extend(perm(b, count-1, hl))

    result = list(set(result))
    result.sort()
    return result

def perm0():
    a = ['0', '1', '2', '3', '4', '5', '6', '7']

    d = []
    inc = 2
    for i in a:
        b = a[:]
        b.remove(i)
        for j in b:
            c = a[:]
            c.remove(i)
            c.remove(j)
            if inc == 2:
                tmp = [i, j]
                tmp.sort()
                d.append(''.join(tmp))
            elif inc == 3:
                for k in c:
                    tmp = [i, j, k]
                    tmp.sort()
                    d.append(''.join(tmp))
            elif inc == 4:
                for k in c:
                    e = c[:]
                    e.remove(k)
                    for l in e:
                        tmp = [i, j, k, l]
                        tmp.sort()
                        d.append(''.join(tmp))


    d = list(set(d))
    d.sort()
    print d

def perm1():
    """去除重复排列"""
    a = ['0', '1', '2', '3', '4', '5', '6', '7']

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
    print '递归遍历'
    a = ['0', '1', '2', '3', '4', '5', '6', '7']
    res = perm(a, 2, '')
    print res

    print '手动遍历'
    perm0()

    print '去除重复2'
    perm1()

    print '遍历一次'
    perm2()

    print '完整排列'
    perm3(a)
