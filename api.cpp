#include "types.hpp"
#include "db.hpp"
#include "common.hpp"
#include "html.hpp"
#include <stdlib.h>
#include <string.h>
#include <cgi.h>

#define ERR(c,str) \
	if ((c)) ERROR (str);

bool cgi_is_init = false;


void ERROR (const char* msg) {
	if (cgi_is_init)
		cgi_fatal (msg);
	else {
		fprintf (stdout, "\n\n%s\n", msg);
		exit (1);
	}
}


namespace action {
	void get_post (void) {
		postid_t id;
		
		{	char *str = cgi_param ("post");
			ERR (str == NULL, "'post' parameter missing");
			
			id = strtol (str, NULL, 10);
		}
		
		post post; user author;
		post.author = &author;
		ERR (db::get_post2 (id, &post), "Could not find post");
		
		html::renderpost (&post);
		
		db::free_strs ();
	}
	
	

	void get_user (void) {
		userid_t id;
		
		{	char *str = cgi_param ("user");
			ERR (str == NULL, "'user' parameter missing");
			
			id = strtol (str, NULL, 10);
		}
		
		user user;
		ERR (db::get_user (id, &user), "Could not find user");
		
		html::renderuser (&user);
		
		db::free_strs ();
	}
}

int main (void) {
	cgi_init (); //ERR (cgi_init () != 1, "cgi_init");
	cgi_process_form ();
	cgi_init_headers ();
	cgi_is_init = true;
	
	ERR (db::init (), "Failed to initialize db");
	
	char *method = getenv ("REQUEST_METHOD");
	ERR (method == NULL, "REQUEST_METHOD is undefined");
	
	char *action = cgi_param ("act");
	ERR (action == NULL, "'act' parameter missing");
	
	if (!strcmp (method, "GET")) {
	
		if (!strcmp (action, "post"))
			action::get_post ();
		else
		
		if (!strcmp (action, "user"))
			action::get_user ();
		else
			cgi_fatal ("Unknown action for GET");
		
	} else
	if (!strcmp (method, "POST")) {
		
		//if (!strcmp (action, "post"))
			//action::post_post ();
		//else
			cgi_fatal ("Unknown action for POST");
	
	} else
		cgi_fatal ("Unknown method");
		
}
