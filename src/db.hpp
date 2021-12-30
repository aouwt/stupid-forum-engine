#ifndef DB_HPP
#define DB_HPP
	#include "types.hpp"
	#include "common.hpp"
	#include <sqlite3.h>
	
	namespace db {
		extern err_t init (void);
		extern void deinit (void);
		
		extern err_t get_post (postid_t, post*);
		extern err_t get_user (userid_t, user*);
		extern err_t get_userid (const char*, userid_t*);
		
		extern err_t get_post2 (postid_t, post*);
		
		extern err_t newuser (const char*, userid_t*);
		
		extern void free_strs (void);
		
		extern sqlite3 *sqldb;
	}
#endif
