from django.db import models

# Create your models here.
class Book(models.Model):
    Catalog = models.CharField(max_length=512)
    BookName = models.CharField(max_length=512)
    BookPath = models.CharField(max_length=512)

    def __unicode__(self):
        return self.BookName
