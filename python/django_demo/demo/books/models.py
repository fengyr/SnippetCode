from django.db import models

# Create your models here.
class author(models.Model):
	name = models.CharField(maxlength=30)
	age = models.IntegerField()

	def __str__(self):
		return self.name

	class Admin:
		list_display = ('name', 'age')

class book(models.Model):
	title = models.CharField(maxlength=30)
	date = models.DateField()
	authors = models.ForeignKey(author)

	def __str__(self):
		return self.title

	class Admin:
		list_display = ('title', 'date')


