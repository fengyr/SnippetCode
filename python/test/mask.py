#!/usr/bin/env python
#!encoding=utf8


def setfirst3(cur_val, set_val):
    mask = 0x7
    mask ^= 0xff
    _set_val = set_val
    _set_val ^= 0xff
    _set_val |= mask
    cur_val |= 0x7
    _set_val &= cur_val
    print _set_val


def setfirst(cur_val, set_val):
    mask = 0x7
    mask ^= 0xff        # 取反
    cur_val &= mask     # 取高5位值，低3位置0
    # print cur_val

    _set_val = set_val
    _set_val ^= 0x07    # 取低3位值，高5位置0

    cur_val |= _set_val # 取8位值，合并之前的高5位和低3位
    print cur_val

if __name__ == '__main__':
    setfirst(235, 0x3)
