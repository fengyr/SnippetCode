import os, time, sys

sys.stdout.close()
sys.stdout.write('\r1')
sys.stdout.flush()
time.sleep(1)
sys.stdout.write('\r2')
sys.stdout.flush()
time.sleep(1)
sys.stdout.write('\r3')
sys.stdout.flush()
