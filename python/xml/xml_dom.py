import xml.dom.minidom

impl = xml.dom.minidom.getDOMImplementation()
	
dom = impl.createDocument(None, 'students', None)
root = dom.documentElement

for i in range(4000):
	item = dom.createElement('student')
#text = dom.createTextNode('')
#	item.appendChild(text)

	item2 = dom.createElement('name')
	text = dom.createTextNode('abc')
	item2.appendChild(text)


	item3 = dom.createElement('age')
	text = dom.createTextNode('123')
	item3.appendChild(text)

	root.appendChild(item)
	item.appendChild(item2)
	item.appendChild(item3)

f = open('./test.xml', 'w')
root.writexml(f, ' ', '    ', '\n')
print root.toprettyxml()
