FORTUNE_FILE?=	/usr/share/games/fortune*/fortunes

all: progs fortune.db

clean:
	${MAKE} -C fortune-to-db
	${MAKE} -C fortune-cgi
	rm -f fortune.db

progs:
	${MAKE} -C fortune-to-db
	${MAKE} -C fortune-cgi

fortune.db:
	fortune-to-db/fortune-to-db ${FORTUNE_FILE} fortune.db

.PHONY: all progs
