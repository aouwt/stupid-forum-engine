#include "db.hpp"
#include <sqlite3.h>

sqlite3* db;

err_t db::init (void) {
	ERR (sqlite3_open ("/usr/share/cgi-data/fourm/stuff.db", &db), "sqlite3_open");
}

err_t db::get_post (postid_t id, post* post) {
	sqlite3_stmt* stmt;

	if (sqlite3_prepare_v2 (db, "SELECT VALUES (author_id, date, text, body) FROM posts WHERE id = ?;", -1, &stmt, NULL))
		return 1;

	if (sqlite3_bind_int (stmt, 1, id))
		return 2;


	if (sqlite3_step (stmt) != SQLITE_ROW)
		return 3;

	post -> id = id;
	post -> author_id = sqlite3_column_int (stmt, 0);
	post -> date = sqlite3_column_int64 (stmt, 1);
	post -> text = sqlite3_column_text (stmt, 2);
	post -> body = sqlite3_column_text (stmt, 3);

	sqlite3_finalize (stmt);

	return 0;
}


err_t db::get_user (userid_t id, user* user) {
	sqlite3_stmt* stmt;

	if (sqlite3_prepare_v2 (db, "SELECT VALUES (name, fname) FROM users WHERE id = ?", -1, &stmt, NULL))
		return 1;

	if (sqlite3_bind_int (stmt, 1, id))
		return 2;


	if (sqlite3_step (stmt)	!= SQLITE_ROW)
		return 3;

	user -> id = id;
	user -> name = sqlite3_column_text (stmt, 0);
	user -> fname = sqlite3_column_text (stmt, 1);

	sqlite3_finalize (stmt);

	return 0;
}
