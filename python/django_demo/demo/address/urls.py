from django.conf.urls.defaults import *

urlpatterns = patterns('demo.address.views', 
		(r'^$', 'show_info'),
		(r'^jump/$', 'jump'),
)

