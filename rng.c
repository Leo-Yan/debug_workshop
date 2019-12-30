#include <stdlib.h>
#include <stdio.h>
#include <time.h>

int main()
{
	float max = 5000;
	float r = max * ((float)rand()/(float)RAND_MAX - 0.5);
	int i;

	for (i = 0; i < 10000; i++) {
		r = max * ((float)rand()/(float)RAND_MAX - 0.5);
		printf("%.2f\n", r);
	}
}
