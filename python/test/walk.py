import os

pathname = []
for root, dirs, files in os.walk('/home/zenki/Desktop/music'):
	pathname.extend([os.path.join(root, name) for name in files])

for i in pathname:												
	print i													   
