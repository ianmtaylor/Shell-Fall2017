//AUTHOR: IAN TAYLOR
#include "./pipes/pipes.h"
#include "./utils/myUtils.h"
#include "./process/process.h"
#include "./tokenize/makeArgs.h"
#include "./msshrcHandler/rcHandler.h"
#include "./linkedlist/linkedList.h"
#include "./linkedlist/listUtils.h"
#include "./history/history.h"
#include "./command/command.h"
#include "./redirect/redirect.h"

int main()
{

/*********Check if .msshrc exists*****************************************************/
  int rcExists = fileExists(".msshrc");
  FILE * fp = NULL;
  char * path = NULL;
  if(rcExists == 1 && !(checkEOF(".msshrc") <= 1))
  {
	fp = fopen(".msshrc", "r");
  }

/******************* Connect to/create histFile *************************************/
History * hist = NULL;
if(rcExists == 1)
{
  hist = history(0,0);
  getHistInfo(hist, fp);
}
else
{
  hist = history(100, 1000);
}

checkHistory(hist);
loadHistory(hist, buildTypeCommand, buildNode_Type, cleanCommand);
LinkedList * cmdLL = linkedList();

/****alias stuff****/
LinkedList * aliasRef = linkedList();
LinkedList * aliasLL = NULL;
if(rcExists == 1)
	aliasLL = loadAlias(aliasRef, fp);
else
	aliasLL = linkedList();

/****close rc file***/
if(rcExists == 1)
{
	rewind(fp);
	fclose(fp);
}
	
/***path stuff*****/
if(rcExists == 1)
{
	path = getPath();
	if(path != NULL)
		setenv("PATH", path, 1);
}

/*********************Command Handler************************************/
  int argc, pipeCount, direct, redCount;
  char **argv = NULL, s[MAX];
  int preCount = 0, postCount = 0;
  char ** prePipe = NULL, ** postPipe = NULL, **postRedPipe = NULL;
  int preRedCount = 0, postRedCount = 0;
  char ** preRed = NULL, ** postRed = NULL;
  char * redFileName = NULL, * repeat = NULL;
  int fail = 0, mustExit = 0;
  printf("command?: ");
  fgets(s, MAX, stdin);
  strip(s);

  while(strcmp(s, "exit") != 0)
  {
	mustExit = 0; //reset bangErr flag
	repeat = lookatLast(cmdLL);
	if(repeat != NULL)//dont repeat last command...please work...
	{
		if(strcmp(s, repeat) != 0)
		{
			addToHistCount(hist, cmdLL, s, buildTypeCommand, buildNode_Type, cleanCommand);
		}
	}
	else
	{
		addToHistCount(hist, cmdLL, s, buildTypeCommand, buildNode_Type, cleanCommand);
	}
	findAlias(aliasRef, aliasLL, s); //convert alias
	
/*****************************************************BANG BLOCK!!!*******************************************************************/
	if(startsWith(s, "!") == 1)//bang or bangbang
	{
		if(startsWith(s, "!!") == 1)//bangbang
		{
			repeat = getCMDBang(cmdLL, cmdLL->size-1);
			if(NULL == repeat)//if no previous cmd for current session
			{
				printf("\nNo previous commands\n");
				mustExit = 1;
			}
			else if(cmdLL->size == 1 && strcmp(repeat, "!!") == 0)//EDGE CASE: start up shell; enter !!; get error; enter !! again
			{
				printf("\nNo previous commands\n");
				mustExit = 1;
			}
			else //copy the new command into s
			{
				
				memset(s, '\0', MAX*sizeof(char));
				strncpy(s, repeat, strlen(repeat));
			}
		}//end bangbang
		else//bang
		{
			removeBang(s);
			int toFind = atoi(s);
			if(0 == toFind) //if atoi fails
			{
				printf("\nError: %s is not a number or is out of range...try again\n", s);
				mustExit = 1;
			}
			else//if atoi succeeds
			{
				memset(s, '\0', MAX*sizeof(char));
				repeat = getCMDBang(cmdLL, toFind);

				if(NULL == repeat)//check to see if bang is a valid command
				{
					printf("\nError: Incorrect range for current session\n");
					mustExit = 1;
				}
				else//valid
				{	
					strncpy(s, repeat, strlen(repeat));
						
				}
			}
		}//end bang
	}//end bangbang or bang
/**************************************END BANG BLOCK****************************************************/
	if(mustExit != 1)//to catch incorrect bangs
	{
		pipeCount = containsPipe(s);
		direct = containsDirect(s);
		redCount = countRedirect(s);

		if(redCount < 2 && pipeCount < 2)
		{
		
			if(startsWith(s, "cd") == 1)//change directory
			{
				char * token = NULL;
				char * saveptr;

				token = strtok_r(s, " ", &saveptr);
				token = strtok_r(NULL, " ", &saveptr);
				if(NULL == token)
				{
					if(chdir(getenv("HOME")) < 0)
						printf("\nError changing to home directory\n");
				}
				else
				{
					if(chdir(token) < 0)
						printf("\nNo file or directory found\n");
				}

			}
			else if(startsWith(s , "history") == 1)//show current history
			{
				printCMDList(cmdLL, printCommand);
			}
			else if(startsWith(s, "alias"))//alias
			{
				char * token;
				char * saveptr;
				

				token = strtok_r(s, "=", &saveptr);
				makeRef(aliasRef, token);
				token = strtok_r(NULL, "=", &saveptr);
				token = cleanStr(token);
				Command * cmd = command(token);
				addLast(aliasLL, buildNode_Type((void *)cmd));
				free(token);
				token = NULL;

				
			}
			else if(startsWith(s, "unalias"))//unalias 
			{

				char * token;
				char * saveptr;

				token = strtok_r(s, " ", &saveptr);
				token = strtok_r(NULL, " ", &saveptr);
				Command * ref = command(token);
				int aliasLocation = countPlaces(aliasRef, ref, compareCMD);
				
				if(-1 == aliasLocation)
				{
					printf("\nAlias does not exist\n");
				}
				else
				{
					if(1 == aliasLocation)
					{
						removeFirst(aliasRef, cleanCommand);
						removeFirst(aliasLL, cleanCommand);
						cleanCommand(ref);
						ref = NULL;
					}
					else if(aliasLocation == aliasRef->size)
					{
						removeLast(aliasRef, cleanCommand);
						removeLast(aliasLL, cleanCommand);
						cleanCommand(ref);
						ref = NULL;
					}
					else
					{
						removeItem(aliasRef, buildNode_Type((void *)ref), cleanCommand, compareCMD);
						removeAliasItem(aliasLL, aliasLocation, cleanCommand);
						Node * cur = aliasLL->head->next;
						
						while(cur != NULL)
						{
							printCommand(cur->data);
							cur = cur->next;
							
						}
						
						cur = aliasRef->head->next;
						while(cur != NULL)
						{
							printCommand(cur->data);
							cur = cur->next;
						}
						
					}
				}
				
				
			}
			else if(startsWith(s, "PATH") == 1)
			{
				
				free(path);
				path = NULL;
				path = (char *)calloc(10000, sizeof(char));
				
				char * token;
				char * saveptr;
				char * token2;
				char * saveptr2;
				
				token = strtok_r(s, "=", &saveptr);
				token = strtok_r(NULL, "=", &saveptr);

				if(strstr(token, "$PATH") != NULL)
				{
					strncat(path, getenv("PATH"), strlen(getenv("PATH")));
					
					if(strstr(token, ":")!= NULL)
					{
						strncat(path, ":", strlen(":"));
						token2 = strtok_r(token, ":", &saveptr2);
						token2 = strtok_r(NULL, ":", &saveptr2);
						strncat(path, token2, strlen(token));
					}
					setenv("PATH",path, 1);
					
						
				}
				else
				{
					strncpy(path, token, strlen(token));
					setenv("PATH", path, 1);
					
				}
				
					
			}
			else if(1 == redCount && 1 == pipeCount)//redirect and pipe
			{
				char * token;
				char * saveptr;
				if(direct == 1)
					token = strtok_r(s, ">", &saveptr);
				else
					token = strtok_r(s, "< ", &saveptr);

				prePipe = parsePrePipe(token, &preCount);
				postPipe = parsePostPipe(token, &postCount);
		
				if(direct == 1)
					token = strtok_r(NULL, ">", &saveptr);
				else
					token = strtok_r(NULL, "<", &saveptr);
				removeSpaces(token);
				redFileName = (char *)calloc(strlen(token)+1, sizeof(char));
				strncpy(redFileName, token, strlen(token));
				fail = pipeWithRed(prePipe, postPipe, redFileName, direct);
				if(-1 == fail)
				{
					printf("\nInvalid Command\n");
					fail = 0;
				}
				free(redFileName);
				redFileName = NULL;
				clean(preCount, prePipe);
				clean(postCount, postPipe);
			}
			else if(pipeCount > 0 && redCount == 0)//pipe
			{
				prePipe = parsePrePipe(s, &preCount);
				postPipe = parsePostPipe(s, &postCount);
				pipeIt(prePipe, postPipe);
				clean(preCount, prePipe);
				clean(postCount, postPipe);
			}// end if pipeCount
			else if(direct > 0 && redCount == 1)//redirect
			{
				preRed = parsePreRed(s, &preRedCount, direct);
				postRed = parsePostRed(s, &postRedCount, direct);
				redTo(direct, preRed, postRed);
				clean(preRedCount, preRed);
				clean(postRedCount, postRed);
			}

			else
			{
				argc = makeargs(s, &argv);
		  		if(argc != -1)
		  			fail = forkIt(argv, argc);

				if(fail != -1)//if command didnt fail..clean args otherwise args is cleaned in process
				{
		  			clean(argc, argv);
		  			argv = NULL;
				}
				printf("\n");
			}
		}
		else
		{
			if(redCount >= 2)
				printf("\nToo many redirects!!!\n");
			else
				printf("\nToo many pipes!!!\n");	
		}
	
	}
	printf("command?: ");
	fgets(s, MAX, stdin);
      	strip(s);
	
  }// end while

/************************Save cmds into history and clean*******************************/
  
  loadIntoHistory(hist, cmdLL, cleanCommand);
  writeList(hist->theList, ".msshrc_history", printCmdFile);
  
  
  cleanHistory(hist, cleanCommand);//clean history
  clearList(cmdLL, cleanCommand);//clean cmd list
  free(cmdLL);
  cmdLL = NULL;
  clearList(aliasLL, cleanCommand);//clean alias
  free(aliasLL);
  clearList(aliasRef, cleanCommand);
  free(aliasRef);
  aliasLL = NULL;
  if(path != NULL)
  {
  	free(path);
  	path = NULL;
  }
  
  return 0;

}// end main

