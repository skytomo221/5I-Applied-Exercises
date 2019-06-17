#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "sc.h"

symbol symbols[MAX_SYMBOLS];
static int end_of_symbols;

int find_symbols(char *label)
{
    int i;

    for (i = 0; i < end_of_symbols; i++)
    {
        if (strcmp(symbols[i].label, label) == 0)
        {
            return i;
        }
    }

    return -1;
}

int enter_symbols(char *label, int type)
{
    if (find_symbols(label) >= 0)
    {
        error(ERROR_DOUBLE, label, 0);
        return -1;
    }

    if (end_of_symbols >= MAX_SYMBOLS)
    {
        error(ERROR_OVERFLOW, "symbols", 0);
        return -1;
    }

    if (type == TYPE_LONG)
    {
        symbols[end_of_symbols].label = strdup(label);
        symbols[end_of_symbols].type = type;
        symbols[end_of_symbols].size = 4;
        symbols[end_of_symbols].offset = OFFSET_AUTO;
    }
    else if (type == TYPE_WORD)
    {
        symbols[end_of_symbols].label = strdup(label);
        symbols[end_of_symbols].type = type;
        symbols[end_of_symbols].size = 2;
        symbols[end_of_symbols].offset = OFFSET_AUTO;
    }
    else if (type == TYPE_BYTE)
    {
        symbols[end_of_symbols].label = strdup(label);
        symbols[end_of_symbols].type = type;
        symbols[end_of_symbols].size = 1;
        symbols[end_of_symbols].offset = OFFSET_AUTO;
    }
    else
    {
        error(ERROR_INTERNAL, "enter_symbols", 0);
    }

    return end_of_symbols++;
}

void setup_symbols()
{
    end_of_symbols = 0;
}

void encode_symbols(FILE *out)
{
    int i;
    int offset;

    offset = 0;
    fprintf(out, "\t\t.align\t\t2\n");
    for (i = 0; i < end_of_symbols; i++)
    {

        if (symbols[i].offset != OFFSET_AUTO)
        {
            continue;
        }

        if (symbols[i].type == TYPE_LONG)
        {
            if (offset % 2 != 0)
            {
                fprintf(out, "\t\t.align\t\t2\n");
                offset += 1;
            }
            fprintf(out, "%s", symbols[i].label);
            fprintf(out, "\t\t.res.l\t\t%d\n", symbols[i].size / 4);
            offset += symbols[i].size;
        }
        else if (symbols[i].type == TYPE_WORD)
        {
            if (offset % 2 != 0)
            {
                fprintf(out, "\t\t.align\t\t2\n");
                offset += 1;
            }
            fprintf(out, "%s", symbols[i].label);
            fprintf(out, "\t\t.res.w\t\t%d\n", symbols[i].size / 2);
            offset += symbols[i].size;
        }
        else if (symbols[i].type == TYPE_BYTE)
        {
            fprintf(out, "%s", symbols[i].label);
            fprintf(out, "\t\t.res.b\t\t%d\n", symbols[i].size);
            offset += symbols[i].size;
        }
        else
        {
            error(ERROR_INTERNAL, "encode_symbols", 0);
        }
    }
}
