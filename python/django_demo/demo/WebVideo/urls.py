from django.conf.urls.defaults import *
from django.conf import settings

urlpatterns = patterns('demo.WebVideo.views',
    # Example:
		(r'^$', 'login'),
		(r'^monitor/$', 'monitor'),
		(r'^logout/$', 'logout'),
)
