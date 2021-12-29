#include "types.hpp"
#include "db.hpp"
#include "common.hpp"
#include <stdlib.h>
#include <string.h>
#include <cgi.h>

bool cgi_is_init = false;

int main (void) {
	ERR (cgi_init (), "cgi_init");
	cgi_is_init = true;
	
	char *method = getenv ("REQUEST_METHOD");
	ERR (method == NULL, "REQUEST_METHOD is undefined.");
	
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
		
		if (!strcmp (action, "post"))
			action::post_post ();
		else
			cgi_fatal ("Unknown action for POST");
	
	} else
		cgi_fatal ("Unknown method");
		
}
