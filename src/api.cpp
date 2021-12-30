#include "types.hpp"
#include "db.hpp"
#include "common.hpp"
#include "html.hpp"
#include "auth.hpp"
#include <stdlib.h>
#include <string.h>
#include <cgi.h>

bool cgi_is_init = false;


void ERR (err_t id, const char* msg) {
	if (id) {
		if (cgi_is_init) {
			printf ("\nERROR: %i\n", id);
			cgi_fatal (msg);
		} else {
			fprintf (stdout, "\n\n%i\n%s\n", id, msg);
			exit (1);
		}
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
	
	void login_user (void) {
		char *username = cgi_param ("uname"); ERR (username == NULL, "Missing username");
		char *password = cgi_param ("password"); ERR (password == NULL, "Missing password");
		
		char *newacc = cgi_param ("new"); if (newacc == NULL) newacc = "";
		
		if (!strcmp (newacc, "on")) {
			userid_t u;
			
			ERR (db::newuser (username, &u), "User already exists");
			ERR (auth::adduserpass (u, password), "General error: auth::adduserpass");
		} else {
			userid_t u;
			
			ERR (db::get_userid (username, &u), "Incorrect username");
			ERR (auth::checkpass (u, password), "Incorrect password");
			
			printf ("Login successful (no token generated)\n");
			
		}
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
	
	if (!strcmp (method, "GET")) {
		char *action = cgi_param ("act");
		ERR (action == NULL, "'act' parameter missing");
	
		if (!strcmp (action, "post"))
			action::get_post ();
		else
		
		if (!strcmp (action, "user"))
			action::get_user ();
		else
			cgi_fatal ("Unknown action for GET");
		
	} else
	if (!strcmp (method, "POST")) {
		char *query = getenv ("QUERY_STRING");
		
		char *action = new char [strlen (query)];
		ERR (sscanf (query, "act=%s", action) == EOF, "Syntax error");
		
		
		if (!strcmp (action, "login"))
			action::login_user ();
		else
			cgi_fatal ("Unknown action for POST");
		
		delete[] action;
	
	} else
		cgi_fatal ("Unknown method");
	
	db::deinit ();
		
}
