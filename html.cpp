#include "html.hpp"
#include "db.hpp"
#include <stdio.h>
#include <sqlite3.h>

#define POST_URL_BEGIN "/cgi-bin/fourm.cgi?act=post&post="
#define POST_URL_END ""
#define USER_URL_BEGIN "/cgi-bin/fourm.cgi?act=user&user="
#define USER_URL_END ""

void datestr (time_t d, char** c) {
	
}

void html::renderpost (post* p) {
	printf ("<p><h1><a href=\"%s%i%s\">%s</a></h1> by <a href=\"%s%s%s\">%s</a></p>",
		POST_URL_BEGIN, p -> id, POST_URL_END,
		p -> title,
		USER_URL_BEGIN, p -> author -> name, USER_URL_END,
		p -> author -> fname
	);

	{	char date [30];
		datestr (p -> date, &date);
		printf ("<i>%s</i>", date);
	}

	printf ("<p>%s</p>", p -> body);
}

void html::renderuser (user* u) {
	printf ("<p><h1>%s</h1><i><a href=\"%s%s%s\">@%s</a></i></p>",
		u -> fname,
		USER_URL_BEGIN, u -> name, USER_URL_END,
		u -> name
	);
	
	
	sqlite3_stmt* stmt;
	post post;
	
	if (sqlite3_prepare_v2 (db::sqldb, "SELECT id, date, title FROM posts WHERE author_id = ?;", -1, &stmt, NULL))
		return;

	if (sqlite3_bind_int (stmt, 1, u -> id))
		return;

	for (;;) {
		switch (sqlite3_step (stmt)) {
			case SQLITE_ROW:
				printf ("<div><i>%s</i><h2><a href=\"%s%i%s\">%s</a></h2></div>",
					datestr (sqlite3_column_int64 (stmt, 1)),
					POST_URL_BEGIN, sqlite3_column_int (stmt, 0), POST_URL_END,
					sqlite3_column_text (stmt, 2)
				);
				break;
			
			case SQLITE_DONE:
				return;
			
			default:
				return;
		}
	}
	
	sqlite3_finalize (stmt);
}
