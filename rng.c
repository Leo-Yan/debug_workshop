#include <stdlib.h>
#include <stdio.h>
#include <time.h>

int main()
{
	int max = 5000;
	int r, i;

	for (i = 0; i < 100000; i++) {
		r = (int)(max * ((float)rand()/(float)RAND_MAX - 0.5));
		printf("%d\n", r);
	}
}
