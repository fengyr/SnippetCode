#!/usr/bin/env python
#!encoding=utf8

import struct

"""
十进制转二进制
"""
def d2b(num, limit=5):
    """
    整数部分
    """
    inter = int(num)
    deci = num - inter

    res = []
    m, n = divmod(inter, 2)
    res.append(str(n))

    while m != 0:
        m, n = divmod(m, 2)
        res.insert(0, str(n))

    """
    小数部分
    """
    res2 = []
    m = deci
    n = 0

    while len(res2) < limit:
        m = m * 2
        n = int(m)
        m = m - n
        res2.append(str(n))
        if (m - n) == 0:
            break

    return ''.join(res) + '.' + ''.join(res2)

"""
二进制转十进制
"""
def b2d(binary):
    m, n = ('%f' % binary).split('.')

    res = 0
    ml = enumerate([i for i in m][::-1])
    for id, num in ml:
        res = res + int(num) * (2 ** id)

    res2 = 0
    nl = enumerate([i for i in n])
    for id, num in nl:
        res2 = res2 + int(num) * (2 ** (0 - id - 1))

    return str(res + res2)

"""
二进制转八进制
"""
def b2o(binary):
    m, n = ('%f' % binary).split('.')

    res = []
    ml = len(m)
    if ml % 3 != 0:
        mlt = (ml / 3) + 1
        ml = mlt * 3

    m = m.rjust(ml, '0')
    start = 0
    for i in range(len(m) / 3):
        res.append(str(int(float(b2d(int(m[start:start + 3]))))))
        start = start + 3

    res2 = []
    nl = len(n)
    if nl % 3 != 0:
        nlt = (nl / 3) + 1
        nl = nlt * 3

    n = n.ljust(nl, '0')
    start = 0
    for i in range(len(n) / 3):
        res2.append(str(int(float(b2d(int(n[start:start + 3]))))))
        start = start + 3

    return ''.join(res) + '.' + ''.join(res2)

"""
八进制转二进制
"""
def o2b(oct):
    m, n = ('%f' % oct).split('.')

    res = [str(int(float(d2b(int(i))))).rjust(3, '0') for i in m]
    res2 = [str(int(float(d2b(int(i))))).ljust(3, '0') for i in n]

    return ''.join(res) + '.' + ''.join(res2).rstrip('0')

"""
浮点数转十六进制
"""
def f2x(flt):
    raw = struct.pack('>f', flt)
    return ','.join([hex(ord(i)).ljust(4, '0') for i in raw])


print d2b(2007.555)
print d2b(0.45)
print d2b(45)
print d2b(0)

print b2d(11111010111.10001)
print b2d(0.0111)
print b2d(101.101)

print b2o(101110.101)
print b2o(1101.1)

print o2b(67.54)
print o2b(56.5)

print f2x(21.5)
print b2d(00000001111111110000000000000001)
