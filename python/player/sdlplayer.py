#encoding=utf8
import os, sys, time
import thread
import pygame

"""
使用SDL_mixer解码音频
"""

class XPlayer:
	def __init__(self):
		self.filename = ''
		print 'pygame:', pygame.ver
		pygame.mixer.init()
		self.player = pygame.mixer.music
		pass

	def clear(self):
		self.player.stop()
		print '停止当前播放 %s' % self.filename
		return '停止播放 %s' % os.path.basename(self.filename)

	def play(self, filename):
		""" use plugs play file
			"""
		self.filename = filename
		try:
			self.player.load(filename)
		except pygame.error:
			print '不支持的视频格式'
			return '不支持的视频格式'
	
		self.player.play()
		return os.path.basename(self.filename)

	def resume(self):
		if self.filename == '':
			return
		else:
			return self.play(self.filename)
									
	def __del__(self):
		pygame.mixer.quit()
		pass
