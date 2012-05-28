from django.conf.urls.defaults import *
from django.conf import settings

urlpatterns = patterns('',
    # Example:
    # (r'^demo/', include('demo.foo.urls')),
		(r'^webvideo/', include('webvideo.WebVideo.urls')),
		(r'^$', include('webvideo.WebVideo.urls')),
		(r'^webconfig/', include('webvideo.WebConfig.urls')),
		#(r'^$', 'django.views.generic.simple.direct_to_template',
		# {'template':'video_login.html'}),

		(r'^sitemedia/(?P<path>.*)$', 'django.views.static.serve', 
		 {'document_root': settings.MY_MEDIA_SETTING}),

    # Uncomment this for admin:
    # (r'^admin/', include('django.contrib.admin.urls')),
)
