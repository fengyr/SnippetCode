# Create your views here.
#!encoding=utf8
from django.shortcuts import render_to_response
from django.core.cache import cache
from django.conf import settings
from django.http import HttpResponseRedirect
import os, sys, time, cPickle
import md5
import controller
import ipconfig
import pdb

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

def verify_log(user, passwd):
	users = {}
	users = load_file(USER_FILE)
		
	pwd = md5.new(passwd)
	try:
		if users[user][0] == pwd.hexdigest():
			return True
		else:
			return False
	except KeyError:
		return False

def get_cache(ip):
	try:
		f = open(CACHE_FILE+'.'+ip)
		f.close()
		return True
	except:
		return False

def set_cache(ip):
	try:
		f = open(CACHE_FILE+'.'+ip, 'w')
	finally:
		f.close()

def del_cache(ip):
	try:
		os.remove(CACHE_FILE+'.'+ip)
	except:
		print "del cache error"

# old monitor html handle
#def filter_list(allsites, author):
#	#import pdb;pdb.set_trace()
#	sites = []
#	for i in allsites:
#		cams = [j for j in i[1:] if int(author) >= int(j[5])]
#		if cams == []:
#			continue
#		cams.insert(0, i[0])
#		sites.append(cams)
#	return sites

def filter_list(allsites, author):
	#import pdb;pdb.set_trace()
	sites = []
	loop = 0
	for i in allsites:
		back = loop = loop + 1
		cams = []
		for j in i[1:]:
			if int(author) >= int(j[5]):
				loop = loop + 1
				cams.append([j, loop])
		if cams == []:
			loop = back
			continue
		cams.insert(0, [i[0], back])
		sites.append(cams)
	return sites

def monitor(request):
	if not get_cache(request.META['REMOTE_ADDR']):
		return HttpResponseRedirect('/webvideo/')

	allsites = load_file(VIDEO_FILE)
	ip_info = ipconfig.get_ip('eth0')

	if request.has_key('author'):
		log_ok = False
		users = load_file(USER_FILE)
		for key, val in users.items():
			if md5.md5(key).hexdigest() == request.GET['author']:
				author = users[key][1]
				sites = filter_list(allsites, author)
				log_ok = True
		if not log_ok:
			return HttpResponseRedirect('/webvideo/')

	l_video_port = 0
	l_video_ip = '192.1.2.3'
	if request.has_key('last_port'):
		# handle logout videoserver message
		l_video_port = request.GET['last_port']
		l_video_ip   = request.GET['last_ip']
		s = controller.init_socket(l_video_ip, SERVER_PORT)
		controller.handle(s, 'stop:%s\r' % l_video_port)
		time.sleep(1)

	video_port = 0
	video_ip = '192.1.2.3'
	java_script_on = False
	if request.has_key('port'):
		video_port = request.GET['port']
		video_ip = request.GET['ip']
		java_script_on = True
		# handle login videoserver message
		s = controller.init_socket(video_ip, SERVER_PORT)
		controller.handle(s, 'start:%s\r' % video_port)

	f = lambda x:('MSIE' in x and 'ie') or 'firefox'
	OS = f(request.META['HTTP_USER_AGENT'])

	d = {'sites':sites, 'os':OS, 'localhttp':ip_info[1], 
		 'serverip':video_ip, 'jsp':java_script_on, 
		 'serverport':video_port, 'author':request.GET['author']
	}

	return render_to_response('video_monitor.html', d)

def login(request):
	ip_info = ipconfig.get_ip('eth0')
	d = {'localhttp':ip_info[1]}
	try:
		if verify_log(request.POST['NAME'], request.POST['PASSWD']):
			set_cache(request.META['REMOTE_ADDR'])
			author = md5.md5(request.POST['NAME']).hexdigest()
			return HttpResponseRedirect('/webvideo/monitor?author=%s' % author)
		else:
			return render_to_response('video_login.html', d)
	except KeyError:
		return render_to_response('video_login.html', d)
	
def logout(request):
	del_cache(request.META['REMOTE_ADDR'])

	l_video_port = 0
	if request.has_key('last_port'):
		# handle logout videoserver message
		l_video_ip = request.GET['last_ip']
		l_video_port = request.GET['last_port']
		s = controller.init_socket(l_video_ip, SERVER_PORT)
		controller.handle(s, 'stop:%s\r' % l_video_port)

	return HttpResponseRedirect('/webvideo/')

def list(request):
	list = load_file(VIDEO_FILE)
	ip_info = ipconfig.get_ip('eth0')
	#import pdb;pdb.set_trace()
	mark = False
	if request.has_key('add'):
		try:
			e = [request.POST['SECTION'], [request.POST['NAME'], request.POST['PORT']]]
			if e not in list and e != ['', ['', '']]:
				for i in list:
					if request.POST['SECTION'] == i[0]:
						i.append([request.POST['NAME'], request.POST['PORT']])
						mark = True
						break
				if not mark:
					list.append(e)
			dump_file(VIDEO_FILE, list)
		except:
			pass

	if request.has_key('del'):
		try:
			for i in list:
				if request.POST['SECTION'] == i[0]:
					i.remove([request.POST['NAME'], request.POST['PORT']])
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

	d = {'localhttp':ip_info[1], 'list':list}
	if request.has_key('sect'):
		d['info'] = [request.GET['sect'], request.GET['name'], request.GET['port']]

	#print list
	return render_to_response('video_list.html', d)

def users(request):
	users = load_file(USER_FILE)
	ip_info = ipconfig.get_ip('eth0')

	if request.has_key('add'):
		try:
			name = request.POST['NAME']
			passwd = request.POST['PASSWD']
			users[name] = md5.md5(passwd).hexdigest()
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

	d = {'localhttp':ip_info[1], 'users':users}
	if request.has_key('name'):
		d['info'] = [request.GET['name'], '*****']

	#print d
	return render_to_response('video_users.html', d)
