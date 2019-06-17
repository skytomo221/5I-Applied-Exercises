#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "sc.h"

int m_option;

int main(int argc, char *argv[])
{
    FILE *in;
    FILE *out;
    FILE *tmp;

    m_option = 0;

    if (argc == 4)
    {
        if (strcmp(argv[1], "-m") == 0)
        {
            m_option = 1;
        }
        else
        {
            perror("invild option");
        }

        if ((in = fopen(argv[2], "r")) == NULL)
        {
            perror("fopen for input");
            exit(1);
        }
        if ((out = fopen(argv[3], "w")) == NULL)
        {
            perror("fopen for output");
            exit(1);
        }
    }
    else if (argc == 3)
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

    if ((tmp = tmpfile()) == NULL)
    {
        perror("fopen for tmpfile");
        exit(1);
    }

    convert(in, tmp);
    fflush(tmp);
    rewind(tmp);
    convert2(tmp, out);

    fclose(in);
    fclose(out);
    fclose(tmp);
    return 0;
}
