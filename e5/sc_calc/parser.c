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
        gen_code("eq", "-");
    }
    else if (token == TOKEN_NE)
    {
        get_token();
        gen_code("ne", "-");
    }
    else if (token == TOKEN_LT)
    {
        get_token();
        gen_code("lt", "-");
    }
    else if (token == TOKEN_LE)
    {
        get_token();
        gen_code("le", "-");
    }
    else if (token == TOKEN_GT)
    {
        get_token();
        gen_code("gt", "-");
    }
    else if (token == TOKEN_GE)
    {
        get_token();
        gen_code("ge", "-");
    }
    else
    {
        return;
    }
    parse_polynomial();
}

void parse()
{
    parse_expression();
}
