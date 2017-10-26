
#include "linkedList.h"



LinkedList * linkedList()
{
	LinkedList * myList = (LinkedList*)calloc(1, sizeof(LinkedList));
	myList->head = (Node*)calloc(1, sizeof(Node));
	myList->size = 0;
	return myList;
}



void addLast(LinkedList * theList, Node * nn)
{
	if(theList == NULL || nn == NULL)
		exit(-99);

	if(theList->size > 0)
	{
		Node * cur = theList->head->next;
		while(cur->next != NULL)
			cur = cur->next;
		cur->next = nn;
		nn->prev = cur;
		nn->next = NULL;
		theList->size++;
	}
	else
	{
		addFirst(theList, nn);
	}
}



void addFirst(LinkedList * theList, Node * nn)
{
	if(theList == NULL || nn == NULL)
		exit(-99);

	if(theList->size > 0)
		theList->head->next->prev = nn;
	nn->next = theList->head->next;
	nn->prev = theList->head;
	theList->head->next = nn;
	theList->size++;


}



void removeFirst(LinkedList * theList, void (*removeData)(void *))
{
	if(theList == NULL)
		exit(-99);
	Node * cur = theList->head->next;
	if(theList->head->next->next != NULL)
	{
        	theList->head->next = cur->next;
		cur->next->prev = theList->head;
	}
	else
	{
		theList->head->next = NULL;
	}
	removeData(cur->data);
	cur->data = NULL;
	free(cur);
	cur = NULL;
	theList->size--;

}

void * getFirst(LinkedList * theList)
{
        if(theList == NULL)
                exit(-99);
	Node * nn = NULL;
	Node * cur = NULL;
	if(theList->size > 0)
	{
		
		cur = theList->head->next;
		
		if(cur->next != NULL)
		{
			
			theList->head->next = cur->next;
			cur->next->prev = theList->head;
		}
		else
		{
			
			theList->head->next = NULL;
		}
		nn = cur;
		theList->size--;
	}
	
	return cur;

}

char * lookatLast(LinkedList * theList)
{
	if(theList == NULL)
	{
		printf("\n\nERROR: LIST IS NULL...Restart shell\n\n");
		exit(-99);
	}
	if(theList->size > 0)
	{
		Node * cur = theList->head->next;
		while(cur->next != NULL)
			cur = cur->next;

		return getDataCMD(cur->data);
	}
	else
	{
		return NULL;
	}
}

char * getCMDBang(LinkedList * theList, int pos)
{
	if(pos < 1 || pos > theList->size)
		return NULL;

	if(theList->size > 0)
	{
		Node * cur = theList->head->next;
		int x = 1;
		for(; x < pos; x++)
		{
			cur = cur->next;
		}
		return getDataCMD(cur->data);
	}
	else
		return NULL;
}


void removeLast(LinkedList * theList, void (*removeData)(void *))
{
	if(theList == NULL)
	{
		printf("\n\nERROR: LIST IS NULL...Restart shell\n\n");
		exit(-99);
	}
	if(theList->size > 0)
	{
		Node * cur = theList->head->next;
		while(cur->next != NULL)
			cur = cur->next;

		cur->prev->next = NULL;
		removeData(cur->data);
		cur->data = NULL;
		free(cur);
		cur = NULL;
		theList->size--;
	}
}



void removeItem(LinkedList * theList, Node * nn, void (*removeData)(void *), int (*compare)(const void *, const void *))
{

	if(theList == NULL || nn == NULL)
		exit(-99);

	if(theList->size > 0)
	{

		Node * cur = theList->head->next;

		int found = 0;
		while(cur != NULL && compare(cur->data, nn->data) != 0)
			cur = cur->next;

		if(cur != NULL && compare(cur->data, nn->data) == 0)
			found = 1;


		if(found == 1 && cur->next == NULL)
			removeLast(theList, removeData);
		else if(found == 1)
		{
			cur->next->prev = cur->prev;
			cur->prev->next = cur->next;
			removeData(cur->data);
			free(cur);
			cur = NULL;
		}
		theList->size--;

	}
	removeData(nn->data);
	free(nn);
	nn = NULL;
}


void removeAliasItem(LinkedList * theList, int pos, void (*removeData)(void *))
{
	if(theList == NULL)
		exit(-99);

	if(theList->size > 0)
	{

		Node * cur = theList->head->next;

		int found = 0;
		int x;
		for(x = 0; x < pos-1; x++)
			cur = cur->next;

		if(cur != NULL)
			found = 1;


		if(found == 1 && cur->next == NULL)
			removeLast(theList, removeData);
		else if(found == 1)
		{
			cur->next->prev = cur->prev;
			cur->prev->next = cur->next;
			removeData(cur->data);
			free(cur);
			cur = NULL;
		}
		theList->size--;

	}
}



void clearList(LinkedList * theList, void (*removeData)(void *))
{
	
	if(theList->size > 0)
	{
		Node * cur = theList->head->next;
		Node * temp;
		while(cur != NULL)
		{
			temp = cur;
			cur = cur->next;
			removeData(temp->data);
			temp->data = NULL;
			free(temp);
			temp = NULL;
		}
		
	}

	free(theList->head);
	theList->head = NULL;
	theList ->size = 0;

}



void printList(const LinkedList * theList, void (*convertData)(void *))
{
	
	if(theList->size > 0)
	{
  		Node * cur = theList->head->next;
		while(cur != NULL)
		{
			convertData(cur->data);
			printf("\n");
			cur = cur->next;
		}
	}
	else
	{

		printf("\nList is Empty\n");
	}

}

void printCMDList(const LinkedList * theList, void (*convertData)(void *))
{
	printf("\n*****HISTORY FOR CURRENT SESSION*****\n");
	if(theList->size > 0)
	{
  		Node * cur = theList->head->next;
		int x = 1;
		while(cur != NULL)
		{
			printf("%d. ", x);
			convertData(cur->data);
			printf("\n");
			cur = cur->next;
			x++;
		}
	}
	else
	{

		printf("\nNo recent commands for this session\n");
	}
	printf("*************************************\n");

}

void writeList(const LinkedList * theList, char * filename, void (*printData)(void *, FILE *))
{
	FILE * fp = fopen(filename, "w");

	if(theList->size > 0)
	{
  		Node * cur = theList->head->next;
		while(cur != NULL)
		{
			printData(cur->data, fp);
			fprintf(fp, "\n");
			cur = cur->next;
		}
	}
	rewind(fp);
	fclose(fp);
	
}

int countPlaces(const LinkedList * theList, Command * cmd, int (*compare)(const void *, const void *))
{
	
	int x= 0;
	if(theList->size > 0)
	{
		Node * cur = theList->head->next;
		while(cur != NULL)
		{
			x++;
			if(compare((void *)cmd, cur->data) == 0)
				return x;
			cur = cur->next;
		}
	}

	return -1;
		
}







