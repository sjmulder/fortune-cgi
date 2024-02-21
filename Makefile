all: progs fortune.db

clean:
	${MAKE} -C fortune-to-db
	${MAKE} -C fortune-cgi
	rm -f fortune.db

progs:
	${MAKE} -C fortune-to-db
	${MAKE} -C fortune-cgi

fortune.db:
	fortune-to-db/fortune-to-db /usr/share/games/fortunes/fortunes fortune.db

.PHONY: all progs
