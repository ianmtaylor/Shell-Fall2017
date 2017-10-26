#include "process.h"
#include <sys/wait.h>
#include <unistd.h>

int forkIt(char ** argv, int argc)
{
	
	int status;
	pid_t pid = fork();
	if(pid != 0)
	{
		waitpid(pid, &status, 0);
		if(status != 0)
		{
			printf("\n\nINCORRECT COMMAND OR PATH.\n\n");
			clean(argc, argv);
			return -1;
		}
	}
	else
	{
		int res = execvp(argv[0], argv);
		exit(-1);

	}

	return 0;
}

void forkItRedirect(char ** argv)
{
	execvp(argv[0], argv);
	exit(-1);
	
}
