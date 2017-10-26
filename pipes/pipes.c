#include "pipes.h"
#include "../tokenize/makeArgs.h"

int containsPipe(char *s)
{
	int contains = 0;

	int x = 0;
	for(; x < strlen(s); x++)
	{
		if(s[x] == '|')
			contains++;
	}
	return contains;
}
char ** parsePrePipe(char *s, int * preCount)
{
	char ** pre = NULL;
	char * thread;
	char * buffer = (char *)calloc(strlen(s)+1, sizeof(char));
	strncpy(buffer, s, strlen(s));
	char * token = strtok_r(buffer, "|", &thread);
	*preCount = makeargs(token, &pre);
	free(buffer);
	buffer = NULL;
	return pre;
}
char ** parsePostPipe(char *s, int * postCount)
{
	char ** post = NULL;
	char * thread;
	char * buffer = (char *)calloc(strlen(s)+1, sizeof(char));
	strncpy(buffer, s, strlen(s));
	char * token = strtok_r(buffer, "|", &thread);
	token = strtok_r(NULL, "|", &thread);
	*postCount = makeargs(token, &post);
	free(buffer);
	buffer = NULL;
	return post;
}
void pipeIt(char ** prePipe, char ** postPipe)
{
	printf("\n");
	pid_t pid;
	int fd[2], res, status;

	res = pipe(fd);

	if(res < 0)
	{
		printf("Pipe Failure\n");
		exit(-1);
	}

	pid = fork();

	if(pid != 0)
	{
   		waitpid(pid, &status, 0);
	}
	else
	{
		pipe(fd);
		pid_t pid2 = fork();

		if(pid2 != 0)
		{
			waitpid(pid, &status, 0);
			close(fd[1]);
			dup2(fd[0], 0);
			close(fd[0]);
			if((status != 0)||(execvp(postPipe[0], postPipe) == -1))
			{
				printf("\nInvalid Command\n");
				exit(-1);
			}
		}
		else
		{
			close(fd[0]);
			close(1);
			dup(fd[1]);
			close(fd[1]);
			if(execvp(prePipe[0], prePipe) == -1)
			{
				printf("\nInvalid Command\n");
				exit(-1);
			}
		}
	}

	

}


int pipeWithRed(char ** prePipe, char ** postPipe, char * file, int direct)
{
	pid_t pid;
	int fd[2], res, status, fd2;

	if(file != NULL)
	{
		if(direct == 1) // >
		{
	    		fd2 = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0666);
			//dup2(fd2, 1);
			//close(fd2);
			
		}
		else // <
		{
    			printf("\nBad operands...probably the wrong redirection symbol\n");
		}
	}

	if(prePipe == NULL || postPipe == NULL)
	{
		//printf("\n\nOperands of Pipe are null\n\n");
		return -1;
	}

	res = pipe(fd);

	if(res < 0)
	{
		printf("Pipe Failure\n");
		exit(-1);
	}

	pid = fork();
	
	if(pid != 0)
	{
   		waitpid(pid, &status, 0);
		if(status != 0)
			return -1;
	}
	else
	{
		pipe(fd);
		pid_t pid2 = fork();

		if(pid2 != 0)
		{
			waitpid(pid, &status, 0);
			close(fd[1]);
			dup2(fd[0], 0);
			close(fd[0]);
			dup2(fd2, 1);
			close(fd2);
			if((status != 0)||(execvp(postPipe[0], postPipe) == -1))
			{
				exit(-1);
			}
		}
		else
		{
			close(fd[0]);
			close(1);
			dup(fd[1]);
			close(fd[1]);
			if(execvp(prePipe[0], prePipe) == -1)
			{
				exit(-1);
			}
		}
	}

	
	
}



