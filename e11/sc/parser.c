#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "sc.h"

void parse_expression();

#ifdef DEBUG11
int debug11cnt = 0;
#endif

// factor ::= [1] ID [2] ( LPAR [3] ( expression [4] { COMMA expression [5]}
//                                  | ϵ
//                                  ) RPAR [6]
//                       | ϵ [7]
//                       )
//          | NUM [8]
//          | LPAR expression RPAR
//          | NOT factor [9]
void parse_factor()
{
    char id[BUFSIZ]; /* [1] */
    int args;        /* [1] */
    int i;           /* [1] */
    if (token == TOKEN_ID)
    {
        strcpy(id, lexeme); /* [2] */
        get_token();
        if (token == TOKEN_LPAR)
        {
            get_token();
            args = 0;
            if (token != TOKEN_RPAR)
            {
                parse_expression();
                gen_code("store_arg", "-"); /* [4] */
                args += 1;                  /* [4] */
                while (token == TOKEN_COMMA)
                {
                    get_token();
                    parse_expression();
                    gen_code("store_arg", "-"); /* [5] */
                    args += 1;                  /* [5] */
                }
            }
            if (token == TOKEN_RPAR)
            {
                get_token();
                gen_code("load_num", "0");       /* [6] */
                gen_code("store_arg", "-");      /* [6] */
                gen_code("call", id);            /* [6] */
                gen_code("load_arg", "-");       /* [6] */
                for (i = 0; i < args; i++)       /* [6] */
                {                                /* [6] */
                    gen_code("cancel_arg", "-"); /* [6] */
                }                                /* [6] */
            }
        }
        else
        {
            gen_code("load_id", id); /* [7] */
        }
    }
    else if (token == TOKEN_NUM)
    {
        gen_code("load_num", lexvalue); /* [8] */
        get_token();
    }
    else if (token == TOKEN_LPAR)
    {
        get_token();
        parse_expression();
        if (token == TOKEN_RPAR)
        {
            get_token();
        }
        else
        {
            error(ERROR_SYNTAX, lexeme, lineno);
        }
    }
    else if (token == TOKEN_NOT)
    {
        get_token();
        parse_factor();
        gen_code("not", "-"); /* [9] */
    }
    else
    {
    }
}

void parse_term()
{
    parse_factor();

    while (1)
    {
        if (token == TOKEN_ASTER)
        {
            get_token();
            parse_factor();
            gen_code("mul", "-");
        }
        else if (token == TOKEN_SLASH)
        {
            get_token();
            parse_factor();
            gen_code("div", "-");
        }
        else if (token == TOKEN_AND)
        {
            get_token();
            parse_factor();
            gen_code("and", "-");
        }
        else
        {
            break;
        }
    }
}

// polynomial ::= ( PLUS term | MINUS term [1] | term )
//                { PLUS term [2] | MINUS term [3] | OR term [4] }
// [1] gen_code("neg", "-");
// [2] gen_code("add", "-");
// [3] gen_code("sub", "-");
// [4] gen_code("or", "-");
void parse_polynomial()
{
    if (token == TOKEN_PLUS)
    {
        get_token();
    }
    else if (token == TOKEN_MINUS)
    {
        get_token();
        gen_code("neg", "-");
    }
    parse_term();
    while (token == TOKEN_PLUS ||
           token == TOKEN_MINUS ||
           token == TOKEN_OR)
    {
        int save_token = token;
        get_token();
        parse_term();
        switch (save_token)
        {
        case TOKEN_PLUS:
            gen_code("add", "-");
            break;
        case TOKEN_MINUS:
            gen_code("sub", "-");
            break;
        case TOKEN_OR:
            gen_code("or", "-");
            break;
        }
    }
}

// expression ::= polynomial ( EQ polynomial [1] | NE polynomial [2]
//                           | LT polynomial [3] | LE polynomial [4]
//                           | GT polynomial [5] | GE polynomial [6]
//                           | ε )
// [1] gen_code("eq", "-");
// [2] gen_code("ne", "-");
// [3] gen_code("lt", "-");
// [4] gen_code("le", "-");
// [5] gen_code("gt", "-");
// [6] gen_code("ge", "-");
void parse_expression()
{
    parse_polynomial();
    if (token == TOKEN_EQ)
    {
        get_token();
        parse_polynomial();
        gen_code("eq", "-");
    }
    else if (token == TOKEN_NE)
    {
        get_token();
        parse_polynomial();
        gen_code("ne", "-");
    }
    else if (token == TOKEN_LT)
    {
        get_token();
        parse_polynomial();
        gen_code("lt", "-");
    }
    else if (token == TOKEN_LE)
    {
        get_token();
        parse_polynomial();
        gen_code("le", "-");
    }
    else if (token == TOKEN_GT)
    {
        get_token();
        parse_polynomial();
        gen_code("gt", "-");
    }
    else if (token == TOKEN_GE)
    {
        get_token();
        parse_polynomial();
        gen_code("ge", "-");
    }
    else
    {
        return;
    }
}

// statement::=[1] ID [2] ( COLEQ expression [3]
//                        | COL [4] statement
//                        | LPAR [5] ( expression [6] { COMMA expression [7] }
//                                   | ϵ
//                                   ) RPAR [8]
//                        )
//             (省略)
//                | RETURN expression [9]
//             (省略)
void parse_statement()
{
#ifdef DEBUG11
    int a;
    for (a = 0; a < debug11cnt; a++)
        printf(" ");
    printf("* <Statement>");
    debug11cnt++;
    printf(" %s\n", lexeme);
#endif
    char id[BUFSIZ]; /* [1] */
    char label1[BUFSIZ];
    char label2[BUFSIZ];
    int args; /* [1] */
    int i;    /* [1] */
    if (token == TOKEN_ID)
    {
#ifdef DEBUG11
        int a;
        for (a = 0; a < debug11cnt; a++)
            printf(" ");
        printf("* <ID>");
        debug11cnt++;
        printf(" %s\n", lexeme);
#endif
        strcpy(id, lexeme); /* [2] */
        get_token();
        if (token == TOKEN_COLEQ)
        {
            get_token();
            parse_expression();
            gen_code("store_id", id); /* [3] */
        }
        else if (token == TOKEN_COL)
        {
            gen_code("label", id); /* [4] */
            get_token();
            parse_statement();
        }
        else if (token == TOKEN_LPAR)
        {
#ifdef DEBUG11
            int a;
            for (a = 0; a < debug11cnt; a++)
                printf(" ");
            printf("* <Lpar>");
            debug11cnt++;
            printf(" %s\n", lexeme);
#endif
            get_token();
            args = 0; /* [5] */
            if (token != TOKEN_RPAR)
            {
                parse_expression();
                gen_code("store_arg", "-"); /* [6] */
                args += 1;                  /* [6] */
                while (token == TOKEN_COMMA)
                {
                    get_token();
                    parse_expression();
                    gen_code("store_arg", "-"); /* [7] */
                    args += 1;                  /* [7] */
                }
            }
            if (token == TOKEN_RPAR)
            {
                gen_code("load_num", "0");       /* [8] */
                gen_code("store_arg", "-");      /* [8] */
                gen_code("call", id);            /* [8] */
                gen_code("cancel_arg", "-");       /* [8] */
                for (i = 0; i < args; i++)       /* [8] */
                {                                /* [8] */
                    gen_code("cancel_arg", "-"); /* [8] */
                }                                /* [8] */
                get_token();
            }
#ifdef DEBUG11
            debug11cnt--;
            for (a = 0; a < debug11cnt; a++)
                printf(" ");
            printf("* </ Rpar>");
            printf(" %s\n", lexeme);
#endif
        }
        else
        {
            error(ERROR_SYNTAX, lexeme, lineno);
        }
#ifdef DEBUG11
        debug11cnt--;
        for (a = 0; a < debug11cnt; a++)
            printf(" ");
        printf("* </ ID>");
        printf(" %s\n", lexeme);
#endif
    }
    else if (token == TOKEN_IF)
    {
#ifdef DEBUG11
        int a;
        for (a = 0; a < debug11cnt; a++)
            printf(" ");
        printf("* <If>");
        debug11cnt++;
        printf(" %s\n", lexeme);
#endif
        get_token();
        parse_expression();
        gen_code("tst", "-");
        strcpy(label1, new_label());
        gen_code("jeq", label1);
        if (token == TOKEN_THEN)
        {
            get_token();
        }
        else
        {
            error(ERROR_SYNTAX, lexeme, lineno);
        }
        parse_statement();
        if (token == TOKEN_ELSE)
        {
            strcpy(label2, new_label());
            gen_code("jmp", label2);
            gen_code("label", label1);
            get_token();
            parse_statement();
            gen_code("label", label2);
        }
        else
        {
            gen_code("label", label1);
        }
#ifdef DEBUG11
        debug11cnt--;
        for (a = 0; a < debug11cnt; a++)
            printf(" ");
        printf("* </ If>");
        printf(" %s\n", lexeme);
#endif
    }
    else if (token == TOKEN_WHILE)
    {
#ifdef DEBUG11
        int a;
        for (a = 0; a < debug11cnt; a++)
            printf(" ");
        printf("* <While>");
        debug11cnt++;
        printf(" %s\n", lexeme);
#endif
        strcpy(label1, new_label());
        gen_code("label", label1);
        get_token();
        parse_expression();
        gen_code("tst", "-");
        strcpy(label2, new_label());
        gen_code("jeq", label2);
        if (token == TOKEN_DO)
        {
            get_token();
        }
        else
        {
            error(ERROR_SYNTAX, lexeme, lineno);
        }
        parse_statement();
        gen_code("jmp", label1);
        gen_code("label", label2);
#ifdef DEBUG11
        debug11cnt--;
        for (a = 0; a < debug11cnt; a++)
            printf(" ");
        printf("* </ While>");
        printf(" %s\n", lexeme);
#endif
    }
    else if (token == TOKEN_REPEAT)
    {
#ifdef DEBUG11
        int a;
        for (a = 0; a < debug11cnt; a++)
            printf(" ");
        printf("* <Repeat>");
        debug11cnt++;
        printf(" %s\n", lexeme);
#endif
        strcpy(label1, new_label());
        gen_code("label", label1);
        get_token();
        parse_statement();
        if (token == TOKEN_UNTIL)
        {
            get_token();
        }
        else
        {
            error(ERROR_SYNTAX, lexeme, lineno);
        }
        parse_expression();
        gen_code("tst", "-");
        gen_code("jeq", label1);
#ifdef DEBUG11
        debug11cnt--;
        for (a = 0; a < debug11cnt; a++)
            printf(" ");
        printf("* </ Repeat>");
        printf(" %s\n", lexeme);
#endif
    }
    else if (token == TOKEN_GOTO)
    {
#ifdef DEBUG11
        int a;
        for (a = 0; a < debug11cnt; a++)
            printf(" ");
        printf("* <Goto>");
        debug11cnt++;
        printf(" %s\n", lexeme);
#endif
        get_token();
        if (token == TOKEN_ID)
        {
            gen_code("jmp", lexeme);
            get_token();
        }
        else
        {
            error(ERROR_SYNTAX, lexeme, lineno);
        }
#ifdef DEBUG11
        debug11cnt--;
        for (a = 0; a < debug11cnt; a++)
            printf(" ");
        printf("* </ Goto>");
        printf(" %s\n", lexeme);
#endif
    }
    else if (token == TOKEN_BEGIN)
    {
#ifdef DEBUG11
        int a;
        for (a = 0; a < debug11cnt; a++)
            printf(" ");
        printf("* <Begin>");
        debug11cnt++;
        printf(" %s\n", lexeme);
#endif
        get_token();
        parse_statement();
        while (token == TOKEN_SEMICOL)
        {
            get_token();
            parse_statement();
        }
        if (token == TOKEN_END)
        {
            get_token();
        }
        else
        {
            error(ERROR_SYNTAX, lexeme, lineno);
        }
#ifdef DEBUG11
        debug11cnt--;
        for (a = 0; a < debug11cnt; a++)
            printf(" ");
        printf("* <End>");
        printf(" %s\n", lexeme);
#endif
    }
    else if (token == TOKEN_RETURN)
    {
#ifdef DEBUG11
        int a;
        for (a = 0; a < debug11cnt; a++)
            printf(" ");
        printf("* <Return>");
        debug11cnt++;
        printf(" %s\n", lexeme);
#endif
        get_token();
        parse_expression();
        gen_code("store_id", "ans"); /* [9] */
        gen_code("return", "-");     /* [9] */
#ifdef DEBUG11
        debug11cnt--;
#endif
    }
    else
    {
        ;
    }
#ifdef DEBUG11
    debug11cnt--;
    for (a = 0; a < debug11cnt; a++)
        printf(" ");
    printf("* </ Statement>");
    printf(" %s\n", lexeme);
#endif
}

void parse_variable()
{
#ifdef DEBUG11
    int a;
    for (a = 0; a < debug11cnt; a++)
        printf(" ");
    printf("* <Variable>");
    debug11cnt++;
    printf(" %s\n", lexeme);
#endif
    switch (token)
    {
    case TOKEN_LONG:
        get_token();
        if (token == TOKEN_ID)
        {
            gen_code("var_long", lexeme);
            get_token();
        }
        else
        {
            error(ERROR_SYNTAX, lexeme, lineno);
        }
        break;
    case TOKEN_WORD:
        get_token();
        if (token == TOKEN_ID)
        {
            gen_code("var_word", lexeme);
            get_token();
        }
        else
        {
            error(ERROR_SYNTAX, lexeme, lineno);
        }
        break;
    case TOKEN_BYTE:
        get_token();
        if (token == TOKEN_ID)
        {
            gen_code("var_byte", lexeme);
            get_token();
        }
        else
        {
            error(ERROR_SYNTAX, lexeme, lineno);
        }
        break;
    default:
        error(ERROR_SYNTAX, lexeme, lineno);
        break;
    }
#ifdef DEBUG11
    debug11cnt--;
#endif
}

void parse_function()
{
#ifdef DEBUG11
    int a;
    for (a = 0; a < debug11cnt; a++)
        printf(" ");
    printf("* <Function>");
    debug11cnt++;
    printf(" %s\n", lexeme);
#endif
    if (token == TOKEN_FUNCTION)
    {
        get_token();
    }
    else
    {
        error(ERROR_SYNTAX, lexeme, lineno);
    }
    if (token == TOKEN_ID)
    {
        gen_code("start_func", lexeme); /* [1] */
        get_token();
    }
    else
    {
        error(ERROR_SYNTAX, lexeme, lineno);
    }
    if (token == TOKEN_LPAR)
    {
        get_token();
    }
    else
    {
        error(ERROR_SYNTAX, lexeme, lineno);
    }
    if (token == TOKEN_LONG ||
        token == TOKEN_WORD ||
        token == TOKEN_BYTE)
    {
        parse_variable();
        while (token == TOKEN_COMMA)
        {
            get_token();
            parse_variable();
        }
    }
    if (token == TOKEN_RPAR)
    {
        gen_code("start_prologue", "-"); /* [2] */
        get_token();
    }
    else
    {
        error(ERROR_SYNTAX, lexeme, lineno);
    }
    while (token == TOKEN_LONG ||
           token == TOKEN_WORD ||
           token == TOKEN_BYTE)
    {
        parse_variable();
        if (token == TOKEN_SEMICOL)
        {
            get_token();
        }
        else
        {
            error(ERROR_SYNTAX, lexeme, lineno);
        }
    }
    gen_code("end_prologue", "-"); /* [3] */
    parse_statement();
    gen_code("end_func", "-"); /* [4] */
#ifdef DEBUG11
    debug11cnt--;
#endif
}

void parse_program()
{
#ifdef DEBUG11
    int a;
    for (a = 0; a < debug11cnt; a++)
        printf(" ");
    printf("* <Program>");
    debug11cnt++;
    printf(" %s\n", lexeme);
#endif
    gen_code("start", "main"); /* [1] */
    while (token == TOKEN_LONG ||
           token == TOKEN_WORD ||
           token == TOKEN_BYTE ||
           token == TOKEN_FUNCTION)
    {
        if (token == TOKEN_LONG ||
            token == TOKEN_WORD ||
            token == TOKEN_BYTE)
        {
            parse_variable();
            switch (token)
            {
            case TOKEN_COL:
                get_token();
                if (token == TOKEN_NUM)
                {
                    get_token();
                }
                gen_code("set_address", lexvalue); /* [2] */
                break;
            default:
                break;
            }
            if (token == TOKEN_SEMICOL)
            {
                get_token();
            }
        }
        else if (token == TOKEN_FUNCTION)
        {
            parse_function();
            if (token == TOKEN_SEMICOL)
            {
                get_token();
            }
            else
            {
                error(ERROR_SYNTAX, lexeme, lineno);
            }
        }
    }
    gen_code("label", "main"); /* [3] */
    parse_statement();
    gen_code("end", "-"); /* [4] */
#ifdef DEBUG11
    debug11cnt--;
#endif
}

void parse()
{
    parse_program();
}
