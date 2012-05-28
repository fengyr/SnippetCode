#!/usr/bin/python
#!encoding=utf8

import unittest
import doctest
import person

class PersonTest(unittest.TestCase):
    """初始化测试例程"""
    def setUp(self):
        self.p = person.Person()
        self.s = person.Student()

    """退出测试例程"""
    def tearDown(self):
        del self.p
        del self.s

    def testPersonTallname(self):
        assert self.p.tallname() == ('zenki', 'talk'), 'Tallname err'

    def testStudentAdd(self):
        assert self.s + 'hello' == ('hello', 'this is a stroy'), 'Add err'

if __name__ == '__main__':
    """ 添加测试组件 """
    testSuit = unittest.TestSuite()
    """ 添加测试用例一 """
    testSuit.addTest(PersonTest('testPersonTallname'))
    """ 添加测试用例二 """
    testSuit.addTest(PersonTest('testStudentAdd'))

    unittest.main()
