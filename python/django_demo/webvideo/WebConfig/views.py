# Create your views here.
#!encoding=utf8
from django.shortcuts import render_to_response
from django.core.cache import cache
from django.conf import settings
from django.http import HttpResponseRedirect, HttpResponse
import os, sys, time, cPickle
import md5
import ipconfig
import pdb
import controller

IP_FILE = settings.MY_MEDIA_SETTING + 'users/ip.conf'
USER_FILE = settings.MY_MEDIA_SETTING + 'users/user.conf'
VIDEO_FILE = settings.MY_MEDIA_SETTING + 'users/video_list.conf'
CACHE_FILE = settings.MY_MEDIA_SETTING + 'users/'
SERVER_PORT = 8800

def load_file(fname):
	f = open(fname, 'r+')
	obj = cPickle.load(f)
	f.close()
	
	return obj

def dump_file(fname, obj):
	f = open(fname, 'r+')
	f.seek(0, 0)
	f.truncate()
	cPickle.dump(obj, f)
	f.close()
	
	
def ip(request):
	#interfaces = ipconfig.get_interface()
	ip_info = ipconfig.get_ip('eth0')
	d = {'localhttp':ip_info[1], 'info':ip_info}

	if request.has_key('mod'):
		try:
			#import pdb;pdb.set_trace()
			if '' in [request.POST['IP1'], request.POST['IP2'], request.POST['IP3'], 
			request.POST['IP4'], request.POST['NM1'], request.POST['NM2'], 
			request.POST['NM3'], request.POST['NM4'], request.POST['GW1'], 
			request.POST['GW2'], request.POST['GW3'], request.POST['GW4']]:
				raise 'post ip error'
			ip = '.'.join([request.POST['IP1'], request.POST['IP2'], request.POST['IP3'], request.POST['IP4']])
			netmask = '.'.join([request.POST['NM1'], request.POST['NM2'], request.POST['NM3'], request.POST['NM4']])
			gateway = '.'.join([request.POST['GW1'], request.POST['GW2'], request.POST['GW3'], request.POST['GW4']])
			#print ip.split('.'), netmask, gateway
			d['modified_ip'] = [ip, netmask, gateway]
			ipconfig.set_ip('eth0', ip, netmask, gateway)
		except:
			pass
	
	if request.has_key('reboot'):
		os.system('reboot')
		return render_to_response('video_reboot.html')

	return render_to_response('video_ip.html', d)

def list_page(list, page):
	leng = 0
	slist = []
	for i in list:
		for j in i[1:]:
			slist.append([i[0], j])
			leng = leng + 1

	p = 1
	min = page*10-10
	max = page*10
	if leng%10 == 0:
		p = leng/10
		slist = slist[min:max]
	else:
		p = leng/10 + 1
		if page > leng/10:
			slist = slist[min:]
		else:
			slist = slist[min:max]
	
	#pdb.set_trace()
	print 'camera num:',leng
	return slist, p

def list(request):
	list = load_file(VIDEO_FILE)
	ip_info = ipconfig.get_ip('eth0')

	#import pdb;pdb.set_trace()
	mark = False
	if request.has_key('add'):
		try:
			e = [request.POST['SECTION'], [request.POST['NAME'], request.POST['IP'], request.POST['PORT'], request.POST['SOURCE'], request.POST['RATE'], request.POST['AUTHOR']]]
			if e not in list and '' not in e and '' not in e[1]:
				for i in list:
					if request.POST['SECTION'] == i[0]:
						i.append([request.POST['NAME'], request.POST['IP'], request.POST['PORT'], request.POST['SOURCE'], request.POST['RATE'], request.POST['AUTHOR']])
						mark = True
						break
				if not mark:
					list.append(e)
			dump_file(VIDEO_FILE, list)
			#print list
		except:
			pass

	if request.has_key('del'):
		try:
			for i in list:
				if request.POST['SECTION'] == i[0]:
					i.remove([request.POST['NAME'], request.POST['IP'], request.POST['PORT'], request.POST['SOURCE'], request.POST['RATE'], request.POST['AUTHOR']])
					if len(i) == 1:
						del i
					dump_file(VIDEO_FILE, list)
		except:
			pass

	if request.has_key('cle'):
		try:
			list = []
			dump_file(VIDEO_FILE, list)
		except:
			pass

	if request.has_key('active'):
		try:
			print 'connect to x264 server', ip_info[1], SERVER_PORT
			s = controller.init_socket(ip_info[1], SERVER_PORT)
			controller.handle(s, 'reset\r')
			time.sleep(1)
		except:
			pass

	"""
	为分页筛选列表
	"""
	curpage = 1
	if request.has_key('page'):
		curpage = page = int(request.GET['page'])
	else:
		page = 1
	slist, allpage = list_page(list, page)

	d = {'localhttp':ip_info[1], 'list':slist, 'page':[allpage, curpage], 'author_class':range(10)}
	if request.has_key('sect'):
		d['info'] = [request.GET['sect'], request.GET['name'], request.GET['ip'], request.GET['port'], request.GET['source'], request.GET['rate'], request.GET['author']]

	return render_to_response('video_list2.html', d)

def users(request):
	#pdb.set_trace()
	users = load_file(USER_FILE)
	ip_info = ipconfig.get_ip('eth0')

	if request.has_key('add'):
		try:
			name = request.POST['NAME']
			passwd = request.POST['PASSWD']
			author = request.POST['AUTHOR']
			users[name] = [md5.md5(passwd).hexdigest(), author]
			dump_file(USER_FILE, users)
		except:
			print 'add user error'
			pass

	if request.has_key('del'):
		try:
			name = request.POST['NAME']
			del users[name]
			dump_file(USER_FILE, users)
		except:
			print 'del user error'
			pass
				
	if request.has_key('cle'):
		try:
			users = {}
			dump_file(USER_FILE, users)
		except:
			pass

	d = {'localhttp':ip_info[1], 'users':users, 'author_class':range(10)}
	if request.has_key('name'):
		d['info'] = [request.GET['name'], '****', request.GET['author']]

	#print d
	return render_to_response('video_users.html', d)

def allsites(request):
	list = load_file(VIDEO_FILE)
	req_addr = request.META['REMOTE_ADDR']

	response = HttpResponse()
	for i in list:
		area = i[0]
		for j in i[1:]:
			if j[1] != req_addr:
				print j[1], '!=', req_addr
				continue
			allsites = area + ':' + ':'.join(j) + '\n'
			response.write(allsites)

	return response
