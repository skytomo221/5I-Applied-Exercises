#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "sc.h"

constant constants[MAX_CONSTANTS];
static int end_of_constants;

int find_constants(char *value)
{
    int i;

    for (i = 0; i < end_of_constants; i++)
    {
        if (strcmp(constants[i].value, value) == 0)
        {
            return i;
        }
    }

    return -1;
}

int enter_constants(char *value)
{
    int i;

    if (find_constants(value) >= 0)
    {
        return find_constants(value);
    }

    if (end_of_constants >= MAX_CONSTANTS)
    {
        error(ERROR_OVERFLOW, value, 0);
        return -1;
    }

    constants[end_of_constants].value = strdup(value);
    constants[end_of_constants].label = strdup(new_label());
    return end_of_constants++;
}

void setup_constants()
{
    end_of_constants = 0;
}

void encode_constants(FILE *out)
{
    int i;

    fprintf(out, "\t\t.align\t\t2\n");
    for (i = 0; i < end_of_constants; i++)
    {
        fprintf(out, "%s\t\t.data.l\t\tH'%08X\n",
                constants[i].label, atoi(constants[i].value));
    }
}
