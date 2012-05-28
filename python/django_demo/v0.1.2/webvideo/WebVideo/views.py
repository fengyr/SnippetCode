# Create your views here.
from django.shortcuts import render_to_response
from django.conf import settings
from django.http import HttpResponseRedirect
import os, sys, time
import md5
import config
import controller

SITE_FILE = settings.MY_MEDIA_SETTING + 'users/webvideo.conf'
USER_FILE = settings.MY_MEDIA_SETTING + 'users/user.conf'
CACHE_FILE = settings.MY_MEDIA_SETTING + 'users/'

def load_file():
	f = open(SITE_FILE)
	sitesinfo = []
	site = f.read().split('[')
	for i in range(len(site)):
		if site[i] == '':
			continue
		else:
			buf = []
			b = site[i].split('\n')
			'' in b and b.remove('')
			for s in b:
				r = s.split(':')
				if r == ['']:
					continue
				buf.append(r)
			sitesinfo.append(buf)

	return sitesinfo

def verify_log(user, passwd):
	try:
		f = open(USER_FILE)
		b = f.read().split('\n')
	finally:
		f.close()
		
	users = {}
	for i in b:
		if i == '':
			continue
		u = i.split(':')
		users[u[0]] = u[1]
	pwd = md5.new(passwd)
	try:
		if users[user] == pwd.hexdigest():
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

def monitor(request):
	if not get_cache(request.META['REMOTE_ADDR']):
		return HttpResponseRedirect('/webvideo/')

	l_video_port = 0
	if request.has_key('last_port'):
		# handle logout videoserver message
		l_video_port = request.GET['last_port']
		s = controller.init_socket()
		controller.handle(s, 'stop:%s\r' % l_video_port)

	time.sleep(1)

	java_script_on = False
	video_port = 0
	if request.has_key('port'):
		video_port = request.GET['port']
		java_script_on = True
		# handle login videoserver message
		s = controller.init_socket()
		controller.handle(s, 'start:%s\r' % video_port)

	f = lambda x:('MSIE' in x and 'ie') or 'firefox'
	OS = f(request.META['HTTP_USER_AGENT'])
	
	sites = load_file()
	d = {'sites':sites, 'os':OS, 'localhttp':config.LOCAL_HTTP, 
		 'serverip':config.VIDEO_SERVER_IP.split(':')[0], 'jsp':java_script_on, 
		 'serverport':video_port
	}

	return render_to_response('video_monitor.html', d)

def login(request):
	d = {'localhttp':config.LOCAL_HTTP}
	try:
		if verify_log(request.POST['NAME'], request.POST['PASSWD']):
			set_cache(request.META['REMOTE_ADDR'])
			return HttpResponseRedirect('/webvideo/monitor')
		else:
			return render_to_response('video_login.html', d)
	except KeyError:
		return render_to_response('video_login.html', d)

def logout(request):
	del_cache(request.META['REMOTE_ADDR'])

	l_video_port = 0
	if request.has_key('last_port'):
		# handle logout videoserver message
		s = controller.init_socket()
		l_video_port = request.GET['last_port']
		controller.handle(s, 'stop:%s\r' % l_video_port)

	return HttpResponseRedirect('/webvideo/')

