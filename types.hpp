typedef uint_least8_t u8;	typedef int_least8_t s8;
typedef uint_least16_t u16;	typedef int_least8_t s16;
typedef uint_least32_t u32;	typedef int_least8_t s32;
typedef uint_least64_t u64;	typedef int_least8_t s64;

typedef s16 postid_t;
typedef u16 authorid_t;

typedef struct {
	authorid_t id;
	char *name = NULL;
	char *fname = NULL;
} author;

typedef struct {
	char *url = NULL;
	char *fname = NULL;
} attachment;

typedef struct {
	postid_t id;
	time_t date;
	char *title = NULL;
	char *body = NULL;
	author *author = NULL;
} post;
