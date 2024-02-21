#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <sysexits.h>
#include <err.h>
#include <sqlite3.h>

#define MAX_FORTUNE	4096

static const char *
read_fortune(FILE *fin)
{
	static char buf[MAX_FORTUNE];
	size_t len=0;

	if (feof(fin))
		return NULL;
	
	while (1) {
		if (len+1 >= sizeof(buf))
			errx(1, "buffer too small");
		if (!fgets(buf+len, sizeof(buf)-len, fin))
			break;
		if (buf[len] == '%') {
			buf[len] = '\0';
			break;
		}

		len = strlen(buf);
	}

	if (!len)
		return NULL;

	return buf;
}

static void
init_schema(sqlite3 *db)
{
	static const char sql[] =
"create table if not exists fortunes ("
"  id integer primary key,"
"  fortune text not null"
")";

	sqlite3_stmt *stmt;

	if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK)
		errx(1, "%s", sqlite3_errmsg(db));
	if (sqlite3_step(stmt) != SQLITE_DONE)
		errx(1, "%s", sqlite3_errmsg(db));
	if (sqlite3_finalize(stmt) != SQLITE_OK)
		errx(1, "%s", sqlite3_errmsg(db));
}

static sqlite3_stmt *
prep_insert(sqlite3 *db)
{
	static const char sql[] =
"INSERT INTO fortunes (fortune) values (?)";

	sqlite3_stmt *stmt;

	if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK)
		errx(1, "%s", sqlite3_errmsg(db));
	
	return stmt;
}

static void
insert_fortune(sqlite3 *db, sqlite3_stmt *insert, const char *fortune)
{
	if (sqlite3_bind_text(insert, 1, fortune, -1, SQLITE_STATIC)
	    != SQLITE_OK)
		errx(1, "%s", sqlite3_errmsg(db));

	if (sqlite3_step(insert) != SQLITE_DONE)
		errx(1, "%s", sqlite3_errmsg(db));

	if (sqlite3_reset(insert) != SQLITE_OK)
		errx(1, "%s", sqlite3_errmsg(db));
	if (sqlite3_clear_bindings(insert) != SQLITE_OK)
		errx(1, "%s", sqlite3_errmsg(db));
}

int
main(int argc, char **argv)
{
	char *in_path, *db_path;
	const char *fortune;
	int c, res;
	size_t count;
	FILE *fin;
	sqlite3 *db;
	sqlite3_stmt *insert;

	while ((c = getopt(argc, argv, "")) != -1)
		exit(EX_USAGE);
	
	argc -= optind;
	argv += optind;

	if (argc != 2)
		errx(EX_USAGE, "usage: fortune-to-db <fortune> <file.db>");
	
	in_path = argv[0];
	db_path = argv[1];
	
	if (!(fin = fopen(in_path, "r")))
		err(1, "%s", in_path);
	if ((res = sqlite3_open(db_path, &db)) != SQLITE_OK)
		errx(1, "%s: %s", db_path, sqlite3_errstr(res));
	
	init_schema(db);
	insert = prep_insert(db);
	
	for (count=0; (fortune = read_fortune(fin)); count++)
		insert_fortune(db, insert, fortune);
	
	printf("inserted %zu fortunes\n", count);

	if (sqlite3_finalize(insert) != SQLITE_OK)
		errx(1, "%s", sqlite3_errmsg(db));
	if (sqlite3_close(db) != SQLITE_OK)
		errx(1, "%s", sqlite3_errmsg(db));
	
	fclose(fin);
	
	return 0;
}
