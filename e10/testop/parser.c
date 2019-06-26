#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "sc.h"

void parse_expression();

void parse_factor()
{
    if (token == TOKEN_ID)
    {
        gen_code("load_id", lexeme);
        get_token();
    }
    else if (token == TOKEN_NUM)
    {
        gen_code("load_num", lexvalue);
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
        gen_code("not", "-");
    }
    else
    {
        error(ERROR_SYNTAX, lexeme, lineno);
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
        else if (token == TOKEN_PERCENT)
        {
            get_token();
            parse_factor();
            gen_code("mod", "-");
        }
        else if (token == TOKEN_SHIFTL)
        {
            get_token();
            parse_factor();
            gen_code("shiftl", "-");
        }
        else if (token == TOKEN_SHIFTR)
        {
            get_token();
            parse_factor();
            gen_code("shiftr", "-");
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

void parse_statement()
{
    char id[BUFSIZ];     /* [1] */
    char label1[BUFSIZ]; /* [1] */
    char label2[BUFSIZ]; /* [1] */
    if (token == TOKEN_ID)
    {
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
        else
        {
            error(ERROR_SYNTAX, lexeme, lineno);
        }
    }
    else if (token == TOKEN_IF)
    {
        get_token();
        parse_expression();
        gen_code("tst", "-");        /* [2] */
        strcpy(label1, new_label()); /* [2] */
        gen_code("jeq", label1);     /* [2] */
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
            strcpy(label2, new_label()); /* [3] */
            gen_code("jmp", label2);     /* [3] */
            gen_code("label", label1);   /* [3] */
            get_token();
            parse_statement();
            gen_code("label", label2); /* [4] */
        }
        else
        {
            gen_code("label", label1); /* [5] */
        }
    }
    else if (token == TOKEN_WHILE)
    {
        strcpy(label1, new_label()); /* [2] */
        gen_code("label", label1);   /* [2] */
        get_token();
        parse_expression();
        gen_code("tst", "-");        /* [3] */
        strcpy(label2, new_label()); /* [3] */
        gen_code("jeq", label2);     /* [3] */
        if (token == TOKEN_DO)
        {
            get_token();
        }
        else
        {
            error(ERROR_SYNTAX, lexeme, lineno);
        }
        parse_statement();
        gen_code("jmp", label1);   /* [4] */
        gen_code("label", label2); /* [4] */
    }
    else if (token == TOKEN_REPEAT)
    {
        strcpy(label1, new_label()); // 1
        gen_code("label", label1);   // 1?
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
        gen_code("tst", "-");    // 4
        gen_code("jeq", label1); // 5
    }
    else if (token == TOKEN_GOTO)
    {
        get_token();
        if (token == TOKEN_ID)
        {
            gen_code("jmp", lexeme); /* [1] */
            get_token();
        }
        else
        {
            error(ERROR_SYNTAX, lexeme, lineno);
        }
    }
    else if (token == TOKEN_BEGIN)
    {
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
    }
    else
    {
        ;
    }
}

void parse_variable()
{
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
}

void parse_program()
{
    gen_code("start", "main");
    while (token == TOKEN_LONG ||
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
            gen_code("set_address", lexvalue);
            break;
        default:
            break;
        }
        if (token == TOKEN_SEMICOL)
        {
            get_token();
        }
    }
    gen_code("label", "main");
    parse_statement();
    gen_code("end", "-");
}

void parse()
{
    parse_program();
}
