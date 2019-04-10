#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	FILE *in;
	char c;

	if (argc != 2) {
		fprintf(stderr, "use: step02 file\n");
		exit(1);
	}

	in = fopen(argv[1], "r");

	if (in == NULL) {
		fprintf(stderr, "file not found\n");
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
