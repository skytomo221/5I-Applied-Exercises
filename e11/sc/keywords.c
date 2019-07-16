#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "sc.h"

static int end_of_keywords;
keyword keywords[MAX_KEYWORDS];

char *name[MAX_KEYWORDS]; /* keyword = (name, value) */
int value[MAX_KEYWORDS];

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
        error(ERROR_OVERFLOW, "keywords", 0);
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
    enter_keywords("function", TOKEN_FUNCTION);
    enter_keywords("return", TOKEN_RETURN);
}
