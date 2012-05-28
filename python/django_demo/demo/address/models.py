from django.db import models

# Create your models here.
class Address(models.Model):
	name = models.CharField('姓名', maxlength=6, unique=True)
	gender = models.CharField('性别', choices=(('M', '男'), ('F', '女')), 
			maxlength=1, radio_admin=True)
	telphone = models.CharField('电话', maxlength=20)
	mobile = models.CharField('手机', maxlength=11)
	room = models.CharField('房间', maxlength=10)

	def __str__(self):
		return self.name

	class Admin:
		list_display = ('name', 'gender', 'telphone', 'mobile', 'room')

class Book(models.Model):
	num = models.IntegerField()
	book = models.ForeignKey('Address')

	def __str__(self):
		return str(self.num)

	class Admin:
		list_display = ('num', 'book')
		
	class Meta:
		ordering = ['num']
