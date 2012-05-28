from django.conf.urls.defaults import *
from django.conf import settings

urlpatterns = patterns('webvideo.WebConfig.views',
    # Example:
		(r'^$', 'list'),
		(r'^list/$', 'list'),
		(r'^users/$', 'users'),
		(r'^ip/$', 'ip'),
		(r'^allsites/$', 'allsites'),
		#(r'^monitor(?P<port>)$', 'monitor'),
)
