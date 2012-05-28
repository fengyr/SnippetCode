#!encoding=utf8

from lxml import etree

#创建根节点
root = etree.Element('class', department='math')
#加入节点方法一
root.append(etree.Element('child1'))
#加入节点方法二
child2 = etree.SubElement(root, 'child2')
child3 = etree.SubElement(root, 'child3')
child4 = etree.SubElement(child2, 'child4')
#为节点添加属性
root.attrib['teacher'] = 'zhanghong'
root[0].attrib['age'] = '12'
root[1].attrib['age'] = '13'
root[2].attrib['age'] = '11'
#为节点添加正文
root.text = 'a math class'
root[0].text = 'his name is jim'
root[1].text = 'his name is terry'
root[2].text = 'her name is mary'
child2.tail = 'add tail'
root.append(etree.Entity('#1234'))
root.append(etree.Comment('some comment'))

#etree.tostring,可选参数
print etree.tostring(root, pretty_print=True)
print etree.tostring(root, pretty_print=True, with_tail=False, method='text')

#通过XPath,来查找内容
text_str = root.xpath('string()')
print text_str
text_text =  root.xpath('//text()')
print text_text
#XPath,查找的内容可以智能识别其父节点信息
for text in text_text:
    print text.getparent().tag

#可以当作list来处理
print root[0], root[1][0].tag, root[2].tag

#迭代处理元素,并提取需要的元素
for e in root.iter('child1'):
    print 'tag:', e.tag, '*text:', e.text

#from_string,解析string
str = etree.tostring(root)
root = etree.fromstring(str)
print etree.tostring(root)

#解析文件对象
#root = etree.parse(some_file_like)
