FORTUNE_FILE?=	/usr/share/games/fortune*/fortunes

all: progs fortune.db

clean:
	${MAKE} -C fortune-to-db clean
	${MAKE} -C fortune-cgi clean
	rm -f fortune.db

progs:
	${MAKE} -C fortune-to-db
	${MAKE} -C fortune-cgi

fortune.db: fortune-to-db/fortune-to-db ${FORTUNE_FILE}
	rm -f fortune.db  # replace, don't append
	fortune-to-db/fortune-to-db ${FORTUNE_FILE} fortune.db

.PHONY: all clean progs
