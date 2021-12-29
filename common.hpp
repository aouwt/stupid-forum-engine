#ifndef COMMON_HPP
#define COMMON_HPP
	
	#define ERR(c,str) \
		if ((c)) ERROR (str);
	
	#define _STR(x) #x
	#define STR(x) _STR (x)

	extern void ERROR (const char*);
	
#endif
