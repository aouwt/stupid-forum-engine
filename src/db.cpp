#include "db.hpp"
#include <sqlite3.h>
#include <string.h>
#include <stdio.h>

sqlite3 *db::sqldb;
sqlite3_stmt *stmt;

err_t db::init (void) {
	if (sqlite3_open_v2 ("/usr/share/cgi-data/forum/stuff.db", &db::sqldb, SQLITE_OPEN_READWRITE, NULL))
		return 1;
		
	return 0;
}


void db::deinit (void) {
	sqlite3_close (db::sqldb);
}

err_t db::newuser (const char *uname, userid_t *user) {
	size_t strlen_uname = strlen (uname);
	sqlite3_stmt* stmt;
	
	// check if already extant
	if (sqlite3_prepare_v2 (db::sqldb, "SELECT id FROM users WHERE name = ?;", -1, &stmt, NULL))
		return 1;
	
	if (sqlite3_bind_text (stmt, 1, uname, strlen_uname, SQLITE_STATIC))
		return 2;
	
	if (sqlite3_step (stmt) != SQLITE_DONE) {
		sqlite3_finalize (stmt);
		return -1; // user exists
	}
	
	sqlite3_finalize (stmt);
	
	
	// find next userid
	if (sqlite3_prepare_v2 (db::sqldb, "SELECT id FROM users ORDER BY id DESC;", -1, &stmt, NULL))
		return 3;
	
	if (sqlite3_step (stmt) == SQLITE_ROW) {
		*user = sqlite3_column_int (stmt, 0) + 1;
	} else
		return 4;
	
	sqlite3_finalize (stmt);
	
	
	// add user
	if (sqlite3_prepare_v2 (db::sqldb, "INSERT INTO users (id, name, fname) VALUES (?, ?, ?);", -1, &stmt, NULL))
		return 5;
	
	if (sqlite3_bind_int (stmt, 1, *user))
		return 6;
	if (sqlite3_bind_text (stmt, 2, uname, strlen_uname, SQLITE_STATIC))
		return 7;
	if (sqlite3_bind_text (stmt, 3, uname, strlen_uname, SQLITE_STATIC))
		return 8;
	
	if (sqlite3_step (stmt) != SQLITE_DONE)
		return 9;
	
	sqlite3_finalize (stmt);
	return 0;
}





err_t db::get_post (postid_t id, post *post) {
	if (sqlite3_prepare_v2 (db::sqldb, "SELECT author_id, date, title, body FROM posts WHERE id = ?;", -1, &stmt, NULL))
		return 1;

	if (sqlite3_bind_int (stmt, 1, id))
		return 2;


	if (sqlite3_step (stmt) != SQLITE_ROW)
		return -1;

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
	if (sqlite3_prepare_v2 (db::sqldb, "SELECT name, fname FROM users WHERE id = ?;", -1, &stmt, NULL))
		return 1;

	if (sqlite3_bind_int (stmt, 1, id))
		return 2;


	if (sqlite3_step (stmt) != SQLITE_ROW)
		return -1;

	user -> id = id;
	user -> name = (char*) sqlite3_column_text (stmt, 0);
	user -> fname = (char*) sqlite3_column_text (stmt, 1);

	return 0;
}


err_t db::get_userid (const char* uname, userid_t* id) {
	sqlite3_stmt *stmt;
	if (sqlite3_prepare_v2 (db::sqldb, "SELECT id FROM users WHERE name = ?;", -1, &stmt, NULL))
		return 1;
	
	if (sqlite3_bind_text (stmt, 1, uname, strlen (uname), SQLITE_STATIC))
		return 2;
	
	if (sqlite3_step (stmt) != SQLITE_ROW)
		return -1;
	
	*id = sqlite3_column_int (stmt, 1);
	
	sqlite3_finalize (stmt);
	
	return 0;
}


void db::free_strs (void) {
	sqlite3_finalize (stmt);
}
