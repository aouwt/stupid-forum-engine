#ifndef TYPES_HPP
#define TYPES_HPP
	#include <cstddef>
	#include <cstdint>
	#include <time.h>
	#include <array>
	
	typedef uint_least8_t u8;	typedef int_least8_t s8;
	typedef uint_least16_t u16;	typedef int_least8_t s16;
	typedef uint_least32_t u32;	typedef int_least8_t s32;
	typedef uint_least64_t u64;	typedef int_least8_t s64;

	typedef s16 postid_t;
	typedef u16 userid_t;
	
	// thx palaiologos
	using token_t = std::array <std::uint64_t, 2>;
	
	typedef s8 err_t;

	typedef struct {
		userid_t id;
		char *name = NULL;
		char *fname = NULL;
	} user;

	typedef struct {
		char *url = NULL;
		char *fname = NULL;
	} attachment;

	typedef struct {
		postid_t id;
		time_t date;
		char *title = NULL;
		char *body = NULL;
		userid_t author_id;
		user* author = NULL;
	} post;
	
	typedef struct {
		userid_t user;
		token_t key;
		time_t expiry;
	} auth_user;
#endif
