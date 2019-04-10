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

	if ((in = fopen(argv[1], "r")) == NULL) {
		perror("fopen for input");
		exit(1);
	}

	while ((c = fgetc(in)) != EOF) {
		fputc(c, stdout);
	}

	fclose(in);
	return 0;
}
