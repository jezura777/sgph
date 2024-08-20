VERSION   = 0
PROG      = sgph

CPPFLAGS  = -DVERSION=\"$(VERSION)\" -D_DEFAULT_SOURCE -D_XOPEN_SORCE=700 -D_BSD_SOURCE
CFLAGS   ?= -std=c99 -pedantic -Wall -Wextra -Os
LDFLAGS  ?= -s

PREFIX   ?= /usr/local

CC       ?= cc
