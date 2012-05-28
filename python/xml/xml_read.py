from xml.dom import minidom

doc = minidom.parse('./test.xml')
print doc.toxml()
