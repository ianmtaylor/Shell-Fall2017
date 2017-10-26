#include "rcHandler.h"
#include "../utils/myUtils.h"



int fileExists(const char * filename)
{
	FILE * fp;
        if((fp = fopen(filename, "r")) != NULL)
        {
                fclose(fp);
                return 1;
        }
        return 0;
}

int createDefaultRCFile()
{
	FILE * fp;

	fp = fopen(".msshrc", "w");

	if(fp == NULL)
	{
		printf("\n\nError Creating File\n\n");
		return 0;
	}
	fprintf(fp, "HISTCOUNT=100\n");
	fprintf(fp, "HISTFILECOUNT=1000\n");
	fprintf(fp, "\n");
	rewind(fp);
	fclose(fp);

	fp = fopen(".msshrc", "r");

	char s[100];
	fgets(s, 100, fp);
	strip(s);

	rewind(fp);
	fclose(fp);

	return 1;


}

LinkedList * loadAlias(LinkedList * ref, FILE * fp)
{
	char s[100];
	fgets(s, 100, fp);
	if(s != NULL)
		strip(s);
	LinkedList * newLL = linkedList();	
	Command * cmd;
	if(feof(fp))
		return newLL;
	
	char * token;
	char * saveptr;
	
	int flag = 0;
	while(!feof(fp) && flag == 0)
	{
		if(!feof(fp))
		{
			fgets(s, 100, fp);
			strip(s);
			flag = startsWith(s, "PATH=");
			if((s[0] != '\0' && s[0] != '\n') && flag == 0)
			{
				if(!feof(fp))
				{
					token = strtok_r(s, "=", &saveptr);
					makeRef(ref, token);
					token = strtok_r(NULL, "=", &saveptr);
					token = cleanStr(token);
					cmd = command(token);
					addLast(newLL, buildNode_Type((void *)cmd));
					free(token);
					token = NULL;
				}
			}
		}
	}

	return newLL;
	
}

void makeRef(LinkedList * ref, char * newref)
{
	char * token;
	char * saveptr;

	token = strtok_r(newref, " ", &saveptr);
	token = strtok_r(NULL, " ", &saveptr);
	Command * cmd = command(token);
	addLast(ref, buildNode_Type((void*)cmd));
}

int startsWith(char * str, char * subString)
{
	int x;
	for(x = 0; subString[x] != '\0'; x++)
	{
		if(str[x] != subString[x])
			return 0;
			
		
	}
	return 1;
}

char * cleanStr(char * str)
{
	char s[100];

	int x = 0;
	int y = 0;
	for(; str[x] != '\0'; x++)
	{
		if(str[x] !='"')
		{
			s[y] = str[x];
			y++;
		}
			
	}
	s[y] = '\0';
	char * newStr = (char*)calloc(strlen(s)+1, sizeof(char)); 
	strncpy(newStr, s, strlen(s));
	return newStr;

}

char * getPath()
{
	
	FILE * fp = fopen(".msshrc", "r");
	char s[10000];
	char * saveptr;
	char * token;
	char * token2;
	char * saveptr2;
	char * path;
	char * str = NULL;
	fgets(s, 10000, fp);
	strip(s);

	while(startsWith(s, "PATH=") != 1 && !feof(fp)) 
	{
		if(!feof(fp))
		{
			fgets(s, 10000, fp);
			strip(s);
			if(!feof(fp))
				s[strlen(s)] = '\0';
		}
	}
	if(startsWith(s, "PATH=") != 1)
	{
		int x = 0;
		memset(s, '\0', 10000*sizeof(char));
		strncpy(s, getenv("PATH"), strlen(getenv("PATH")));
	}
	else if(strstr(s, "$PATH") != NULL)
	{
		path = (char *)calloc(10000, sizeof(char));
		token = strtok_r(s, "=", &saveptr);
		token = strtok_r(NULL, "=", &saveptr);
		strncat(path, getenv("PATH"), strlen(getenv("PATH")));
					
		if(strstr(token, ":")!= NULL)
		{
			strncat(path, ":", strlen(":"));
			token2 = strtok_r(token, ":", &saveptr2);
			token2 = strtok_r(NULL, ":", &saveptr2);
			strncat(path, token2, strlen(token));
		}
				
		rewind(fp);
		fclose(fp);
		return path;			
						
	}
	else
	{
		token = strtok_r(s, "=", &saveptr);
		token = strtok_r(NULL, "=", &saveptr);
		str = (char *)calloc(strlen(token)+1, sizeof(char));
		strncpy(str, token, strlen(token));
	}	
		
	
	rewind(fp);
	fclose(fp);
	return str;

	
}




void findAlias(LinkedList * refList, LinkedList * aliasList, char * s)
{
	
	
	Command * cmd = command(s);
	int exists = countPlaces(refList, cmd, compareCMD);

	if(exists > 0 && aliasList->size > 0)
	{
		Node * cur = aliasList->head->next;
		int x = 1;
		for(; x < exists; x++)
		{
			cur = cur->next;
		}

		Command * cmd2 = (Command *)cur->data;
		memset(s, '\0', MAX*sizeof(char));
		strncpy(s, cmd2->data, strlen(cmd2->data));
		
		
	}
	
	cleanCommand(cmd);
}




