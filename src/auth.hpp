#ifndef AUTH_HPP
#define AUTH_HPP
	#include "types.hpp"
	
	#define TOKEN_EXPIRY 60*60*60*24*7*2
	namespace auth {
		token_t gentoken (void);
		auth_user authuser (userid_t user);
		err_t checkpass (userid_t user, const char* pass);
		err_t mkhash (const char* salt, const char* pass, hash_t hash);
		err_t adduserpass (userid_t user, const char* pass);
		err_t mkhash (salt_t* salt, const char* pass, hash_t hash);
		salt_t mksalt (void);
	}

#endif
