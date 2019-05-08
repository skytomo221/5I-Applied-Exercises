#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "sc.h"

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
