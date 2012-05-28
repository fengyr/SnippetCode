# Create your views here.
from django.http import HttpResponse, HttpResponseRedirect
from django.template import loader, Context
from demo.address.models import *
from django.shortcuts import get_object_or_404, render_to_response, get_list_or_404

def show_info(request):

	#a = Address.objects.all()
	#t = loader.get_template('address.html')
	#c = Context({'info':a})

	#return HttpResponse(t.render(c))
	a = get_list_or_404(Address)
	return render_to_response('address.html', {'info':a})

def jump(request):
	return HttpResponseRedirect('/login/')
