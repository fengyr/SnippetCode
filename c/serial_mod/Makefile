all:serial.so

serial.so:serial.c serial.h
	gcc serial.c -fPIC -shared -o serial.so

clean:
	rm *.so
