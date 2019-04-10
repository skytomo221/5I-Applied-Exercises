#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	FILE *in;
	FILE *out;
	char c;

	if (argc != 3) {
		fprintf(stderr, "use: %s input output\n", argv[0]);
		exit(1);
	}

	if ((in = fopen(argv[1], "r")) == NULL) {
		perror("fopen for input");
		exit(1);
	}
	if ((out = fopen(argv[2], "w")) == NULL) {
		perror("fopen for output");
		exit(1);
	}

	while ((c = fgetc(in)) != EOF) {
		fputc(c, out);
	}

	fclose(in);
	fclose(out);
	return 0;
}
