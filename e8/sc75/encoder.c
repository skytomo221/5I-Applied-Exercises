#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "sc.h"

static FILE *codein;
static FILE *codeout;

void encode(char *op, char *opr)
{
    fprintf(codeout, "; %-16s\t%s\n", op, opr);
}

void convert2(FILE *in, FILE *out)
{
    char op[BUFSIZ];
    char opr[BUFSIZ];

    codein = in;
    codeout = out;
    while (fscanf(codein, "%s %s", op, opr) != EOF)
    {
        encode(op, opr);
    }
}
