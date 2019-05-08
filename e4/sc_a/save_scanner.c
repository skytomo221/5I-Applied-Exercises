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

state0:
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
state2:
    // * (state2 -> state3)
    // EOF (state2 -> state0)
    // Other than EOF, * (state2 -> state2)
state3:
    // ) (state3 -> state0)
    // * (state3 -> state3)
    // EOF (state3 -> state0)
    // Other than ), *, EOF (state3 -> state2)
state4:
    // ALPHABET (state4 -> state4)
    // DIGIT (state4 -> state4)
    // Other than ALPHABET and DIGIT (state4 -> ID, IF, THEN, ... -> final)
state5:
    // x (state5 -> state6)
    // DIGHT (state5 -> state8)
    // Other than x, DIGHT (state5 -> NUM -> final)
state6:
    // HEXDIGHT (state6 -> state7)
    // Other than HEXDIGHT (state6 -> ERROR -> final)
state7:
    // HEXDIGHT (state7 -> state7)
    // Other than HEXDIGHT (state7 -> NUM -> final)
state8:
    // DIGHT (state8 -> state8)
    // Other than DIGHT (state8 -> NUM -> final)
state9:
    // \ (state9 -> state10)
    // ', " (state9 -> ERROR -> final)
    // Othar than \, ', " (state10 -> state11)
state10:
    // n, t, 0, ', ", \ (state10 -> NUM -> state11)
    // Other than n, t, 0, ', ", \ (state10 -> ERROR -> final)
state11:
    // ' (state11 -> NUM -> final)
    // Othar than ' (state11 -> ERROR -> final)
state12:
    // = (state12 -> COLEQ -> final)
    // Other than = (state12 -> COL -> final)
state13:
    // = (state13 -> LE -> final)
    // > (state13 -> NE -> final)
    // Other than =, > (state13 -> LT -> final)
state14:
    // = (state14 -> GE -> final)
    // < (state13 -> NE -> final)
    // Other than =, < (state13 -> GT -> final)
state15:
    // > (state14 -> GE -> final)
    // < (state13 -> LE -> final)
    // Other than >, < (state13 -> EQ -> final)
final:
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
