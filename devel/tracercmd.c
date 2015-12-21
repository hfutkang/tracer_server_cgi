#include <fcgi_stdio.h>
#include <stdlib.h>
#include <string.h>
#include "commands.h"

typedef int (*processor_t)(void);

struct command_processor {
	char* command;
	processor_t proc;
};

struct command_processor command_processors[] = {
	{"getposition", getposition},
	{"getpositions", getpositions},
	{"reportposition", reportposition},
	{"reportLBS", reportLBS},
	{"getLBS", getLBS},
	{"activate", activate},
	{"getinfo", getinfo},
	{"getlastedition", getlastedition}
};

#define ARRAY_SIZE(array)	sizeof(array)/sizeof(array[0])

static int process_command(char* qstr)
{
	struct command_processor cp;
	int i;
	char* cmd;

	init_param(qstr);
	cmd = get_param_value("cmd");
	if (!cmd)
		return 0;
	for (i = 0; i < ARRAY_SIZE(command_processors); ++i) {
		cp = command_processors[i];
		if (0 == strcmp(cp.command, cmd)) {
			printf("Status: 200 OK\r\n");
			printf("Content-type:text/xml\r\n\r\n");
			/*
			printf("<p> cmd is %s  </ p>", cmd);
			*/
			return cp.proc();
		}
	}
#if 1
	int count = 0;
	printf("Status: 200 OK\r\n");
	printf("Content-type:text/html\r\n\r\n");
	printf("<p> Hello FastCGI !  </ p>");
	printf("<br /> Request number = [%d]", ++count);
	printf("<br /> Process ID: %d ", getpid());
#endif
	return -1;
}

int main()
{
	int ret;
	char *qstr;
	char cmd[128];

	while (FCGI_Accept() >= 0) {
		char *param;

		qstr = getenv("QUERY_STRING");

		ret = process_command(qstr);
#if 0
		printf("Status: 200 OK\r\n");
		printf("Content-type:text/html\r\n\r\n");
		printf("<p> Hello FastCGI !  </ p>");
		printf("<br /> Request number = [%d]", ++count);
		printf("<br /> Process ID: %d ", getpid());
		if (cmd)
			printf("<br /> cmd is %s \n", cmd);
		else
			printf("<br/> cmd is null\n");
		printf("\n");
#endif
	}

	return 0;
}

