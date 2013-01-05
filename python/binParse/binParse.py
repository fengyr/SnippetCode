#!/usr/bin/env python
#!encoding=utf8

'''
File: binParse.py
Author: Zenki (Zenki.J.Zha), zenki2001cn@163.com
Description:
Version: 0.2
Last Modified: 2012-12-30 14:59:03
    0.2:
        1. 添加注释
'''

import config
from symbolTable import SymbolTableBuilder
from asmTable import AsmTableBuilder

import os

##
# @Synopsis use objdump bin to generate cache file
#
# @Param binPath    src binary file path
# @Param cachePath  des cache dir
#
# @Returns
def generateCaches(bin_path, cache_path, options):
    """docstring for generateCache"""
    cmd = '%s %s %s > %s' % (config.OBJDUMP, options, bin_path, cache_path)
    os.system(cmd)

def main():
    syms_cache_path = '%s/%s' % ('./cache', config.DUMP_SYMS_CACHE)
    asm_cache_path = '%s/%s' % ('./cache', config.DUMP_ASM_CACHE)

    generateCaches('./main', syms_cache_path, '-t')
    generateCaches('./main', asm_cache_path, '-S -d')

    asm_table_builder = AsmTableBuilder(asm_cache_path)
    symbol_table_builder = SymbolTableBuilder(syms_cache_path)

    # 遍历.text段的符号，并根据该符号，查找对应的汇编代码
    for table in symbol_table_builder.getTableForSymfield('.text'):
        symbol_name = table.getName()
        print '****** ', symbol_name
        print asm_table_builder.getCodeForTextField(symbol_name)

if __name__ == '__main__':
    main()
