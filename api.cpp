#include <cgi.h>

#define ERR(c,str) \
	if ((c)) cgi_fatal ("Error: "#str);;

int main (void) {
	if (cgi_init ()) {
		puts ("Error: cgi_init");
		return 1;
	}
	
	char *method = getenv ("REQUEST_METHOD");
	ERR (method == NULL, "REQUEST_METHOD is undefined.");
	
	char *action = cgi_param ("act");
	ERR (action == NULL, "'act' parameter missing");
	
	if (method == "GET") {
	
		if (action == "post")
			action::get_post ();
		else
		
		if (action == "user")
			action::get_user ();
		else
			cgi_fatal ("Unknown action for GET");
		
	} else
	if (method == "POST") {
		
		if (action == "post")
			action::post_post ();
		else
			cgi_fatal ("Unknown action for POST");
	
	} else
		cgi_fatal ("Unknown method");
		
}
