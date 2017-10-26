#include "./redirect.h"




char ** parsePreRed(char *s, int * preCount, int c)
{
	char ** pre = NULL;
	char * thread;
	char * token;
	char * buffer = (char *)calloc(strlen(s)+1, sizeof(char));
	strncpy(buffer, s, strlen(s));
	if(1 == c)
		token = strtok_r(buffer, ">", &thread);
	else
		token = strtok_r(buffer, "<", &thread);

	*preCount = makeargs(token, &pre);
	free(buffer);
	buffer = NULL;
	return pre;
}

char ** parsePostRed(char *s, int * postCount, int c)
{
	char ** post = NULL;
	char * thread;
	char * token;
	char * buffer = (char *)calloc(strlen(s)+1, sizeof(char));
	strncpy(buffer, s, strlen(s));
	if(1 == c)
	{
		token = strtok_r(buffer, ">", &thread);
		token = strtok_r(NULL, ">", &thread);
	}
	else
	{
		token = strtok_r(buffer, "<", &thread);
		token = strtok_r(NULL, "<", &thread);
	}

	*postCount = makeargs(token, &post);
	free(buffer);
	buffer = NULL;
	return post;
}


void redTo(int c, char ** preRed, char ** postRed)
{
	int fd;
	int status;
	pid_t pid = fork();

	if(pid != 0)
	{
		waitpid(pid, &status, 0);
		if(status != 0)
			printf("\nCommand invalid or Incorrect Path!!!\n");	
	}
	else
	{
		if(c == 1) // >
		{
	    		fd = open(postRed[0], O_WRONLY | O_CREAT | O_TRUNC, 0666);
			dup2(fd, 1);
			close(fd);
			
		}
		else // <
		{
    			fd = open(postRed[0], O_RDONLY);
			if(fd < 0)
			{
				printf("\n\nRedirection Error: Can't open file...Sorry\n\n");
				return;
			}

			
			dup2(fd, 0);
			close(fd);
    		
    		
		}
		forkItRedirect(preRed);
	}
}

int containsDirect(char * cmd) 
{

	int x = 0;
	for(; x < strlen(cmd); x++)
	{
		if(cmd[x] == '>' || cmd[x] == '<')
		{
			if(cmd[x] == '>')
				return 1;
			else
				return 2;
		}
			
	}
	return 0;
}

int countRedirect(char * cmd)
{
	int count = 0;
	int x = 0;
	for(; x < strlen(cmd); x++)
	{
		if(cmd[x] == '>' || cmd[x] == '<')
		{
			count++;
		}
			
	}
	return count;
}


