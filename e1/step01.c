#include <stdio.h>
#include <stdlib.h>

int main()
{
	FILE *in;
	char c;

	in = fopen("test.dat", "r");

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
