#!/usr/bin/env python
#!encoding=utf8

'''
File: symbolTable.py
Author: Zenki (Zenki.J.Zha), zenki2001cn@163.com
Description:
Version:
Last Modified: 十二月 05, 2012
'''

class SymbolTableBuilder(object):
    """docstring for SymbolTableBuilder"""
    def __init__(self, cache_path):
        super(SymbolTableBuilder, self).__init__()
        self.cache_path = cache_path
        self.symbol_tables = []

        self.__buildSymbolTable()

    def __buildSymbolTable(self):
        """docstring for buildSymbolTable"""
        findMark = -1       # null->-1, find->1, end->0
        f = open(self.cache_path)

        try:
            for line in f:
                line = line.rstrip('\n')

                if line == '':
                    continue
                if line.find('SYMBOL TABLE:') != -1:
                    findMark = 1
                    # print 'find symbol table'
                    continue

                if findMark == 1:
                    offset = line[:8]
                    func_field = line[9:12].strip()
                    sym_type = line[13:17].strip()
                    sym_field, tails = line[17:].split('\t')
                    size = tails[:8]
                    name = tails[8:].strip()

                    symbol = SymbolTable(
                        offset, func_field, sym_type, sym_field, size, name)
                    self.symbol_tables.append(symbol)
                elif findMark == 0:
                    break
        except Exception, e:
            print e
        finally:
            f.close()

        return self.symbol_tables

    def getTableForSymfield(self, sym_field):
        """docstring for getTableForType"""

        # debug info
        # for table in self.symbol_tables:
        #     table.printInfo()

        return [table for table in self.symbol_tables if table.getSymField() == sym_field]

class SymbolTable(object):
    """
    1. 段内偏移     0804a060
    2. 符号作用域   g
    3. 符号类型     F
    4. 符号所在域   .text
    5. 内存空间大小 0000005a
    6. 符号名       puts@@GLIBC_2.0
    """
    def __init__(self, offset, func_field, sym_type, sym_field, size, name):
        super(SymbolTable, self).__init__()
        self.__offset = offset
        self.__func_field = func_field
        self.__sym_type = sym_type
        self.__sym_field = sym_field
        self.__size = size
        self.__name = name

    def __del__(self):
        """docstring for __del__"""

    def getOffset(self):
        """docstring for getOffset"""
        return self.__offset

    def getFuncField(self):
        """docstring for getFuncField"""
        return self.__func_field

    def getSymType(self):
        """docstring for getsym_type"""
        return self.__sym_type

    def getSymField(self):
        """docstring for getSymField"""
        return self.__sym_field

    def getSize(self):
        """docstring for getSize"""
        return self.__size

    def getName(self):
        """docstring for getName"""
        return self.__name

    def getPacks(self):
        """docstring for getPacks"""
        return (self.__offset, self.__func_field, self.__sym_type, self.__sym_field, self.__size, self.__name)

    def printInfo(self):
        """docstring for print"""
        print self.__offset, ' : ', \
              self.__func_field, ' : ', \
              self.__sym_type, ' : ', \
              self.__sym_field, ' : ', \
              self.__size, ' : ', \
              self.__name
