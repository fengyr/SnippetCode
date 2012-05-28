import sys
import doctest

def info(obj):
    list = [ i for i in dir(obj) if callable(getattr(obj, i))]
    for i in list:
        print i, "--------->", getattr(obj, i).__doc__

#info(sys)

class Person:
    def __init__(self):
        self.name = 'zenki'

    def tallname(self):
        """
        >>> tallname()
        'zenki talk'
        """
        return self.name, "talk"

class Student(Person):

    stroy = 'this is a stroy'

    def __init__(self):
        Person.__init__(self)

    def __add__(self, context):
        """
        >>> __add__('hello')
        'hello this is a stroy'
        """
        return context, self.__class__.stroy

if __name__ == '__main__':
    u = Student()
    u.tallname()
    u+'hello'
