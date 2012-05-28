#!/usr/bin/python

import time
person_id = 110

class person(object):
    def __init__(self, name, phone):
        self.name = name
        self.phone = phone

    def _private_show(self):
        print self.name
        print self.phone

    def display(self):
        print "my name is:", self.name, "\n", "my phone is:", self.phone

    def __del__(self):
        print 'del'

class OnePerson(person):
    def __init__(self, name, phone):
        self.name = name
        self.phone = phone

    def display(self):
        print "one person name is:", self.name, "\n", "one person phone is:", self.phone

    def __del__(self):
        print 'del'

if __name__ == '__main__':
    p = person('zenki', '13915981321')
    p.display()
    p._private_show()
    del p

    p = OnePerson('one person', '012345678')
    p.display()
    p._private_show()
    del p
