CFLAGS = -D_REENTRANT -Wall -pedantic -Isrc
LDFLAGS = -lpthread

ifdef DEBUG
CFLAGS += -g
LDFLAGS += -g -lpthread
endif

TARGETS = tests/thrdtest tests/heavy tests/shutdown

all: $(TARGETS)

tests/shutdown: tests/shutdown.o src/threadpool.o $(LDFLAGS)
tests/thrdtest: tests/thrdtest.o src/threadpool.o $(LDFLAGS)
tests/heavy: tests/heavy.o src/threadpool.o $(LDFLAGS)
src/threadpool.o: src/threadpool.c src/threadpool.h $(LDFLAGS)
tests/thrdtest.o: tests/thrdtest.c src/threadpool.h $(LDFLAGS)
tests/heavy.o: tests/heavy.c src/threadpool.h $(LDFLAGS)

clean:
	rm -f $(TARGETS) *~ */*~ */*.o
