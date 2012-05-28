from django.conf.urls.defaults import *

urlpatterns = patterns('demo.tagtest.views',
		(r'^index/$', 'tagtest_index'),
)

urlpatterns += patterns('',
		(r'^$', 'django.views.generic.simple.direct_to_template',
		 {'template': 'tagtest.html'}),
)
