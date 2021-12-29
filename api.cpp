#include <cgi.h>

#define ERR(c,str) \
	if ((c)) cgi_fatal ("Error in "#str);;

int main (void) {
	if (cgi_init ()) {
		puts ("Error in cgi_init");
		return 1;
	}
}
