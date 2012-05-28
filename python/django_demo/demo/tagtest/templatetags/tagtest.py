from django import template
from django.http import HttpResponseRedirect, HttpResponse
from django.template import loader, Context

register = template.Library()

class mytagsNode(template.Node):
	def __init__(self, goto):
		self.name = goto
	
	def render(self, context):
		r = HttpResponse()
		t = loader.get_template(self.name)
		c = Context({})
		return t.render(c)

@register.tag
def mytags(parser, token):
	arg = token.contents.split()
	return mytagsNode(arg[1])

