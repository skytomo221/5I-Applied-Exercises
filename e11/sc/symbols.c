#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "sc.h"

symbol symbols[MAX_SYMBOLS];
static int end_of_symbols;
static int level;
static int frame_base;

int find_symbols(char *label)
{
    int i;

    for (i = end_of_symbols - 1; i >= 0; i--)
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
    int i;
    if (i = find_symbols(label) >= 0 && symbols[i].level == level)
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
        symbols[end_of_symbols].level = level;
        symbols[end_of_symbols].size = 4;
        symbols[end_of_symbols].offset = OFFSET_AUTO;
    }
    else if (type == TYPE_WORD)
    {
        symbols[end_of_symbols].label = strdup(label);
        symbols[end_of_symbols].type = type;
        symbols[end_of_symbols].level = level;
        symbols[end_of_symbols].size = 2;
        symbols[end_of_symbols].offset = OFFSET_AUTO;
    }
    else if (type == TYPE_BYTE)
    {
        symbols[end_of_symbols].label = strdup(label);
        symbols[end_of_symbols].type = type;
        symbols[end_of_symbols].level = level;
        symbols[end_of_symbols].size = 1;
        symbols[end_of_symbols].offset = OFFSET_AUTO;
    }
    else if (level != 0)
    {
        symbols[end_of_symbols].label = strdup(label);
        symbols[end_of_symbols].type = type;
        symbols[end_of_symbols].level = level;
        symbols[end_of_symbols].size = 4;
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
    level = 0;
}

void encode_symbols(FILE *out)
{
    int i;
    int offset;
    offset = 0;
    fprintf(out, "\t\t.align\t\t2\n");
    for (i = 0; i < end_of_symbols; i++)
    {
        if (symbols[i].level != 0)
        {
            continue;
        }
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

void frame_start()
{
    level = level + 1;
    frame_base = end_of_symbols;
}

void frame_end()
{
    int i, fp, offset;
    fp = find_symbols("fp");
    offset = symbols[fp].offset;
    for (i = 0; i < end_of_symbols; i++)
    {
        if (symbols[i].level != 0)
        {
            symbols[i].offset = (fp - i) * 4;
        }
    }
}

int frame_size()
{
    int offset;
    int i;
    if ((i = find_symbols("er0")) < 0)
    {
        error(ERROR_INTERNAL, "frame_offset", 0);
        return -1;
    }
    offset = symbols[end_of_symbols - 1].offset - symbols[i].offset;
    return -offset;
}

void frame_cancel()
{
    end_of_symbols = frame_base;
    level = level - 1;
}
