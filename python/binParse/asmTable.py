#!/usr/bin/env python
#!encoding=utf8

'''
File: asmTable.py
Author: Zenki (Zenki.J.Zha), zenki2001cn@163.com
Description:
Version:
Last Modified: 十二月 05, 2012
'''

import re

class AsmTableBuilder(object):
    """docstring for AsmTableBuilder"""
    def __init__(self, cache_path):
        super(AsmTableBuilder, self).__init__()
        self.cache_path = cache_path
        # AsmTable object list
        self.asm_tables = []

        self.__buildAsmTable()

    def __buildAsmTable(self):
        """docstring for __buildAsmTable"""
        f = open(self.cache_path)

        section_mark = 0
        section_name = ''
        symbol_name = ''
        symbols_map = {}
        asm_table = None

        try:
            for line in f:
                m = re.match('Disassembly of section (.*):', line)
                if m != None:
                    if asm_table != None:
                        # asm_table.printInfo()
                        self.asm_tables.append(asm_table)
                    section_name = m.groups()[0]
                    section_mark = 1
                    asm_table = AsmTable(section_name)
                    continue

                if section_mark:
                    m = re.match('(\d|\w){8} <(.*)>:', line)
                    if m != None:
                        symbol_name = m.groups()[1]
                        symbols_map[symbol_name] = ''
                        continue

                    if symbol_name != '' and asm_table != None:
                        symbols_map[symbol_name] = symbols_map[symbol_name] + line
                        asm_table.updateSymbolsMap(symbols_map)
        except Exception, e:
            print e, line
        finally:
            f.close()

    def getCodeForTextField(self, symbol_name):
        """docstring for getCodeForSymbolName"""
        for table in self.asm_tables:
            if table.getSection() == '.text':
                symbols_map = table.getSymbolsMap()
                if symbol_name in symbols_map:
                    return symbols_map[symbol_name]
                else:
                    return ''

# Every one AsmTable object represents a symbol field type,
# and consists of a set of symbolic name and source code mapping map.
class AsmTable(object):
    """
    1. section          'symbol field'
    2. symbol_maps      {'symbol name' : 'source code'}
    """
    def __init__(self, section):
        super(AsmTable, self).__init__()
        self.__section = section
        self.__symbols_map = {}

    def getSection(self):
        """docstring for getSection"""
        return self.__section

    def getSymbolsMap(self):
        """docstring for getSymbol"""
        return self.__symbols_map

    def updateSymbolsMap(self, symbols_map):
        """docstring for addSymbolsMap"""
        self.__symbols_map = symbols_map

    def getPacks(self):
        """docstring for getPacks"""
        return (self.__section, self.__symbols_map)

    def printInfo(self):
        """docstring for printInfo"""
        print '__section: ', self.__section, ' : '
        print '__symbols_map: ', self.__symbols_map
        # import pdb; pdb.set_trace() ### XXX BREAKPOINT
        # for key, val in self.__symbols_map:
            # print 'section: ', key
            # print 'code: ', val
