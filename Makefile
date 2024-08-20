.POSIX:

include config.mk

SRCS = main.c util.c
OBJS = ${SRCS:.c=.o}

all: ${PROG}

util.o: util.c config.h util.h config.mk
main.o: main.c config.h util.h arg.h config.mk

.c.o:
	${CC} ${CFLAGS} ${CPPFLAGS} -c $<

${PROG}: ${OBJS} config.h config.mk
	${CC} -o $@ ${CPPFLAGS} ${CFLAGS} ${OBJS} ${LDFLAGS}

config.h:
	cp config.def.h $@

clean:
	-rm ${OBJS} ${PROG}

install: all
	mkdir -p ${DESTDIR}${PREFIX}/bin
	install -m 755 ${PROG} ${DESTDIR}${PREFIX}/bin/${PROG}

uninstall:
	rm -f ${DESTDIR}${PREFIX}/bin/${PROG}

.PHONY: all clean install uninstall
