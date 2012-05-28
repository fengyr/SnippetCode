import threading
from threading import Timer
from time import time, sleep

lock_a = threading.Lock()
lock_b = threading.Lock()

def show(*caller):
	lock_a.acquire()
	print caller[0], time()
	sleep(2)
	print 'sleep 2 sec'
	lock_a.release()

Timer(3, show, ('func', 0)).start()
sleep(1)

t = threading.Thread(target=show, args=(('thread1', 0)))
t.start()
t = threading.Thread(target=show, args=(('thread2', 0)))
t.start()


