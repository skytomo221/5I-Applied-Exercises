#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "sc.h"

static FILE *lexin;
static FILE *lexout;
static char c;
static int end_of_lexeme;
char lexeme[BUFSIZ];
int token;

void get_char()
{
    c = fgetc(lexin);
    return;
}

/*------------------------------------------------------------*/

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

#ifdef DEBUG
    printf("[%c]: ", c);
#endif
state0:
#ifdef DEBUG
    printf("state0 ");
#endif
    end_of_lexeme = 0;
    end_of_lexeme = 0;
    if (char_pos(WHITESPACE, c) >= 0)
    {
        get_char();
        goto state0;
    }
    else if (c == '(')
    {
        save_char(c);
        get_char();
        goto state1;
    }
    else if (char_pos(ALPHABET, tolower(c)) >= 0)
    {
        save_char(c);
        get_char();
        goto state4;
    }
    else if (c == '0')
    {
        save_char(c);
        get_char();
        goto state5;
    }
    else if (char_pos(DIGIT, c) >= 0)
    {
        save_char(c);
        get_char();
        goto state8;
    }
    else if (c == '\'')
    {
        save_char(c);
        get_char();
        goto state9;
    }
    else if (c == ':')
    {
        save_char(c);
        get_char();
        goto state12;
    }
    else if (c == '<')
    {
        save_char(c);
        get_char();
        goto state13;
    }
    else if (c == '>')
    {
        save_char(c);
        get_char();
        goto state14;
    }
    else if (c == '=')
    {
        save_char(c);
        get_char();
        goto state15;
    }
    else if (c == ')')
    {
        save_char(c);
        get_char();
        token = TOKEN_RPAR;
        goto final;
    }
    else if (c == '+')
    {
        save_char(c);
        get_char();
        token = TOKEN_PLUS;
        goto final;
    }
    else if (c == '-')
    {
        save_char(c);
        get_char();
        token = TOKEN_MINUS;
        goto final;
    }
    else if (c == '*')
    {
        save_char(c);
        get_char();
        token = TOKEN_ASTER;
        goto final;
    }
    else if (c == '/')
    {
        save_char(c);
        get_char();
        token = TOKEN_SLASH;
        goto final;
    }
    else if (c == ';')
    {
        save_char(c);
        get_char();
        token = TOKEN_SEMICOL;
        goto final;
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
/* 課題演習のため空欄 */
state1:
#ifdef DEBUG
    printf("-> state1 ");
#endif
    // * (state1 -> state2)
    if (c == '*')
    {
        save_char(c);
        get_char();
        goto state2;
    }
    // Other than * (state1 -> LPAR -> final)
    else
    {
        save_char(c);
        get_char();
        token = TOKEN_LPAR;
        goto final;
    }
state2:
#ifdef DEBUG
    printf("-> state2 ");
#endif
    // * (state2 -> state3)
    if (c == '*')
    {
        save_char(c);
        get_char();
        goto state3;
    }
    // EOF (state2 -> state0)
    else if (c == EOF)
    {
        save_char(c);
        token = TOKEN_EOF;
        goto state0;
    }
    // Other than EOF, * (state2 -> state2)
    else
    {
        save_char(c);
        get_char();
        goto state2;
    }
state3:
#ifdef DEBUG
    printf("-> state3 ");
#endif
    // ) (state3 -> state0)
    if (c == ')')
    {
        save_char(c);
        get_char();
        goto state0;
    }
    // * (state3 -> state3)
    else if (c == '*')
    {
        save_char(c);
        get_char();
        goto state0;
    }
    // EOF (state3 -> state0)
    else if (c == EOF)
    {
        save_char(c);
        token = TOKEN_EOF;
        goto state0;
    }
    // Other than ), *, EOF (state3 -> state2)
    else
    {
        save_char(c);
        get_char();
        goto state2;
    }
state4:
#ifdef DEBUG
    printf("-> state4 ");
#endif
    // ALPHABET (state4 -> state4)
    // DIGIT (state4 -> state4)
    if (char_pos(ALPHABET, tolower(c)) >= 0 ||
        char_pos(DIGIT, tolower(c)) >= 0)
    {
        save_char(c);
        get_char();
        goto state4;
    }
    // Other than ALPHABET and DIGIT (state4 -> ID, IF, THEN, ... -> final)
    else
    {
        // 予約語か？
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
state5:
#ifdef DEBUG
    printf("-> state5 ");
#endif
    // x (state5 -> state6)
    if (c == 'x')
    {
        save_char(c);
        get_char();
        goto state6;
    }
    // DIGHT (state5 -> state8)
    else if (char_pos(DIGIT, tolower(c)) >= 0)
    {
        save_char(c);
        get_char();
        goto state8;
    }
    // Other than x, DIGHT (state5 -> NUM -> final)
    else
    {
        save_char(c);
        get_char();
        token = TOKEN_NUM;
        goto final;
    }
state6:
#ifdef DEBUG
    printf("-> state6 ");
#endif
    // HEXDIGIT (state6 -> state7)
    if (char_pos(HEXDIGIT, tolower(c)) >= 0)
    {
        save_char(c);
        get_char();
        goto state7;
    }
    // Other than HEXDIGIT (state6 -> ERROR -> final)
    else
    {
        save_char(c);
        get_char();
        token = TOKEN_ERROR;
        goto final;
    }
state7:
#ifdef DEBUG
    printf("-> state7 ");
#endif
    // HEXDIGIT (state7 -> state7)
    if (char_pos(HEXDIGIT, tolower(c)) >= 0)
    {
        save_char(c);
        get_char();
        goto state7;
    }
    // Other than HEXDIGIT (state7 -> NUM -> final)
    else
    {
        save_char(c);
        get_char();
        token = TOKEN_NUM;
        goto final;
    }
state8:
#ifdef DEBUG
    printf("-> state8 ");
#endif
    // DIGHT (state8 -> state8)
    if (char_pos(DIGIT, tolower(c)) >= 0)
    {
        save_char(c);
        get_char();
        goto state8;
    }
    // Other than DIGHT (state8 -> NUM -> final)
    else
    {
        save_char(c);
        get_char();
        token = TOKEN_NUM;
        goto final;
    }
state9:
#ifdef DEBUG
    printf("-> state9 ");
#endif
    // \ (state9 -> state10)
    if (c == '\\')
    {
        save_char(c);
        get_char();
        goto state10;
    }
    // ', " (state9 -> ERROR -> final)
    else if (c == '\'' || c == '\"')
    {
        save_char(c);
        get_char();
        token = TOKEN_ERROR;
        goto final;
    }
    // Othar than \, ', " (state10 -> state11)
    else
    {
        save_char(c);
        get_char();
        goto state11;
    }
state10:
#ifdef DEBUG
    printf("-> state10 ");
#endif
    // n, t, 0, ', ", \ (state10 -> state11)
    if (c == 'n' ||
        c == 't' ||
        c == '0' ||
        c == '\'' ||
        c == '\"' ||
        c == '\\')
    {
        save_char(c);
        get_char();
        goto state11;
    }
    // Other than n, t, 0, ', ", \ (state10 -> ERROR -> final)
    else
    {
        save_char(c);
        get_char();
        token = TOKEN_ERROR;
        goto final;
    }
state11:
#ifdef DEBUG
    printf("-> state11 ");
#endif
    // ' (state11 -> NUM -> final)
    if (c == '\'')
    {
        save_char(c);
        get_char();
        token = TOKEN_NUM;
        goto final;
    }
    // Othar than ' (state11 -> ERROR -> final)
    else
    {
        save_char(c);
        get_char();
        token = TOKEN_ERROR;
        goto final;
    }
state12:
#ifdef DEBUG
    printf("-> state12 ");
#endif
    // = (state12 -> COLEQ -> final)
    if (c == '=')
    {
        save_char(c);
        get_char();
        token = TOKEN_COLEQ;
        goto final;
    }
    // Other than = (state12 -> COL -> final)
    else
    {
        save_char(c);
        get_char();
        token = TOKEN_COL;
        goto final;
    }
state13:
#ifdef DEBUG
    printf("-> state13 ");
#endif
    // = (state13 -> LE -> final)
    if (c == '=')
    {
        save_char(c);
        get_char();
        token = TOKEN_LE;
        goto final;
    }
    // > (state13 -> NE -> final)
    else if (c == '>')
    {
        save_char(c);
        get_char();
        token = TOKEN_NE;
        goto final;
    }
    // Other than =, > (state13 -> LT -> final)
    else
    {
        save_char(c);
        get_char();
        token = TOKEN_LT;
        goto final;
    }
state14:
#ifdef DEBUG
    printf("-> state14 ");
#endif
    // = (state14 -> GE -> final)
    if (c == '=')
    {
        save_char(c);
        get_char();
        token = TOKEN_GE;
        goto final;
    }
    // < (state13 -> NE -> final)
    else if (c == '<')
    {
        save_char(c);
        get_char();
        token = TOKEN_NE;
        goto final;
    }
    // Other than =, < (state13 -> GT -> final)
    else
    {
        save_char(c);
        get_char();
        token = TOKEN_GT;
        goto final;
    }
state15:
#ifdef DEBUG
    printf("-> state15 ");
#endif
    // > (state14 -> GE -> final)
    if (c == '>')
    {
        save_char(c);
        get_char();
        token = TOKEN_GE;
        goto final;
    }
    // < (state13 -> LE -> final)
    else if (c == '<')
    {
        save_char(c);
        get_char();
        token = TOKEN_LE;
        goto final;
    }
    // Other than >, < (state13 -> EQ -> final)
    else
    {
        save_char(c);
        get_char();
        token = TOKEN_EQ;
        goto final;
    }
final:
#ifdef DEBUG
    printf("-> final\n");
#endif
    return;
}

void print_token()
{
    if (token == TOKEN_EOF)
    {
        fprintf(lexout, "EOF\t[%s]\n", lexeme);
    }
    else if (token == TOKEN_ERROR)
    {
        fprintf(lexout, "ERROR\t[%s]\n", lexeme);
    }
    else if (token == TOKEN_ID)
    {
        fprintf(lexout, "ID\t[%s]\n", lexeme);
    }
    else if (token == TOKEN_NUM)
    {
        fprintf(lexout, "NUM\t[%s]\n", lexeme);
    }
    else if (token == TOKEN_COL)
    {
        fprintf(lexout, "COL\t[%s]\n", lexeme);
    }
    else if (token == TOKEN_SEMICOL)
    {
        fprintf(lexout, "SEMICOL\t[%s]\n", lexeme);
    }
    else if (token == TOKEN_COLEQ)
    {
        fprintf(lexout, "COLEQ\t[%s]\n", lexeme);
    }
    else if (token == TOKEN_EQ)
    {
        fprintf(lexout, "EQ\t[%s]\n", lexeme);
    }
    else if (token == TOKEN_NE)
    {
        fprintf(lexout, "NE\t[%s]\n", lexeme);
    }
    else if (token == TOKEN_LT)
    {
        fprintf(lexout, "LT\t[%s]\n", lexeme);
    }
    else if (token == TOKEN_LE)
    {
        fprintf(lexout, "LE\t[%s]\n", lexeme);
    }
    else if (token == TOKEN_GT)
    {
        fprintf(lexout, "GT\t[%s]\n", lexeme);
    }
    else if (token == TOKEN_GE)
    {
        fprintf(lexout, "GE\t[%s]\n", lexeme);
    }
    else if (token == TOKEN_LPAR)
    {
        fprintf(lexout, "LPAR\t[%s]\n", lexeme);
    }
    else if (token == TOKEN_RPAR)
    {
        fprintf(lexout, "RPAR\t[%s]\n", lexeme);
    }
    else if (token == TOKEN_PLUS)
    {
        fprintf(lexout, "PLUS\t[%s]\n", lexeme);
    }
    else if (token == TOKEN_MINUS)
    {
        fprintf(lexout, "MINUS\t[%s]\n", lexeme);
    }
    else if (token == TOKEN_ASTER)
    {
        fprintf(lexout, "ASTER\t[%s]\n", lexeme);
    }
    else if (token == TOKEN_SLASH)
    {
        fprintf(lexout, "SLASH\t[%s]\n", lexeme);
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