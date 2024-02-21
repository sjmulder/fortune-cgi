CFLAGS+=	-Wall -Wextra
LDLIBS+=	-lsqlite3 -lyajl

all: fortune.cgi

clean:
	rm -f fortune.cgi *.o

fortune.cgi: fortune-cgi.c
	${LINK.c} -o fortune.cgi fortune-cgi.c ${LDLIBS}

.PHONY: all clean
