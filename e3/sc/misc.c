#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "sc.h"

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
