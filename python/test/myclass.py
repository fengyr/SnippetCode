#!/usr/bin/env python
#!encoding=utf8

class Myclass:
    def __init__(self, fd=0, address="0.0.0.0"):
        self.fd = fd
        self.address = address

    def display(self):
        print self.address, self.fd
        print data1
        print data2

    def __del__(self):
        print self.fd, "destroy"

myclass = Myclass(1, "192.168.100.1")
myclass.display()

myclass2 = Myclass(2, "192.168.100.2")
myclass2.display()

myclass3 = Myclass()
myclass3.display()
