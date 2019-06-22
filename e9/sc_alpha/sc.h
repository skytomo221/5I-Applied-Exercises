#define WHITESPACE " \t\n"
#define ALPHABET "abcdefghijklmnopqrstuvwxyz_"
#define DIGIT "0123456789"
#define HEXDIGIT "0123456789abcdef"

#define TOKEN_EOF -1
#define TOKEN_ERROR 0
#define TOKEN_ID 1
#define TOKEN_NUM 2
#define TOKEN_COL 3
#define TOKEN_SEMICOL 4
#define TOKEN_COLEQ 5
#define TOKEN_EQ 6
#define TOKEN_NE 7
#define TOKEN_LT 8
#define TOKEN_LE 9
#define TOKEN_GT 10
#define TOKEN_GE 11
#define TOKEN_LPAR 12
#define TOKEN_RPAR 13
#define TOKEN_PLUS 14
#define TOKEN_MINUS 15
#define TOKEN_ASTER 16
#define TOKEN_SLASH 17
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

#define MAX_ERRORCOUNT 10
#define ERROR_OVERFLOW 1
#define ERROR_SYNTAX 2
#define ERROR_UNDEFINED 3
#define ERROR_DOUBLE 4
#define ERROR_TYPE 5
#define ERROR_INTERNAL 6

#define MAX_CONSTANTS 1024
#define MAX_SYMBOLS 1024

#define TYPE_LONG 1
#define TYPE_WORD 2
#define TYPE_BYTE 3
#define OFFSET_AUTO -1

#define UNDEFINED -1

//#define DEBUG3 1
//#define DEBUG4 1

typedef struct
{
	char *name;
	int value;
} keyword;
extern keyword keywords[];

extern char lexeme[];
extern int token;
extern int lineno;

typedef struct
{
	char *value;
	char *label;
} constant;
extern constant constants[];

typedef struct
{
	char *label;
	int type;
	int size;
	int offset;
} symbol;
extern symbol symbols[];

/* scanner.c */
extern void get_token();
extern void print_token();
extern void convert(FILE *in, FILE *out);
extern char lexvalue[];
/* keywords.c */
extern int find_keywords(char *the_name);
extern void setup_keywords();
/* misc.c */
extern int char_pos(char *s, char c);
extern void error(int type, char *hint, int lineno);
extern char *new_label();
/* parser.c */
extern void gen_code(char *op, char *opr);
extern void parse();
/* encoder.c */
extern void convert2(FILE *in, FILE *out);
/* constants.c */
extern int find_constants(char *value);
extern int enter_constants(char *value);
extern void setup_constants();
extern void encode_constants(FILE *out);
/* symbols.c */
extern int find_symbols(char *label);
extern int enter_symbols(char *label, int type);
extern void setup_symbols();
extern void encode_symbols(FILE *out);

