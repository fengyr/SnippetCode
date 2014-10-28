CFLAGS = -D_REENTRANT -Wall -pedantic -Isrc
LDFLAGS = -lpthread

ifdef DEBUG
CFLAGS += -g
LDFLAGS += -g -lpthread
endif

TARGETS = tests/thrdtest tests/heavy tests/shutdown src/libthreadpool.a

all: $(TARGETS)

tests/shutdown: tests/shutdown.o src/threadpool.o $(LDFLAGS)
tests/thrdtest: tests/thrdtest.o src/threadpool.o $(LDFLAGS)
tests/heavy: tests/heavy.o src/threadpool.o $(LDFLAGS)
src/threadpool.o: src/threadpool.c src/threadpool.h $(LDFLAGS)
tests/thrdtest.o: tests/thrdtest.c src/threadpool.h $(LDFLAGS)
tests/heavy.o: tests/heavy.c src/threadpool.h $(LDFLAGS)
src/libthreadpool.a: src/threadpool.o
	ar rcvs src/libthreadpool.a src/threadpool.o

clean:
	rm -f $(TARGETS) *~ */*~ */*.o
