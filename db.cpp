#include "db.hpp"
#include <sqlite3.h>

sqlite3* db;

err_t db::init (void) {
	ERR (sqlite3_open ("/usr/share/cgi-data/fourm/stuff.db", &db), "sqlite3_open");
}

err_t db::get_post (postid_t id, post* post) {
	
