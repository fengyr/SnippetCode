# Create your views here.
from django.shortcuts import render_to_response
from django.conf import settings
from django.http import HttpResponseRedirect
import os
import md5

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
			b.remove('')
			for s in b:
				buf.append(s.split(':'))
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

	sites = load_file()
	d = {'sites':sites}

	return render_to_response('video_monitor.html', d)

def login(request):
	try:
		if verify_log(request.POST['NAME'], request.POST['PASSWD']):
			set_cache(request.META['REMOTE_ADDR'])
			return HttpResponseRedirect('/webvideo/monitor')
		else:
			return render_to_response('video_login.html')
	except KeyError:
		return render_to_response('video_login.html')

def logout(request):
	del_cache(request.META['REMOTE_ADDR'])
	return HttpResponseRedirect('/webvideo/')
