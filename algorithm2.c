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

static int seq_prepare_buffer(char *file_name, double *buf, int seq_num)
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

		buf[i] = strtod(line, NULL);
		i++;
	}

	free(line);
	fclose(stream);
	return i;
}

static double max(double a, double b)
{
	if (a >= b)
		return a;
	else
		return b;
}

static int seq_max_sum(double *buf, int seq_num)
{
	int i, j;
	double maxsofar = 0, sum;

	for (i = 0; i < seq_num; i++) {

		sum = 0;

		for (j = i; j < seq_num; j++) {
			sum += buf[j];
			maxsofar = max(maxsofar, sum);
		}
	}

	return maxsofar;
}

int main(void)
{
	int seq_num;
	double *seq_buf, max_sum;

	seq_num = seq_length("number_sequence.txt");
	printf("%d\n", seq_num);

	seq_buf = malloc(sizeof(double) * seq_num);

	seq_prepare_buffer("number_sequence.txt", seq_buf, seq_num);

	//for (int i = 0; i < seq_num; i++)
	//	printf("%.2lf\n", seq_buf[i]);

	max_sum = seq_max_sum(seq_buf, seq_num);
	printf("max_sum = %.2lf\n", max_sum);

	return 0;
}
