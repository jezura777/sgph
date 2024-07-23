.POSIX:

include config.mk

COMPONENTS = util

all: sgph

util.o: util.c config.h util.h config.mk
main.o: main.c config.h util.h arg.h config.mk

sgph: config.h $(COMPONENTS:=.o) $(COMPONENTS:=.h) main.o config.mk
	$(CC) -o $@ $(CPPFLAGS) $(CFLAGS) $(COMPONENTS:=.o) main.o $(LDFLAGS)

config.h:
	cp config.def.h $@

clean:
	rm -rf quark main.o $(COMPONENTS:=.o)
