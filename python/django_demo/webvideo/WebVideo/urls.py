from django.conf.urls.defaults import *
from django.conf import settings

urlpatterns = patterns('webvideo.WebVideo.views',
    # Example:
		(r'^$', 'login'),
		(r'^monitor/$', 'monitor'),
		(r'^logout/$', 'logout'),
		(r'^list/$', 'list'),
		(r'^users/$', 'users'),
		#(r'^monitor(?P<port>)$', 'monitor'),
)
