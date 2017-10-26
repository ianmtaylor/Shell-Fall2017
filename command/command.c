#include "./command.h"

Command * command(char * newString)
{
	Command * cmd = (Command *)calloc(1, sizeof(Command));
        cmd->data = (char *)calloc(strlen(newString)+1, sizeof(char));
        strncpy(cmd->data, newString, strlen(newString));
        return cmd;

}


void * buildTypeCommand(char * newString)
{
	Command * cmd = command(newString);
	return (void *)cmd;
}

void cleanCommand(void * cmd)
{
	Command * toClean = (Command *)cmd;
	free(toClean->data);
	toClean->data = NULL;
	free(toClean);
	toClean = NULL;
}

void printCommand(void * cmd)
{
	Command * toPrint = (Command *)cmd;
	printf("%s\n", toPrint->data);
}

void printCmdFile(void * cmd, FILE * fp)
{
	Command * toPrint = (Command *)cmd;
	fprintf(fp, "%s", toPrint->data);
}

char * getDataCMD(void * cmd)
{
	Command * newCmd = (Command *)cmd;
	return newCmd->data;
}

int compareCMD(const void * ptr, const void * ptr2)
{
	Command * cm1 = (Command *)ptr;
	Command * cm2 = (Command *)ptr2;

	return strcmp(cm1->data, cm2->data);
}




