#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	FILE *in;
	char c;

	if (argc != 2) {
		fprintf(stderr, "use: %s file\n", argv[0]);
		exit(1);
	}

	in = fopen(argv[1], "r");

	if (in == NULL) {
		fprintf(stderr, "fopen for input: file not found\n");
		exit(1);
	}

	c = fgetc(in);

	while (c != EOF) {
		fputc(c, stdout);
		c = fgetc(in);
	}

	fclose(in);
	return 0;
}
