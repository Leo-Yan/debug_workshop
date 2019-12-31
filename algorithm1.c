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

static int max(int a, int b)
{
	if (a >= b)
		return a;
	else
		return b;
}

static int seq_max_sum(int *buf, int seq_num)
{
	int i, j, k;
	int maxsofar = 0, sum;

	for (i = 0; i < seq_num; i++) {
		for (j = i; j < seq_num; j++) {
			sum = 0;
			for (k = i; k <= j; k++) {
				sum += buf[k];
				maxsofar = max(maxsofar, sum);
			}
		}
	}

	return maxsofar;
}

int main(void)
{
	int seq_num;
	int *seq_buf, max_sum;

	seq_num = seq_length("number_sequence.txt");
	printf("%d\n", seq_num);

	seq_buf = malloc(sizeof(int) * seq_num);

	seq_prepare_buffer("number_sequence.txt", seq_buf, seq_num);

	//for (int i = 0; i < seq_num; i++)
	//	printf("%.2lf\n", seq_buf[i]);

	max_sum = seq_max_sum(seq_buf, seq_num);
	printf("max_sum = %d\n", max_sum);

	return 0;
}
