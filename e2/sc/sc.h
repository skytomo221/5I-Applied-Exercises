#define ERROR_OVERFLOW 1

#define TOKEN_IF 256
#define TOKEN_THEN 257
#define TOKEN_ELSE 258
#define TOKEN_WHILE 259
#define TOKEN_DO 260
#define TOKEN_REPEAT 261
#define TOKEN_UNTIL 262
#define TOKEN_GOTO 263
#define TOKEN_BEGIN 264
#define TOKEN_END 265
#define TOKEN_LONG 266
#define TOKEN_WORD 267
#define TOKEN_BYTE 268
#define TOKEN_AND 269
#define TOKEN_OR 270
#define TOKEN_NOT 271

#define MAX_KEYWORDS 20

#define ALPHABET "abcdefghijklmnopqrstuvwxyz"
#define DIGIT "0123456789"

#define TOKEN_EOF -1
#define TOKEN_ERROR 0
#define TOKEN_ID 1

typedef struct {
	char *name;
	int value;
} keyword;
extern keyword keywords[];

extern char lexeme[];
extern int token;

/* scanner.c */
extern void get_token();
extern void print_token();
extern void convert(FILE *in, FILE *out);
/* keywords.c */
extern int find_keywords(char *the_name);
extern void setup_keywords();
/* misc.c */
extern int char_pos(char *s, char c);
extern void error(int type, char *hint);
