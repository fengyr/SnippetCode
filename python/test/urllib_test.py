import urllib2, httplib

urllib2.httplib.HTTPConnection.debuglevel = 1
request = urllib2.Request('http://127.0.0.1')
print request.get_host()
opener = urllib2.build_opener()
f = opener.open(request)
print f.headers
print f.read()
