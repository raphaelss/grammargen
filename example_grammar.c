#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void
gen_seq(int n, const char *str)
{
	if (n <= 0) {
		fwrite(str, 1, strlen(str), stdout);
		return;
	}
	double r;
	do {
		switch(*str) {
		case 'o':
			gen_seq(n-1, "f[o]o");
			break;
		case 'f':
			r = drand48() * 2.0;
			if (r < 1.0) {
				gen_seq(n-1, "ff");
			} else {
				gen_seq(n-1, "xx");
			}
			break;
		default:
			putchar(*str);
			break;
		}
	} while (*++str);
}

int
main(int argc, char **argv)
{
	int n = atoi(argv[1]);
	int seed = atoi(argv[2]);
	srand48_deterministic(seed);
	gen_seq(n, "o");
	return 0;
}
