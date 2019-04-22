#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define ERROR_OVERFLOW 1

void error(int type, char *hint)
{
    if (type == ERROR_OVERFLOW)
    {
        fprintf(stderr, "[%s] overflowed.\n", hint);
    }
    else
    {
        fprintf(stderr, "error: Unknown error.\n");
    }
}

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

typedef struct
{
    char *name;
    int value;
} keyword;
keyword keywords[MAX_KEYWORDS];
int end_of_keywords;

char *name[MAX_KEYWORDS]; /* keyword = (name, value) */
int value[MAX_KEYWORDS];
int end_of_keywords;

int find_keywords(char *the_name)
{
    int i;

    for (i = 0; i < end_of_keywords; i++)
    {
        if (strcmp(keywords[i].name, the_name) == 0)
        {
            return i;
        }
    }

    return -1;
}

int enter_keywords(char *the_name, int the_value)
{
    if (end_of_keywords >= MAX_KEYWORDS)
    {
        error(ERROR_OVERFLOW, "keywords");
        return -1;
    }

    keywords[end_of_keywords].name = strdup(the_name);
    keywords[end_of_keywords].value = the_value;

    return end_of_keywords++;
}

void setup_keywords()
{
    end_of_keywords = 0;
    enter_keywords("if", TOKEN_IF);
    enter_keywords("then", TOKEN_THEN);
    enter_keywords("else", TOKEN_ELSE);
    enter_keywords("while", TOKEN_WHILE);
    enter_keywords("do", TOKEN_DO);
    enter_keywords("repeat", TOKEN_REPEAT);
    enter_keywords("until", TOKEN_UNTIL);
    enter_keywords("goto", TOKEN_GOTO);
    enter_keywords("begin", TOKEN_BEGIN);
    enter_keywords("end", TOKEN_END);
    enter_keywords("long", TOKEN_LONG);
    enter_keywords("word", TOKEN_WORD);
    enter_keywords("byte", TOKEN_BYTE);
    enter_keywords("and", TOKEN_AND);
    enter_keywords("or", TOKEN_OR);
    enter_keywords("not", TOKEN_NOT);
}

/*------------------------------------------------------------*/

int char_pos(char *s, char c)
{
    int i;

    for (i = 0; s[i] != '\0'; i++)
    {
        if (s[i] == c)
        {
            return i;
        }
    }
    return -1;
}

/*------------------------------------------------------------*/
FILE *lexin;
FILE *lexout;
char c;

void get_char()
{
    c = fgetc(lexin);
    return;
}

/*------------------------------------------------------------*/

int end_of_lexeme;
char lexeme[BUFSIZ];

void save_char(char c)
{
    lexeme[end_of_lexeme++] = c;
    lexeme[end_of_lexeme] = '\0';
}

/*------------------------------------------------------------*/

#define ALPHABET "abcdefghijklmnopqrstuvwxyz"
#define DIGIT "0123456789"

#define TOKEN_EOF -1
#define TOKEN_ERROR 0
#define TOKEN_ID 1

int token;

void get_token()
{
    int i;

state0:
    end_of_lexeme = 0;
    if (char_pos(ALPHABET, tolower(c)) >= 0)
    {
        save_char(c);
        get_char();
        goto state1;
    }
    else if (c == EOF)
    {
        save_char(c);
        token = TOKEN_EOF;
        goto final;
    }
    else
    {
        save_char(c);
        get_char();
        token = TOKEN_ERROR;
        goto final;
    }
state1:
    if (char_pos(ALPHABET, tolower(c)) >= 0)
    {
        save_char(c);
        get_char();
        goto state1;
    }
    else if (char_pos(DIGIT, c) >= 0)
    {
        save_char(c);
        get_char();
        goto state1;
    }
    else
    {
        if ((i = find_keywords(lexeme)) >= 0)
        {
            token = keywords[i].value;
        }
        else
        {
            token = TOKEN_ID;
        }
        goto final;
    }
final:
    return;
}

void print_token()
{
    if (token == TOKEN_EOF)
    {
        fprintf(lexout, "EOF\n");
    }
    else if (token == TOKEN_ERROR)
    {
        fprintf(lexout, "ERROR\t[%s]\n", lexeme);
    }
    else if (token == TOKEN_ID)
    {
        fprintf(lexout, "ID\t[%s]\n", lexeme);
    }
    else if (token == TOKEN_IF)
    {
        fprintf(lexout, "IF\t[%s]\n", lexeme);
    }
    else if (token == TOKEN_THEN)
    {
        fprintf(lexout, "THEN\t[%s]\n", lexeme);
    }
    else if (token == TOKEN_ELSE)
    {
        fprintf(lexout, "ELSE\t[%s]\n", lexeme);
    }
    else if (token == TOKEN_WHILE)
    {
        fprintf(lexout, "WHILE\t[%s]\n", lexeme);
    }
    else if (token == TOKEN_DO)
    {
        fprintf(lexout, "DO\t[%s]\n", lexeme);
    }
    else if (token == TOKEN_REPEAT)
    {
        fprintf(lexout, "REPEAT\t[%s]\n", lexeme);
    }
    else if (token == TOKEN_UNTIL)
    {
        fprintf(lexout, "UNTIL\t[%s]\n", lexeme);
    }
    else if (token == TOKEN_GOTO)
    {
        fprintf(lexout, "GOTO\t[%s]\n", lexeme);
    }
    else if (token == TOKEN_BEGIN)
    {
        fprintf(lexout, "BEGIN\t[%s]\n", lexeme);
    }
    else if (token == TOKEN_END)
    {
        fprintf(lexout, "END\t[%s]\n", lexeme);
    }
    else if (token == TOKEN_LONG)
    {
        fprintf(lexout, "LONG\t[%s]\n", lexeme);
    }
    else if (token == TOKEN_WORD)
    {
        fprintf(lexout, "WORD\t[%s]\n", lexeme);
    }
    else if (token == TOKEN_BYTE)
    {
        fprintf(lexout, "BYTE\t[%s]\n", lexeme);
    }
    else if (token == TOKEN_AND)
    {
        fprintf(lexout, "AND\t[%s]\n", lexeme);
    }
    else if (token == TOKEN_OR)
    {
        fprintf(lexout, "OR\t[%s]\n", lexeme);
    }
    else if (token == TOKEN_NOT)
    {
        fprintf(lexout, "NOT\t[%s]\n", lexeme);
    }
    else
    {
        fprintf(lexout, "print_token: Unknown token.\n");
    }
}

/*------------------------------------------------------------*/

void convert(FILE *in, FILE *out)
{
    lexin = in;
    lexout = out;
    setup_keywords();
    get_char();
    get_token();
    while (token != TOKEN_EOF)
    {
        print_token();
        get_token();
    }
}

/*------------------------------------------------------------*/

int main(int argc, char *argv[])
{
    FILE *in;
    FILE *out;

    if (argc == 3)
    {
        if ((in = fopen(argv[1], "r")) == NULL)
        {
            perror("fopen for input");
            exit(1);
        }
        if ((out = fopen(argv[2], "w")) == NULL)
        {
            perror("fopen for output");
            exit(1);
        }
    }
    else if (argc == 2)
    {
        if ((in = fopen(argv[1], "r")) == NULL)
        {
            perror("fopen for input");
            exit(1);
        }
        out = stdout;
    }
    else
    {
        fprintf(stderr, "use: %s input [output]\n", argv[0]);
        exit(1);
    }

    convert(in, out);

    fclose(in);
    fclose(out);
    return 0;
}
