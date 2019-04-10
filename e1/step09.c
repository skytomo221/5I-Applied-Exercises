#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

/*------------------------------------------------------------*/

int char_pos(char *s, char c)
{
	int i;

	for (i = 0; s[i] != '\0'; i++) {
		if (s[i] == c) {
			return i;
		}
	}
	return -1;
}

/*------------------------------------------------------------*/

char c;

void get_char(FILE *in)
{
	c = fgetc(in);
	return;
}

/*------------------------------------------------------------*/

int end_of_lexeme;
char lexeme[BUFSIZ];

void save_char(char c)
{
	lexeme[end_of_lexeme++] = c;
	lexeme[end_of_lexeme] = '\0';
}

/*------------------------------------------------------------*/

#define ALPHABET "abcdefghijklmnopqrstuvwxyz"
#define DIGIT "0123456789"

#define TOKEN_EOF -1
#define TOKEN_ERROR 0
#define TOKEN_ID 1

/*------------------------------------------------------------*/


int main(int argc, char *argv[])
{
	FILE *in;
	FILE *out;
	char c;

	if (argc == 3) {
		if ((in = fopen(argv[1], "r")) == NULL) {
			perror("fopen for input");
			exit(1);
		}
		if ((out = fopen(argv[2], "w")) == NULL) {
			perror("fopen for output");
			exit(1);
		}
	} else if (argc == 2) {
		if ((in = fopen(argv[1], "r")) == NULL) {
			perror("fopen for input");
			exit(1);
		}
		out = stdout;
	} else {
		fprintf(stderr, "use: %s input [output]\n", argv[0]);
		exit(1);
	}

	convert(in, out);

	fclose(in);
	fclose(out);
	return 0;
}
