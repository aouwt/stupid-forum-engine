#include "db.hpp"
#include <sqlite3.h>
#include <stdio.h>

sqlite3 *db::sqldb;
sqlite3_stmt *stmt;

err_t db::init (void) {
	if (sqlite3_open ("/usr/share/cgi-data/forum/stuff.db", &db::sqldb))
		return 1;
		
	return 0;
}

err_t db::get_post (postid_t id, post *post) {
	if (sqlite3_prepare_v2 (db::sqldb, "SELECT author_id, date, title, body FROM posts WHERE id = ?;", -1, &stmt, NULL))
		return 1;

	if (sqlite3_bind_int (stmt, 1, id))
		return 2;


	if (sqlite3_step (stmt) != SQLITE_ROW)
		return 3;

	post -> id = id;
	post -> author_id = sqlite3_column_int (stmt, 0);
	post -> date = sqlite3_column_int64 (stmt, 1);
	post -> title = (char*) sqlite3_column_text (stmt, 2);
	post -> body = (char*) sqlite3_column_text (stmt, 3);

	return 0;
}


err_t db::get_post2 (postid_t id, post *post) {
	if (db::get_post (id, post))
		return 1;
	if (post -> author != NULL)
		if (db::get_user (post -> author_id, post -> author))
			return 2;
	
	return 0;
}
	

err_t db::get_user (userid_t id, user *user) {
	sqlite3_stmt* stmt;

	if (sqlite3_prepare_v2 (db::sqldb, "SELECT name, fname FROM users WHERE id = ?;", -1, &stmt, NULL))
		return 1;

	if (sqlite3_bind_int (stmt, 1, id))
		return 2;


	if (sqlite3_step (stmt) != SQLITE_ROW)
		return 3;

	user -> id = id;
	user -> name = (char*) sqlite3_column_text (stmt, 0);
	user -> fname = (char*) sqlite3_column_text (stmt, 1);

	return 0;
}

void db::free_strs (void) {
	sqlite3_finalize (stmt);
}
