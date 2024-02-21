#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>
#include <sqlite3.h>
#include <yajl/yajl_gen.h>

static void
print_callback(void *ctx, const char *str, size_t len)
{
	(void)ctx;

	fwrite(str, len, 1, stdout);
}

int
main()
{
	static const char sql[] =
"select id, fortune from fortunes order by random() limit 1";

	const char *db_path;
	const unsigned char *fortune;
	sqlite3 *db;
	sqlite3_stmt *stmt;
	yajl_gen yajl;
	int res, id;

	if (!(db_path = getenv("FORTUNE_DB")) || !db_path[0])
		db_path = "fortune.db";

	res = sqlite3_open_v2(db_path, &db, SQLITE_OPEN_READONLY, NULL);
	if (res != SQLITE_OK)
		errx(1, "%s: %s", db_path, sqlite3_errstr(res));

	if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK)
		errx(1, "%s", sqlite3_errmsg(db));
	if (sqlite3_step(stmt) != SQLITE_ROW)
		errx(1, "%s", sqlite3_errmsg(db));

	id = sqlite3_column_int(stmt, 0);
	fortune = sqlite3_column_text(stmt, 1);

	if (!(yajl = yajl_gen_alloc(NULL)))
		errx(1, "yajl_gen_alloc() failed");
	if (!yajl_gen_config(yajl, yajl_gen_beautify, 1))
		errx(1, "yajl_gen_config(beautify) failed");
	if (!yajl_gen_config(yajl, yajl_gen_print_callback,
	    print_callback, NULL))
		errx(1, "yajl_gen_config(print_callback) failed");

	printf("Content-Type: text/json\r\n");
	printf("\r\n");
	
	yajl_gen_map_open(yajl);
	yajl_gen_string(yajl, (const unsigned char *)"id", 2);
	yajl_gen_integer(yajl, id);
	yajl_gen_string(yajl, (const unsigned char *)"fortune", 7);
	yajl_gen_string(yajl, fortune, strlen((const char *)fortune));
	yajl_gen_map_close(yajl);
	yajl_gen_free(yajl);

	if (sqlite3_step(stmt) != SQLITE_DONE)
		errx(1, "%s", sqlite3_errmsg(db));
	if (sqlite3_finalize(stmt) != SQLITE_OK)
		errx(1, "%s", sqlite3_errmsg(db));
	if (sqlite3_close(db) != SQLITE_OK)
		errx(1, "%s", sqlite3_errmsg(db));
	
	return 0;
}
