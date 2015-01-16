CFLAGS   = -Wall -std=c99
CPPFLAGS = -DNDEBUG
LDFLAGS  = -lm

INSTALL     = install
INSTALL_BIN = $(INSTALL) -D -m 755

prefix  = /usr/local
bindir  = $(prefix)/bin

.PHONY: all
all: src/wave

src/wave: src/wave.c

.PHONY: install
install: all
	$(INSTALL_BIN) src/wave $(bindir)/wave

.PHONY: uninstall
uninstall:
	$(RM) $(bindir)/wave

.PHONY: clean
clean:
	$(RM) src/wave
