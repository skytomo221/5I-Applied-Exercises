#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

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
        token = TOKEN_ID;
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
