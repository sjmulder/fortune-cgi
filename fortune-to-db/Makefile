PKGS=		sqlite3

CFLAGS.pkg!=	pkg-config ${PKG_FLAGS} --cflags ${PKGS}
LDLIBS.pkg!=	pkg-config ${PKG_FLAGS} --libs ${PKGS}

CFLAGS+=	${CFLAGS.pkg} -Wall -Wextra
LDLIBS+=	${LDLIBS.pkg}

all: fortune-to-db

clean:
	rm -f fortune-to-db *.o

.PHONY: all clean
