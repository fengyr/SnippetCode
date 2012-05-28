from django.db import models

# Create your models here.
class Wiki(models.Model):
	pagename = models.CharField('页名', maxlength=20, unique=True)
	content = models.TextField('内容')
	
	class Admin:
		list_display = ('pagename', 'content')
