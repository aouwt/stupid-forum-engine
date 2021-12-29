#include "common.hpp"
#include "api.hpp"
#include <cgi.h>
#include <stdlib.h>

void ERROR (const char* msg) {
	if (cgi_is_init)
		cgi_fatal (msg);
	else {
		fputs (stderr, msg);
		exit (1);
	}
}
