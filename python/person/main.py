#!/usr/bin/python

import person
import cPickle

def read_info():
	person_hash = {}
	f = file("info.db", 'r')
	try:
		person_hash = cPickle.load(f)
	except EOFError:
		pass
	f.close()

	print "name    phone\n"
	for name, phone in person_hash.items():
		phone.display()

	return person_hash

def process():
	'''main process'''
	person_hash = read_info()
	#person_hash = {}
	f = file("info.db", 'w')
	running = 1
	while running:
		name = raw_input("Enter name:")
		if name == 'q':
			break
		phone = int(raw_input("Enter phone:"))
	
		p = person.person(name, phone)
		person_hash[name] = p
	
	#person_hash[name].display()
	cPickle.dump(person_hash, f)
	f.close()

process()