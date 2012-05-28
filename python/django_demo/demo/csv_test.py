#coding=utf-8
from django.http import HttpResponse
from django.template import loader, Context

address = [('张三', '地址一'), ('李四', '地址二')]
number = [1, 2, 3, 4]

def output(request):
	print request.REQUEST['address']
	#print filename
	response = HttpResponse(mimetype='text/html')
	#response['Content-Disposition'] = 'attachment; filename=%s.csv' % filename

	t = loader.get_template('csv.html')
	c = Context({'data': address, 'num':number})
	response.write(t.render(c))
	return response

