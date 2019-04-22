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
