#!/usr/bin/python
#!encoding=utf8
import time
import sys, os
import glob
import Image
import ImageFont
import ImageDraw
import ImageEnhance
import Image

class Marker():
	def __init__(self, PicDir, Format='bmp', Action='AddTime', Scale='Normal', FontSize=16, Fps=25):
		self.Font = ImageFont.truetype('FreeSans.ttf', FontSize)
		self.Fps = Fps
		self.PicDir = PicDir
		self.FileList = glob.glob(os.path.join(PicDir, '*.%s' % Format))
		self.FileList.sort()
		self._cal_stat()
		self.Size = None
		self.Action = Action
		self.Scale = Scale
	
	def __del__(self):
	    pass

	def DoWork(self):
		res = 'Failed'
		action = getattr(self, self.Action)
		if callable(action):
			res = action()
			return res
		else:
			print 'This module has not this function'
		return 'Failed'

	def _cal_stat(self):
		self.TotalTime = len(self.FileList) / self.Fps
		self.InterTime = float(1) / float(self.Fps)

	def GetPicSize(self):
		return self.Size
	
	def TextToScreen(self, context):
		sys.stdout.write('\r'+str(context))
		sys.stdout.flush()

	def DrawInfo(self, width, height, frames, result):
		print '\n'
		print '--------------------------------'
		print 'TotalTime:\t%d Sec' % self.TotalTime
		print 'InterTime:\t%.3f Sec' % self.InterTime
		print 'Width:\t\t%d' % width
		print 'Height:\t\t%d' % height
		print 'Frames:\t\t%d' % (frames - 1)
		print 'Result:\t\t%s' % result
		print '--------------------------------'

	def BlackWhite(self):
		StartTime = list(time.localtime())
		count = 0
		width = 0
		height = 0
		Result = 'Finished'
		if self.Scale != 'Normal':
			try:
				width = int(self.Scale.split('x')[0])
				height = int(self.Scale.split('x')[1])
			except:
				Result = 'Exit, scale error'
				self.DrawInfo(width, height, count, Result)
				return Result

		for file in self.FileList:
			self.TextToScreen('Convert %s ...' % file)
			try:
				im = Image.open(file)
				if self.Scale != 'Normal':
					im.thumbnail((width, height))
				elif self.Scale == 'Normal':
					if not self.Size:
						width = im.size[0]
						height = im.size[1]
						self.Size = '%dx%d' % (im.size[0], im.size[1])
			except:
				print 'Open %s faild, error format' % file
				Result = 'Exit, format error'
				break

			enhancer = ImageEnhance.Color(im)
			im = enhancer.enhance(0.0)
			#每Fps帧图像更新一秒
			if count % self.Fps == 0:
				StartTime[5] = StartTime[5] + 1
			#保存处理后的图像
			try:
				im.save(file)
			except:
				print 'Convert %s Error' % file
				continue
			count = count + 1

		self.DrawInfo(width, height, count, Result)
		return Result


	def AddTime(self):
		StartTime = list(time.localtime())
		MillSec = 0
		count = 0
		width = 0
		height = 0
		Result = 'Finished'
		if self.Scale != 'Normal':
			try:
				width = int(self.Scale.split('x')[0])
				height = int(self.Scale.split('x')[1])
			except:
				Result = 'Exit, scale error'
				self.DrawInfo(width, height, count, Result)
				return Result

		for file in self.FileList:
			self.TextToScreen('Convert %s ...' % file)
			try:
				im = Image.open(file)
				if self.Scale != 'Normal':
					im.thumbnail((width, height))
				elif self.Scale == 'Normal':
					if not self.Size:
						width = im.size[0]
						height = im.size[1]
						self.Size = '%dx%d' % (im.size[0], im.size[1])
			except:
				print 'Open %s faild, error format' % file
				Result = 'Exit, format error'
				break

			draw = ImageDraw.Draw(im)
			#每Fps帧图像更新一秒
			if count % self.Fps == 0:
				StartTime[5] = StartTime[5] + 1
				MillSec = 0
			context = '%02d:%02d:%.3f Frame %06d' % (StartTime[3], StartTime[4], StartTime[5]+MillSec, count)
			draw.text((10, 10), context, font=self.Font)
            #当需要dump成YUV格式时,需要转换色彩空间,文件扩展名改成tif
			#im = im.convert('YCbCr')
			#保存处理后的图像
			try:
				im.save(file)
			except:
				print 'AddTime: convert %s Error' % file
				continue
			MillSec = MillSec + self.InterTime
			count = count + 1

		self.DrawInfo(width, height, count, Result)
		return Result

	
if __name__ == '__main__':
	marker = Marker('/home/zenki/python/PIL/temp', 'ppm')
	marker.AddTime()


