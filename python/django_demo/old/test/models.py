from django.db import models

# Create your models here.
class poll(models.Model):
	question = models.CharField(maxlength=200)
	pub_date = models.DateTimeField('date published')

	def __str__(self):
		return self.question

	class Admin:
		list_display = ('question', 'pub_date')
		list_filter = ['pub_date']

class choice(models.Model):
	poll = models.ForeignKey(poll)
	choice = models.CharField(maxlength=200)
	votes = models.IntegerField()

	def __str__(self):
		return self.choice
	
	class Admin:
		list_display = ('poll', 'choice', 'votes')
		list_filter = ['poll']
