#ifndef PIPES_H
#define PIPES_H

#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>

int containsPipe(char *s);
char ** parsePrePipe(char *s, int * preCount);
char ** parsePostPipe(char *s, int * postCount);
void pipeIt(char ** prePipe, char ** postPipe);
int pipeWithRed(char ** prePipe, char ** postPipe, char * file, int direct);

#endif 
