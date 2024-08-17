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
	rm -rf sgph main.o $(COMPONENTS:=.o)

install: all
	mkdir -p ${DESTDIR}${PREFIX}/bin
	cp -f sgph ${DESTDIR}${PREFIX}/bin
	chmod 755 ${DESTDIR}${PREFIX}/bin/sgph

uninstall:
	rm -f ${DESTDIR}${PREFIX}/bin/sgph\
