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
static int c_lineno = 1;
int lineno;
char lexvalue[BUFSIZ];

void get_char()
{
    c = fgetc(lexin);
    if (c == '\n')
    {
        c_lineno += 1;
    }
    return;
}

/*------------------------------------------------------------*/

void save_char(char c)
{
    lexeme[end_of_lexeme++] = c;
    lexeme[end_of_lexeme] = '\0';
}

/*------------------------------------------------------------*/

int token;

void get_token()
{
    int i, value = 0;

state0:
#ifdef DEBUG3
    printf("[%c] state0 ", c);
#endif
    end_of_lexeme = 0;
    if (char_pos(WHITESPACE, c) >= 0)
    {
        get_char();
        lineno = c_lineno;
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
    else if ((i = char_pos(DIGIT, c)) >= 0)
    {
        save_char(c);
        value = i;
        get_char();
#ifdef DEBUG4
        printf("i = %d, value = %d\n", i, value);
#endif
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
    else if (c == '%')
    {
        save_char(c);
        get_char();
        token = TOKEN_PERCENT;
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
#ifdef DEBUG3
    printf("-> [%c] state1 ", c);
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
        token = TOKEN_LPAR;
        goto final;
    }
state2:
#ifdef DEBUG3
    printf("-> [%c] state2 ", c);
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
#ifdef DEBUG3
    printf("-> [%c] state3 ", c);
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
        goto state3;
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
#ifdef DEBUG3
    printf("-> [%c] state4 ", c);
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
#ifdef DEBUG3
    printf("-> [%c] state5 ", c);
#endif
    // x (state5 -> state6)
    if (c == 'x')
    {
        save_char(c);
        get_char();
        goto state6;
    }
    else if (c == 'b')
    {
        save_char(c);
        value = i;
        get_char();
        goto state16;
    }
    // DIGHT (state5 -> state8)
    else if ((i = char_pos(DIGIT, c)) >= 0)
    {
        save_char(c);
        value = i;
        get_char();
        goto state8;
    }
    // Other than x, DIGHT (state5 -> NUM -> final)
    else
    {
        token = TOKEN_NUM;
        sprintf(lexvalue, "%d", value);
        goto final;
    }
state6:
#ifdef DEBUG3
    printf("-> [%c] state6 ", c);
#endif
    // HEXDIGIT (state6 -> state7)
    if ((i = char_pos(HEXDIGIT, tolower(c))) >= 0)
    {
        save_char(c);
        value = i;
        get_char();
#ifdef DEBUG4
        printf("i = %d (%x), value = %d (%x)\n", i, i, value, value);
#endif
        goto state7;
    }
    // Other than HEXDIGIT (state6 -> ERROR -> final)
    else
    {
        get_char();
        token = TOKEN_ERROR;
        goto final;
    }
state7:
#ifdef DEBUG3
    printf("-> [%c] state7 ", c);
#endif
    // HEXDIGIT (state7 -> state7)
    if ((i = char_pos(HEXDIGIT, tolower(c))) >= 0)
    {
        save_char(c);
        value = value * 0x10 + i;
        get_char();
#ifdef DEBUG4
        printf("i = %d (%x), value = %d (%x)\n", i, i, value, value);
#endif
        goto state7;
    }
    // Other than HEXDIGIT (state7 -> NUM -> final)
    else
    {
        token = TOKEN_NUM;
        sprintf(lexvalue, "%d", value);
        goto final;
    }
state8:
#ifdef DEBUG3
    printf("-> [%c] state8 ", c);
#endif
    // DIGHT (state8 -> state8)
    if ((i = char_pos(DIGIT, c)) >= 0)
    {
        save_char(c);
        value = value * 10 + i;
        get_char();
#ifdef DEBUG4
        printf("i = %d, value = %d\n", i, value);
#endif
        goto state8;
    }
    // Other than DIGHT (state8 -> NUM -> final)
    else
    {
        token = TOKEN_NUM;
        sprintf(lexvalue, "%d", value);
        goto final;
    }
state9:
#ifdef DEBUG3
    printf("-> [%c] state9 ", c);
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
        value = c;
        get_char();
#ifdef DEBUG4
        printf("i = %d (%c), value = %d (%c)\n", i, i, value, value);
#endif
        goto state11;
    }
state10:
#ifdef DEBUG3
    printf("-> [%c] state10 ", c);
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
        switch (c)
        {
        case 'n':
            value = '\n';
            break;
        case 't':
            value = '\t';
            break;
        case '0':
            value = '\0';
            break;
        default:
            value = c;
            break;
        }
        get_char();
#ifdef DEBUG4
        printf("i = %d (%c), value = %d (%c)\n", i, i, value, value);
#endif
        goto state11;
    }
    // Other than n, t, 0, ', ", \ (state10 -> ERROR -> final)
    else
    {
        save_char(c);
        token = TOKEN_ERROR;
        goto final;
    }
state11:
#ifdef DEBUG3
    printf("-> [%c] state11 ", c);
#endif
    // ' (state11 -> NUM -> final)
    if (c == '\'')
    {
        save_char(c);
        get_char();
        token = TOKEN_NUM;
        sprintf(lexvalue, "%d", value);
        goto final;
    }
    // Othar than ' (state11 -> ERROR -> final)
    else
    {
        token = TOKEN_ERROR;
        goto final;
    }
state12:
#ifdef DEBUG3
    printf("-> [%c] state12 ", c);
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
        token = TOKEN_COL;
        goto final;
    }
state13:
#ifdef DEBUG3
    printf("-> [%c] state13 ", c);
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
    else if (c == '<')
    {
        save_char(c);
        get_char();
        token = TOKEN_SHIFTL;
        goto final;
    }
    // Other than =, > (state13 -> LT -> final)
    else
    {
        token = TOKEN_LT;
        goto final;
    }
state14:
#ifdef DEBUG3
    printf("-> [%c] state14 ", c);
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
    else if (c == '>')
    {
        save_char(c);
        get_char();
        token = TOKEN_SHIFTR;
        goto final;
    }
    // Other than =, < (state13 -> GT -> final)
    else
    {
        token = TOKEN_GT;
        goto final;
    }
state15:
#ifdef DEBUG3
    printf("-> [%c] state15 ", c);
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
        token = TOKEN_EQ;
        goto final;
    }
state16:
#ifdef DEBUG3
    printf("-> [%c] state16 ", c);
#endif
    // BINDIGIT (state16 -> state17)
    if ((i = char_pos(BINDIGIT, tolower(c))) >= 0)
    {
        save_char(c);
        value = i;
        get_char();
#ifdef DEBUG4
        printf("i = %d (%x), value = %d (%x)\n", i, i, value, value);
#endif
        goto state17;
    }
    // Other than HEXDIGIT (state16 -> ERROR -> final)
    else
    {
        get_char();
        token = TOKEN_ERROR;
        goto final;
    }
state17:
#ifdef DEBUG3
    printf("-> [%c] state17 ", c);
#endif
    // HEXDIGIT (state7 -> state7)
    if ((i = char_pos(BINDIGIT, tolower(c))) >= 0)
    {
        save_char(c);
        value = value * 2 + i;
        get_char();
#ifdef DEBUG4
        printf("i = %d (%x), value = %d (%x)\n", i, i, value, value);
#endif
        goto state17;
    }
    // Other than HEXDIGIT (state7 -> NUM -> final)
    else
    {
        token = TOKEN_NUM;
        sprintf(lexvalue, "%d", value);
        goto final;
    }
final:
#ifdef DEBUG3
    printf("-> final\n", c);
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
        fprintf(lexout, "NUM\t[%s]\t[%s]\n", lexeme, lexvalue);
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
    parse();
    if (token != TOKEN_EOF)
    {
        error(ERROR_SYNTAX, lexeme, lineno);
    }
}

void gen_code(char *op, char *opr)
{
    fprintf(lexout, "%-16s\t%s\n", op, opr);
}