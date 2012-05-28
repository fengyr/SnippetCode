# Create your views here.
from django.http import HttpResponse, HttpResponseRedirect
from django.template import loader, Context
from django.shortcuts import render_to_response

def tagtest_index(request):
	return render_to_response('login.html')
