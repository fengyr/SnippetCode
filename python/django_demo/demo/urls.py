from django.conf.urls.defaults import *
from django.conf import settings

urlpatterns = patterns('',
    # Example:
    # (r'^demo/', include('demo.foo.urls')),
		(r'^$', 'demo.hello.index'),
		(r'^add/$', 'demo.add.index'),
		(r'^list/$', 'demo.list.index'),
		(r'^csv/$', 'demo.csv_test.output'),
		(r'^login/$', 'demo.login.login'),
		(r'^logout/$', 'demo.login.logout'),
		(r'^address/', include('demo.address.urls')),
		(r'^ajax/$', 'django.views.generic.simple.direct_to_template',
		 {'template': 'ajax.html'}),
		(r'^ajax/input/$', 'demo.ajax.views.input'),
		(r'^tagtest/', include('demo.tagtest.urls')),
		(r'^webvideo/', include('demo.WebVideo.urls')),


		(r'^media/(?P<path>.*)$', 'django.views.static.serve', 
		 {'document_root': settings.MY_MEDIA_SETTING}),

    # Uncomment this for admin:
     (r'^admin/', include('django.contrib.admin.urls')),
)
