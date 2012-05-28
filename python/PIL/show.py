#!encoding=utf8
import Image
import ImageFilter
import ImageChops
import ImageDraw
import ImageFont
import ImageSequence
import ImageEnhance
import ImageFilter
import ImageOps
import ImageColor
import ImagePath
import ImageStat
from random import random

im = Image.open('frame1.ppm')
im2 = Image.open('frame_text.ppm')
print 'format: ', im.format, '|size: ', im.size, '|mode: ', im.mode, '|info: ', im.info, '|palette: ', im.palette
bands = im.getbands()
print 'bands:', bands
stat = ImageStat.Stat(im)
print 'extrema: ', stat.extrema, '|count: ', stat.count, '|sum: ', stat.sum, '|sum2: ', stat.sum2, '|pixel: ', stat.mean, '|median: ', stat.median, '|rms: ', stat.rms, '|var: ', stat.var, '|stddev: ', stat.stddev

#Mode
#('1', 'L', 'P', 'RGB', 'RGBA', 'CMYK', 'YCbCr', 'I', 'F')
#Format
#('bmp', 'png', 'tif', 'jpeg')
#获取分量类型
#im.getbands()
#缩小
#im.thumbnail((704, 576))
#旋转效果
#im.rotate(45)
#滤镜效果
#im = im.filter(ImageFilter.SMOOTH)
#im2 = im.filter(ImageFilter.EMBOSS)
#像素点操作
#pix = im.load()
#获取数据流
#core = im.getdata()
#图像检测
im2c = im2.convert('YCbCr')
imc = im.convert('YCbCr')
im2c.save('ybr2.tif')
imc.save('ybr1.tif')
#im3 = ImageChops.subtract(im2, im)
#im3 = ImageChops.difference(im2, im)
#im3 = ImageChops.invert(im)
#im3 = ImageChops.multiply(im, im2)
#im3 = ImageChops.screen(im, im2)
#im3.show()
#类型转换
#im = im.convert('YCbCr')
#im.save('save.tiff')
#画图和文字操作
#font = ImageFont.truetype('FreeSans.ttf', 20)
#draw = ImageDraw.Draw(im)
#draw.text((100, 100), 'i am drawing it', font=font)
#del draw
#im.save('frame_text.ppm')
#遍历图像序列
#it = ImageSequence.Iterator(im)
#for frame in it:
#print 'format: ', frame.format, '| size: ', frame.size, '| mode: ', frame.mode, '| info: ', frame.info, '| palette: ', frame.palette
#图像加强
#enhance = ImageEnhance.Color(im)
#im2 = enhance.enhance(0.5)
#对只读图像的操作
#im2 = ImageOps.grayscale(im)
#im2 = ImageOps.fit(im, (704, 576), Image.NEAREST, 1.0, (5.0, 10.0))
#
#im2 = Image.new('RGB', (352, 288))
#pix = []
#for x in range(352):
#	for y in range(288):
#		color = ImageColor.getcolor('rgb(%d, %d, %d)' % (random()*100, random()*100, random()*100), 'RGB')
#		pix.append(color)
#im2.putdata(pix)


#im.show()
