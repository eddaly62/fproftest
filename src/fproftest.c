// fproftest.c
// test of using fprof, function profile utility

#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <fcntl.h>
#include <fprof.h>
#include <unistd.h>

enum ETIME {START = 0, END = 1};

typedef void(*cb_func_c)(char*);

struct timeval start, end;

struct DAP_PATTERN_LUT{
    char *pattern;
    cb_func_c cb;
};

void callback1(char *s);

struct DAP_PATTERN_LUT relut[] = {
    {"033A", &callback1},
};

void callback1(char *s){
    fprintf(stdout, "callback1 function called, pattern = %s\n", s);
    return;
}

void do_nothing1(void) {
	printf("do nothing 1\n");
	(*relut[0].cb)(relut[0].pattern);
	// create an error
	open("nofile.txt", O_RDONLY);
}

void do_nothing2(void) {
	printf("do nothing 2\n");
	(*relut[0].cb)(relut[0].pattern);
}

void do_something(void) {
	int fd;
	char buf[10];
	(*relut[0].cb)(relut[0].pattern);
	// make an error
    read(fd, buf, 10);
}

// Determine elapse time
double elapsed_time(enum ETIME sts, struct timeval *start, struct timeval *end)
{
	long long startusec = 0, endusec = 0;
	double elapsed = 0;

	do_nothing1();

	do_nothing2();

	do_something();

	(*relut[0].cb)(relut[0].pattern);

	if (sts == START)
	{
		gettimeofday(start, NULL);
	}
	else
	{
		gettimeofday(end, NULL);
		startusec = start->tv_sec * 1000000 + start->tv_usec;
		endusec = end->tv_sec * 1000000 + end->tv_usec;
		elapsed = (double)(endusec - startusec) / 1000000.0;	// seconds
		elapsed = (double)(endusec - startusec);				// usec
	}
	return elapsed;
}


int main(int argc, char *argv[])
{
	double elapsedt=0;

	elapsedt = elapsed_time(START, &start, &end);
	sleep(1);
	elapsedt = elapsed_time(END, &start, &end);

	fprintf(stdout, "Elapsed time is %f usec\n", elapsedt);

	while(1);
	exit(EXIT_SUCCESS);

}



