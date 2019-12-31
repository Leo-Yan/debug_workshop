#include <stdio.h>
#include <stdlib.h>

static int seq_length(char *file_name)
{
	FILE *stream;
	char *line = NULL;
	size_t len;
	int line_num = 0;

	stream = fopen(file_name, "r");
	if (!stream)
		return -1;

	while ((getline(&line, &len, stream)) != -1)
		line_num++;

	free(line);
	fclose(stream);
	return line_num;
}

static int seq_prepare_buffer(char *file_name, int *buf, int seq_num)
{
	FILE *stream;
	char *line = NULL;
	size_t len;
	int i = 0;

	stream = fopen(file_name, "r");
	if (!stream)
		return -1;

	while ((getline(&line, &len, stream)) != -1) {
		if (i >= seq_num)
			break;

		buf[i] = atoi(line);
		i++;
	}

	free(line);
	fclose(stream);
	return i;
}

//static double max(double a, double b)
//{
//	if (a >= b)
//		return a;
//	else
//		return b;
//}

#define max(a, b) ((a) > (b) ? (a) : (b))

static inline void prefetch(const void *ptr)
{
	asm volatile("prfm pldl1keep, %a0\n" : : "p" (ptr));
}

static int seq_max_sum(int *buf, int seq_num)
{
	int maxsofar, sum;
	int val;
	int i, j;
	//int maxsofar = 0, sum;

	//for (i = 0; i < seq_num; i++) {

	//	sum = 0;

	//	for (j = i; j < seq_num; j++) {
	//		sum += buf[j];
	//		prefetch(buf + j + 1);
	//		maxsofar = max(maxsofar, sum);
	//	}
	//}


	asm volatile (
		"1:	cmp %w0, #0x0\n"
		"b.le	5f\n"
		"mov	%5, #0x0\n"
		"mov	%2, #0x0\n"
		"2:	mov	%6, %5\n"
		"mov	%3, #0x0\n"
		"3: 	ldr	%w4, [%1, %6, lsl #2]\n"
		"add	%6, %6, #0x1\n"
		"add	%w3, %w3, %w4\n"
		"cmp	%w2, %w3\n"
		"csel	%w2, %w2, %w3, ge  // ge = tcont\n"
		"cmp	%0, %6\n"
		"b.gt	3b\n"
		"add	%5, %5, #0x1\n"
		"cmp	%0, %5\n"
		"b.gt	2b\n"
		"5:	"
		: : "r" (seq_num), "r" (buf), "r" (maxsofar), "r" (sum), "r" (val), "r" (i), "r" (j));

	return maxsofar;
}

int main(void)
{
	int seq_num;
	int *seq_buf, max_sum;

	seq_num = seq_length("number_sequence.txt");
	printf("%d\n", seq_num);

	seq_buf = malloc(sizeof(int) * (seq_num + 4));

	seq_prepare_buffer("number_sequence.txt", seq_buf, seq_num);

	//for (int i = 0; i < seq_num; i++)
	//	printf("%.2lf\n", seq_buf[i]);

	max_sum = seq_max_sum(seq_buf, seq_num);
	printf("max_sum = %d\n", max_sum);

	return 0;
}
