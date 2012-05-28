import gzip
from StringIO import StringIO

f = open('readmp3.py', 'r')
print 'before compress len ', len(f.read())
f.seek(0)

zipf = gzip.open('gzip.gz', 'w')
zipf.write(f.read())
zipf.close()
f.close()

f = open('gzip.gz', 'r')
print 'after compress len ', len(f.read())
f.close()

zipf = gzip.open('gzip.gz', 'r')
print 'org data: ', zipf.read()
zipf.close()


