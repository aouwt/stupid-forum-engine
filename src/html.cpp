#include "html.hpp"
#include "db.hpp"
#include <stdio.h>
#include <sqlite3.h>
#include <time.h>

#define POST_URL_BEGIN "/cgi-bin/fourm.cgi?act=post&post="
#define POST_URL_END ""
#define USER_URL_BEGIN "/cgi-bin/fourm.cgi?act=user&user="
#define USER_URL_END ""

err_t datestr (time_t d, char c [], size_t len) {
	time_t now = time (NULL);
	
	time_t diff = now - d;
	
	#define _(pf) return (pf == EOF ? 1 : 0)
		if (diff < 60)
			_ (snprintf (c, len, "%i seconds ago", int (diff)));
		else
		if (diff < 60*60)
			_ (snprintf (c, len, "%i minutes ago", int (diff / 60)));
		else
		if (diff < 60*60*24)
			_ (snprintf (c, len, "%i hours ago", int (diff / (60*60))));
		else
		if (diff < 60*60*24*7)
			_ (snprintf (c, len, "%i days ago", int (diff / (60*60*24))));
		else {
			struct tm *date = localtime (&d);
			if (strftime (c, len, "%H:%M %F", date) == 0)
				return 1;
		}
		return 0;
	#undef _
}

void html::renderminipost (post* p) {
	printf ("<hr /><div>");
	
	{	char date [30];
		datestr (p -> date, date, 30);
		printf ("<i>%s</i>", date);
	}
	
	printf ("<p><h1><a href=\"%s%i%s\">%s</a></h1>",
		POST_URL_BEGIN, p -> id, POST_URL_END,
		p -> title
	);
	
	printf ("by <a href=\"%s%s%s\">%s</a></p>",
		USER_URL_BEGIN, p -> author -> name, USER_URL_END,
		p -> author -> fname
	);
	
	printf ("</div>");
}

void printposts (sqlite3_stmt* stmt) {
	for (;;) {
		switch (sqlite3_step (stmt)) {
			case SQLITE_ROW:
				{	post p; user a;
					p.author = &a;
					
					p.id = sqlite3_column_int (stmt, 0);
					p.title = (char*) sqlite3_column_text (stmt, 3);
					p.date = sqlite3_column_int64 (stmt, 2);
					p.author_id = sqlite3_column_int (stmt, 1);
					
					db::get_user (p.author_id, p.author);
					
					html::renderminipost (&p);
				}
					
				break;
			
			case SQLITE_DONE:
				return;
			
			default:
				return;
		}
	}
}

void html::renderpost (post* p) {
	printf ("<p><h1><a href=\"%s%i%s\">%s</a></h1> by <a href=\"%s%s%s\">%s</a></p>",
		POST_URL_BEGIN, p -> id, POST_URL_END,
		p -> title,
		USER_URL_BEGIN, p -> author -> name, USER_URL_END,
		p -> author -> fname
	);

	{	char date [30];
		datestr (p -> date, date, 30);
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
	
	if (sqlite3_prepare_v2 (db::sqldb, "SELECT * FROM posts WHERE author_id = ?;", -1, &stmt, NULL))
		return;

	if (sqlite3_bind_int (stmt, 1, u -> id))
		return;
	
	printposts (stmt);
	
	sqlite3_finalize (stmt);
}
