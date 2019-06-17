#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "sc.h"

static FILE *codein;
static FILE *codeout;
static int current_id;

void encode(char *op, char *opr)
{
    if (m_option)
    {
        fprintf(codeout, "; %-16s\t%s\n", op, opr);
    }
    if (strcmp(op, "start") == 0)
    {
        fprintf(codeout, "\t\t.cpu\t\t300HA\n");
        fprintf(codeout, "\t\t.section\tA,code,locate=H'000000\n");
        fprintf(codeout, "\t\t.data.l\t\treset\n");
        fprintf(codeout, "\t\t.section\tB,code,locate=H'000100\n");
        fprintf(codeout, "reset:\t\tmov.l\t\t#H'FFFF10,ER7\n");
        fprintf(codeout, "\t\tbra\t\t%s\n", opr);
    }
    else if (strcmp(op, "end") == 0)
    {
        fprintf(codeout, "\t\tbra\t\t$\n");
        encode_constants(codeout);
        fprintf(codeout, "\t\t.section\tC,data,locate=H'FFBF20\n");
        encode_symbols(codeout);
        fprintf(codeout, "\t\t.end\n");
    }
    else if (strcmp(op, "label") == 0)
    {
        fprintf(codeout, "%s\t\t.equ\t\t$\n", opr);
    }
    else if (strcmp(op, "load_num") == 0)
    {
        enter_constants(opr);
    }
    else if (strcmp(op, "var_long") == 0)
    {
        current_id = enter_symbols(opr, TYPE_LONG);
    }
    else if (strcmp(op, "var_word") == 0)
    {
        current_id = enter_symbols(opr, TYPE_WORD);
    }
    else if (strcmp(op, "var_byte") == 0)
    {
        current_id = enter_symbols(opr, TYPE_BYTE);
    }
    else if (strcmp(op, "set_address") == 0)
    {
        if (current_id == UNDEFINED)
        {
            error(ERROR_INTERNAL, "encode: set_address", 0);
            return;
        }
        symbols[current_id].offset = atoi(opr);
        fprintf(codeout, "%s\t\t.equ\t\tH'%08X\n",
                symbols[current_id].label, atoi(opr));
    }
    else
    {
        ;
    }
}

void convert2(FILE *in, FILE *out)
{
    char op[BUFSIZ];
    char opr[BUFSIZ];

    codein = in;
    codeout = out;
    setup_constants();
    setup_symbols();
    current_id = UNDEFINED;
    while (fscanf(codein, "%s %s", op, opr) != EOF)
    {
        encode(op, opr);
    }
}
