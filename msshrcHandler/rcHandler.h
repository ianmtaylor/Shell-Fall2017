#ifndef RCHANDLER_H
#define RCHANDLER_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include "../linkedlist/linkedList.h"
#include "../linkedlist/listUtils.h"
#include "../utils/myUtils.h"
#include "../command/command.h"
#include "../history/history.h"

int fileExists(const char * filename);
int createDefaultRCFile();
LinkedList * loadAlias(LinkedList * ref, FILE * fp);
int startsWith(char * str, char * subString);
char * cleanStr(char * str);
void makeRef(LinkedList * ref, char * newref);
char * getPath();
void findAlias(LinkedList * refList, LinkedList * aliasList, char * s);

#endif
