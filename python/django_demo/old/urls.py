from django.conf.urls.defaults import *

urlpatterns = patterns('',
    # Example:
    # (r'^old/', include('old.foo.urls')),

    # Uncomment this for admin:
     (r'^admin/', include('django.contrib.admin.urls')),
)
