#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "sc.h"

#define MAX_STACKS 10

typedef struct stack
{
    int kind[MAX_STACKS];
    int value[MAX_STACKS];
    int top;
} Stack;

void push(Stack *self, int kind, int value);
void *pop(Stack *self);
void init(Stack *self);
int is_empty(Stack *self);

void push(Stack *self, int kind, int value)
{
    if (self->top < 0)
        self->top = 0;
    else
        self->top++;
    self->kind[self->top] = kind;
    self->value[self->top] = value;
}
void *pop(Stack *self)
{
    self->top--;
}
void init(Stack *self)
{
    self->top = -1;
}
int is_empty(Stack *self)
{
    return self->top < 0;
}
int peek_kind(Stack *self)
{
    return self->kind[self->top];
}
int peek_value(Stack *self)
{
    return self->value[self->top];
}
