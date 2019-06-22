#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "sc.h"

static FILE *codein;
static FILE *codeout;
static int current_id;

char *ER2R(char *er)
{
    char *r;

    if (strcmp(er, "ER0") == 0)
    {
        r = "R0";
    }
    else if (strcmp(er, "ER1") == 0)
    {
        r = "R1";
    }
    else if (strcmp(er, "ER2") == 0)
    {
        r = "R2";
    }
    else if (strcmp(er, "ER3") == 0)
    {
        r = "R3";
    }
    else if (strcmp(er, "ER4") == 0)
    {
        r = "R4";
    }
    else if (strcmp(er, "ER5") == 0)
    {
        r = "R5";
    }
    else if (strcmp(er, "ER6") == 0)
    {
        r = "R6";
    }
    else if (strcmp(er, "ER7") == 0)
    {
        r = "R7";
    }
    else
    {
        error(ERROR_INTERNAL, "ER2R", 0);
        r = NULL;
    }
    return r;
}

char *ER2RL(char *er)
{
    char *r;

    if (strcmp(er, "ER0") == 0)
    {
        r = "R0L";
    }
    else if (strcmp(er, "ER1") == 0)
    {
        r = "R1L";
    }
    else if (strcmp(er, "ER2") == 0)
    {
        r = "R2L";
    }
    else if (strcmp(er, "ER3") == 0)
    {
        r = "R3L";
    }
    else if (strcmp(er, "ER4") == 0)
    {
        r = "R4L";
    }
    else if (strcmp(er, "ER5") == 0)
    {
        r = "R5L";
    }
    else if (strcmp(er, "ER6") == 0)
    {
        r = "R6L";
    }
    else if (strcmp(er, "ER7") == 0)
    {
        r = "R7L";
    }
    else
    {
        error(ERROR_INTERNAL, "ER2R", 0);
        r = NULL;
    }
    return r;
}

static int idle_sp = -1;
static char *idle[BUFSIZ];

void push_idle(char *r)
{
    idle[++idle_sp] = r;
}

char *pop_idle()
{
    char *r;

    if (idle_sp >= 0)
    {
        r = idle[idle_sp--];
    }
    else
    {
        r = NULL;
        error(ERROR_INTERNAL, "pop_idle", 0);
    }

    return r;
}

static int inuse_sp = -1;
static char *inuse[BUFSIZ];

void push_inuse(char *r)
{
    inuse[++inuse_sp] = r;
}

char *pop_inuse()
{
    char *r;

    if (inuse_sp >= 0)
    {
        r = inuse[inuse_sp--];
    }
    else
    {
        r = NULL;
        error(ERROR_INTERNAL, "pop_inuse", 0);
    }

    return r;
}

void encode(char *op, char *opr)
{
    char label1[BUFSIZ];
    char label2[BUFSIZ];
    char *r0;
    char *r1;
    int i;
    // fprintf(codeout, "; %-16s\t%s\n", op, opr);
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
        if ((i = enter_constants(opr)) < 0)
        {
            return;
        }
        r0 = pop_idle();
        fprintf(codeout, "\t\tmov.l\t\t@%s,%s\n", constants[i].label, r0);
        push_inuse(r0);
    }
    else if (strcmp(op, "load_id") == 0)
    {
        if ((i = find_symbols(opr)) < 0)
        {
            error(ERROR_UNDEFINED, opr, 0);
            return;
        }
        if (symbols[i].type == TYPE_LONG)
        {
            r0 = pop_idle();
            fprintf(codeout, "\t\tmov.l\t\t@%s,%s\n", opr, r0);
            push_inuse(r0);
        }
        else if (symbols[i].type == TYPE_WORD)
        {
            r0 = pop_idle();
            fprintf(codeout, "\t\tmov.w\t\t@%s,%s\n", opr, ER2R(r0));
            fprintf(codeout, "\t\texts.l\t\t%s\n", r0);
            push_inuse(r0);
        }
        else if (symbols[i].type == TYPE_BYTE)
        {
            r0 = pop_idle();
            fprintf(codeout, "\t\tmov.b\t\t@%s,%s\n", opr, ER2RL(r0));
            fprintf(codeout, "\t\texts.w\t\t%s\n", ER2R(r0));
            fprintf(codeout, "\t\texts.l\t\t%s\n", r0);
            push_inuse(r0);
        }
        else
        {
            error(ERROR_TYPE, opr, 0);
        }
    }
    else if (strcmp(op, "store_id") == 0)
    {
        if ((i = find_symbols(opr)) < 0)
        {
            error(ERROR_UNDEFINED, opr, 0);
            return;
        }
        if (symbols[i].type == TYPE_LONG)
        {
            r0 = pop_inuse();
            fprintf(codeout, "\t\tmov.l\t\t%s,@%s\n", r0, opr);
            push_idle(r0);
        }
        else if (symbols[i].type == TYPE_WORD)
        {
            r0 = pop_inuse();
            fprintf(codeout, "\t\tmov.w\t\t%s,@%s\n", ER2R(r0), opr);
            push_idle(r0);
        }
        else if (symbols[i].type == TYPE_BYTE)
        {
            r0 = pop_inuse();
            fprintf(codeout, "\t\tmov.b\t\t%s,@%s\n", ER2RL(r0), opr);
            push_idle(r0);
        }
        else
        {
            error(ERROR_TYPE, opr, 0);
        }
    }
    else if (strcmp(op, "neg") == 0)
    {
        r0 = pop_inuse();
        fprintf(codeout, "\t\tneg.l\t\t%s\n", r0);
        push_inuse(r0);
    }
    else if (strcmp(op, "not") == 0)
    {
        r0 = pop_inuse();
        fprintf(codeout, "\t\tnot.l\t\t%s\n", r0);
        push_inuse(r0);
    }
    else if (strcmp(op, "mul") == 0)
    {
        r0 = pop_inuse();
        r1 = pop_inuse();
        fprintf(codeout, "\t\tmulxs.w\t\t%s,%s\n", ER2R(r0), r1);
        push_inuse(r1);
        push_idle(r0);
    }
    else if (strcmp(op, "div") == 0)
    {
        r0 = pop_inuse();
        r1 = pop_inuse();
        fprintf(codeout, "\t\tdivxs.w\t\t%s,%s\n", ER2R(r0), r1);
        fprintf(codeout, "\t\texts.l\t\t%s\n", r1);
        push_inuse(r1);
        push_idle(r0);
    }
    else if (strcmp(op, "and") == 0)
    {
        r0 = pop_inuse();
        r1 = pop_inuse();
        fprintf(codeout, "\t\tand.l\t\t%s,%s\n", r0, r1);
        push_inuse(r1);
        push_idle(r0);
    }
    else if (strcmp(op, "add") == 0)
    {
        r0 = pop_inuse();
        r1 = pop_inuse();
        fprintf(codeout, "\t\tadd.l\t\t%s,%s\n", r0, r1);
        push_inuse(r1);
        push_idle(r0);
    }
    else if (strcmp(op, "sub") == 0)
    {
        r0 = pop_inuse();
        r1 = pop_inuse();
        fprintf(codeout, "\t\tsub.l\t\t%s,%s\n", r0, r1);
        push_inuse(r1);
        push_idle(r0);
    }
    else if (strcmp(op, "or") == 0)
    {
        r0 = pop_inuse();
        r1 = pop_inuse();
        fprintf(codeout, "\t\tor.l\t\t%s,%s\n", r0, r1);
        push_inuse(r1);
        push_idle(r0);
    }
    else if (strcmp(op, "tst") == 0)
    {
        r0 = pop_inuse();
        fprintf(codeout, "\t\tand.l\t\t%s,%s\n", r0, r0);
        push_idle(r0);
    }
    else if (strcmp(op, "jmp") == 0)
    {
        fprintf(codeout, "\t\tbra\t\t%s\n", opr);
    }
    else if (strcmp(op, "jeq") == 0)
    {
        fprintf(codeout, "\t\tbeq\t\t%s\n", opr);
    }
    else if (strcmp(op, "eq") == 0)
    {
        r0 = pop_inuse();
        r1 = pop_inuse();
        strcpy(label1, new_label());
        strcpy(label2, new_label());
        fprintf(codeout, "\t\tcmp.l\t\t%s,%s\n", r0, r1);
        fprintf(codeout, "\t\tbeq\t\t%s\n", label1);
        fprintf(codeout, "\t\tmov.l\t\t#H'00000000,%s\n", r1);
        fprintf(codeout, "\t\tbra\t\t%s\n", label2);
        fprintf(codeout, "%s:", label1);
        fprintf(codeout, "\t\tmov.l\t\t#H'FFFFFFFF,%s\n", r1);
        fprintf(codeout, "%s\t\t.equ\t\t$\n", label2);
        push_inuse(r1);
        push_idle(r0);
    }
    else if (strcmp(op, "ne") == 0)
    {
        r0 = pop_inuse();
        r1 = pop_inuse();
        strcpy(label1, new_label());
        strcpy(label2, new_label());
        fprintf(codeout, "\t\tcmp.l\t\t%s,%s\n", r0, r1);
        fprintf(codeout, "\t\tbne\t\t%s\n", label1);
        fprintf(codeout, "\t\tmov.l\t\t#H'00000000,%s\n", r1);
        fprintf(codeout, "\t\tbra\t\t%s\n", label2);
        fprintf(codeout, "%s:", label1);
        fprintf(codeout, "\t\tmov.l\t\t#H'FFFFFFFF,%s\n", r1);
        fprintf(codeout, "%s\t\t.equ\t\t$\n", label2);
        push_inuse(r1);
        push_idle(r0);
    }
    else if (strcmp(op, "lt") == 0)
    {
        r0 = pop_inuse();
        r1 = pop_inuse();
        strcpy(label1, new_label());
        strcpy(label2, new_label());
        fprintf(codeout, "\t\tcmp.l\t\t%s,%s\n", r0, r1);
        fprintf(codeout, "\t\tblt\t\t%s\n", label1);
        fprintf(codeout, "\t\tmov.l\t\t#H'00000000,%s\n", r1);
        fprintf(codeout, "\t\tbra\t\t%s\n", label2);
        fprintf(codeout, "%s:", label1);
        fprintf(codeout, "\t\tmov.l\t\t#H'FFFFFFFF,%s\n", r1);
        fprintf(codeout, "%s\t\t.equ\t\t$\n", label2);
        push_inuse(r1);
        push_idle(r0);
    }
    else if (strcmp(op, "le") == 0)
    {
        r0 = pop_inuse();
        r1 = pop_inuse();
        strcpy(label1, new_label());
        strcpy(label2, new_label());
        fprintf(codeout, "\t\tcmp.l\t\t%s,%s\n", r0, r1);
        fprintf(codeout, "\t\tble\t\t%s\n", label1);
        fprintf(codeout, "\t\tmov.l\t\t#H'00000000,%s\n", r1);
        fprintf(codeout, "\t\tbra\t\t%s\n", label2);
        fprintf(codeout, "%s:", label1);
        fprintf(codeout, "\t\tmov.l\t\t#H'FFFFFFFF,%s\n", r1);
        fprintf(codeout, "%s\t\t.equ\t\t$\n", label2);
        push_inuse(r1);
        push_idle(r0);
    }
    else if (strcmp(op, "gt") == 0)
    {
        r0 = pop_inuse();
        r1 = pop_inuse();
        strcpy(label1, new_label());
        strcpy(label2, new_label());
        fprintf(codeout, "\t\tcmp.l\t\t%s,%s\n", r0, r1);
        fprintf(codeout, "\t\tbgt\t\t%s\n", label1);
        fprintf(codeout, "\t\tmov.l\t\t#H'00000000,%s\n", r1);
        fprintf(codeout, "\t\tbra\t\t%s\n", label2);
        fprintf(codeout, "%s:", label1);
        fprintf(codeout, "\t\tmov.l\t\t#H'FFFFFFFF,%s\n", r1);
        fprintf(codeout, "%s\t\t.equ\t\t$\n", label2);
        push_inuse(r1);
        push_idle(r0);
    }
    else if (strcmp(op, "ge") == 0)
    {
        r0 = pop_inuse();
        r1 = pop_inuse();
        strcpy(label1, new_label());
        strcpy(label2, new_label());
        fprintf(codeout, "\t\tcmp.l\t\t%s,%s\n", r0, r1);
        fprintf(codeout, "\t\tbge\t\t%s\n", label1);
        fprintf(codeout, "\t\tmov.l\t\t#H'00000000,%s\n", r1);
        fprintf(codeout, "\t\tbra\t\t%s\n", label2);
        fprintf(codeout, "%s:", label1);
        fprintf(codeout, "\t\tmov.l\t\t#H'FFFFFFFF,%s\n", r1);
        fprintf(codeout, "%s\t\t.equ\t\t$\n", label2);
        push_inuse(r1);
        push_idle(r0);
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
        fprintf(stderr, "intermediate code error: %s %s\n", op, opr);
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
    push_idle("ER5");
    push_idle("ER4");
    push_idle("ER3");
    push_idle("ER2");
    push_idle("ER1");
    push_idle("ER0");
    current_id = UNDEFINED;
    while (fscanf(codein, "%s %s", op, opr) != EOF)
    {
        encode(op, opr);
    }
    pop_idle();
    pop_idle();
    pop_idle();
    pop_idle();
    pop_idle();
    pop_idle();
}
