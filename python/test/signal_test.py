import signal
import time
import sys

import signal, os

def handler(signum, frame):
    print 'Signal handler called with signal', signum
    raise IOError, "Couldn't open device!"

def inter(signum, frame):
    print 'Interrupt signal', signum
    print frame
    sys.exit(1)

# Set the signal handler and a 5-second alarm
signal.signal(signal.SIGALRM, handler)
#signal.alarm(5)
signal.signal(signal.SIGINT, inter)

# This open() may hang indefinitely
#fd = os.open('/dev/ttyS0', os.O_RDWR)

while True:
    time.sleep(1)