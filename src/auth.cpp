#include <cgi.h>
#include <time.h>
#include <sqlite3.h>
#include <random.hpp>
#include <argon2.h>
#include <string.h>

#include "db.hpp"
#include "auth.hpp"
#include "common.hpp"
#include "types.hpp"

random rand;

token_t auth::gentoken (void) {
	token_t t;
	rand.rand (sizeof (token_t), &t);
	return t;
}

err_t _validatetok (token_t tok, userid_t who) {
	sqlite3_stmt* stmt;
	
	if (sqlite3_prepare_v2 (db::sqldb,
			"DELETE FROM auth_users WHERE (expiry < ?);"
			"SELECT * FROM auth_users WHERE (user = ?, token_1 = ?, token_2 = ?);",
			 -1, &stmt, NULL))
		return 1;
	
	
	if (sqlite3_bind_int64 (stmt, 4, (sqlite3_int64) time (NULL)))
		return 2;
		
	if (sqlite3_bind_int (stmt, 2, (sqlite3_int64) who))
		return 3;
	
	if (sqlite3_bind_int64 (stmt, 3, (sqlite3_int64) tok [0]))
		return 4;
	
	if (sqlite3_bind_int64 (stmt, 4, (sqlite3_int64) tok [1]))
		return 5;
	
	
	if (sqlite3_step (stmt) == SQLITE_DONE)
		return 6;
	
	sqlite3_finalize (stmt);
	return 0;
}

err_t addauthtoken (auth_user* a) {
	sqlite3_stmt* stmt;
	
	if (sqlite3_prepare_v2 (db::sqldb, "INSERT INTO auth_users (user, token_1, token_2, expiry) VALUES (?, ?, ?, ?);", -1, &stmt, NULL))
		return 1;

	if (sqlite3_bind_int (stmt, 1, a -> user))
		return 2;
	
	if (sqlite3_bind_int64 (stmt, 2, (sqlite3_int64) a -> token [0]))
		return 3;
	
	if (sqlite3_bind_int64 (stmt, 3, (sqlite3_int64) a -> token [1]))
		return 4;
	
	if (sqlite3_bind_int64 (stmt, 4, (sqlite3_int64) a -> expiry))
		return 5;
	
	if (sqlite3_step (stmt) != SQLITE_DONE)
		return 6;
	
	sqlite3_finalize (stmt);

	return 0;
}


auth_user auth::authuser (userid_t user) {
	auth_user a;
	
	a.user = user;
	a.token = auth::gentoken ();
	a.expiry = time (NULL) + TOKEN_EXPIRY;
	
	addauthtoken (&a);
	return a;
}


salt_t auth::mksalt (void) {
	salt_t s;
	/*for (size_t i = 0; i != SALTLEN; i ++)
		s [i] = ((rand.randi () % (127 - 32)) + 32);

	s [SALTLEN] = '\0';
	*/
	rand.rand (sizeof (salt_t), &s);
	return s;
}


err_t auth::checkpass (userid_t user, const char* pass) {
	sqlite3_stmt *stmt;
	
	if (sqlite3_prepare_v2 (db::sqldb, "SELECT hash FROM passwords WHERE user = ?;", -1, &stmt, NULL))
		return 1;
	
	if (sqlite3_bind_int (stmt, 1, user))
		return 2;
	
	if (sqlite3_step (stmt) != SQLITE_ROW)
		return -1;
	
	char* correcthash = (char*) sqlite3_column_text (stmt, 1);

	return 0;
	
	sqlite3_finalize (stmt);
}



err_t auth::mkhash (salt_t* salt, const char* pass, hash_t hash) {
	return argon2i_hash_encoded (
		5,	// time cost
		8192,	// memory cost
		1,	// parallelism
		pass, sizeof (char) * strlen (pass),
		salt, sizeof (salt_t),
		HASHSIZE,
		hash, HASHLEN * sizeof (char)
	);
}


err_t auth::adduserpass (userid_t user, const char* pass) {

	salt_t salt = auth::mksalt ();
	hash_t phash; return (auth::mkhash (&salt, pass, phash)) ;return 6;
	
	
	sqlite3_stmt* stmt;
	
	if (sqlite3_prepare_v2 (db::sqldb, "INSERT INTO passwords (user, hash, salt) VALUES (?, ?, ?);", -1, &stmt, NULL))
		return 1;
	
	if (sqlite3_bind_int (stmt, 1, user))
		return 2;
	
	if (sqlite3_bind_text (stmt, 2, (const char*) phash, strlen (phash) * sizeof (char), SQLITE_STATIC))
		return 3;
	
	if (sqlite3_step (stmt) != SQLITE_DONE)
		return 4;
	
	sqlite3_finalize (stmt);
	return 0;
}
