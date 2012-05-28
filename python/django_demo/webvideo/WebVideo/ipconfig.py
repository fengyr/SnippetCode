#!/usr/bin/python
import os, sys
import socket
import fcntl
import struct
import time

#SIOCGIFADDR 	8915
#SIOCGIFNETMASK 891B
INTERFACE = '/etc/network/interfaces'

def get_interface():
	pf = os.popen('mii-tool')
	list = pf.readlines()
	pf.close()
	interface = []
	for i in list:
		interface.append(i.split(':')[0])

	return interface

def get_ip(ifname):
	#import pdb;pdb.set_trace()
	try:
		s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
		addr = socket.inet_ntoa(fcntl.ioctl(s.fileno(), 0x8915, 
					struct.pack('256s', ifname[:15]))[20:24])
		netmask = socket.inet_ntoa(fcntl.ioctl(s.fileno(), 0x891B, 
					struct.pack('256s', ifname[:15]))[20:24])
		s.close()
		pf = os.popen('route|grep default')
		route = pf.readline().split()[1]
		pf.close()
	except:
		return ['eth0', '0.0.0.0', '0.0.0.0', '0.0.0.0']

	return [ifname, addr, netmask, route]

def set_ip(ifname, addr, netmask, route):
	try:
		f = open(INTERFACE, 'r+')
		list = f.readlines()
		newl = []
		for i in list:
			if 'address' in i:
				i = '\t%s %s\n' % (i.split()[0], addr)
			if 'netmask' in i:
				i = '\t%s %s\n' % (i.split()[0], netmask)
			if 'gateway' in i:
				i = '\t%s %s\n' % (i.split()[0], route)

			newl.append(i)
		f.seek(0, 0)
		f.truncate()
		f.writelines(newl)
		f.close()
	except:
		return 'failed'
	#import pdb;pdb.set_trace()
	#os.popen('/etc/init.d/networking restart')
	#print 'networking restarted'
	return 'success'

if __name__ == '__main__':
	for i in get_interface():
		print i, get_ip(i)
		set_ip(i, '192.168.100.111', '255.255.255.1', '192.168.100.33')
