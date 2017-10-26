#ifndef COMMAND_H
#define COMMAND_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct command
{
	char * data;
};
typedef struct command Command;

Command * command(char * newString);
void * buildTypeCommand(char * newString);

void cleanCommand(void * cmd);

void printCommand(void * cmd);

void printCmdFile(void * cmd, FILE * fp);

char * getDataCMD(void * cmd);

int compareCMD(const void * ptr, const void * ptr2);

Command * getAliasInfo(Command * ref, char * s);

#endif




