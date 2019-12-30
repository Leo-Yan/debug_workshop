#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <errno.h>
#include <err.h>

static int seq_length(char *file_name)
{
        int fd = open(file_name, O_RDONLY);
        struct stat fs;
        char *buf, *buf_end;
        char *begin, *end, c;
	int line_num = 0;
 
        if (fd == -1) {
                err(1, "open: %s", file_name);
                return 0;
        }
 
        if (fstat(fd, &fs) == -1) {
                err(1, "stat: %s", file_name);
                return 0;
        }
 
        /* fs.st_size could have been 0 actually */
        buf = mmap(0, fs.st_size, PROT_READ, MAP_SHARED, fd, 0);
        if (buf == (void*) -1) {
                err(1, "mmap: %s", file_name);
                close(fd);
                return 0;
        }
 
        buf_end = buf + fs.st_size;
 
        begin = end = buf;
        while (1) {
                if (! (*end == '\r' || *end == '\n')) {
                        if (++end < buf_end)
				continue;
                } else if (1 + end < buf_end) {
                        /* see if we got "\r\n" or "\n\r" here */
                        c = *(1 + end);
                        if ( (c == '\r' || c == '\n') && c != *end)
                                ++end;
                }
 
		line_num++;
 
                if ((begin = ++end) >= buf_end)
                        break;
        }
 
        munmap(buf, fs.st_size);
        close(fd);
        return line_num;
}

static int seq_prepare_buffer(char *file_name, double *seq_buf, int seq_num)
{
        int fd = open(file_name, O_RDONLY);
        struct stat fs;
        char *buf, *buf_end;
        char *begin, *end, c;
	int i = 0;
 
        if (fd == -1) {
                err(1, "open: %s", file_name);
                return 0;
        }
 
        if (fstat(fd, &fs) == -1) {
                err(1, "stat: %s", file_name);
                return 0;
        }
 
        /* fs.st_size could have been 0 actually */
        buf = mmap(0, fs.st_size, PROT_READ, MAP_SHARED, fd, 0);
        if (buf == (void*) -1) {
                err(1, "mmap: %s", file_name);
                close(fd);
                return 0;
        }
 
        buf_end = buf + fs.st_size;
 
        begin = end = buf;
        while (1) {
                if (! (*end == '\r' || *end == '\n')) {
                        if (++end < buf_end)
				continue;
                } else if (1 + end < buf_end) {
                        /* see if we got "\r\n" or "\n\r" here */
                        c = *(1 + end);
                        if ( (c == '\r' || c == '\n') && c != *end)
                                ++end;
                }
 
		seq_buf[i] = strtod(begin, NULL);
		i++;
 
                if ((begin = ++end) >= buf_end)
                        break;
        }
 
        munmap(buf, fs.st_size);
        close(fd);
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
