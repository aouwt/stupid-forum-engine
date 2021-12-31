#ifndef COMMON_HPP
#define COMMON_HPP
	
	#define _STR(x) #x
	#define STR(x) _STR (x)
	
	#define LEN(x) (sizeof (x) / sizeof (x [0]))
	
	#define HASHLEN 20
	#define SALTLEN 20
	
#endif
