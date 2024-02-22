PKGS=		yajl sqlite3

CFLAGS.pkg!=	pkg-config ${PKG_FLAGS} --cflags ${PKGS}
LDLIBS.pkg!=	pkg-config ${PKG_FLAGS} --libs ${PKGS}

CFLAGS+=	${CFLAGS.pkg} -Wall -Wextra
LDLIBS+=	${LDLIBS.pkg}

all: fortune.cgi

clean:
	rm -f fortune.cgi *.o

fortune.cgi: fortune-cgi.c
	${LINK.c} -o fortune.cgi fortune-cgi.c ${LDLIBS}

.PHONY: all clean
