import sched
import time

def message(msg):
    print msg

s = sched.scheduler(time.time, time.sleep)
e1 = s.enter(1, 1, message, 'h')
e2 = s.enter(3, 1, message, 'w')
s.cancel(e2)
s.run()
print 'end'