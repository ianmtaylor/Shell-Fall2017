CC=gcc

mssh:	cscd340Lab6.c ./tokenize/makeArgs.o ./utils/myUtils.o ./process/process.o ./pipes/pipes.o ./msshrcHandler/rcHandler.o ./linkedlist/linkedList.o ./linkedlist/listUtils.o ./history/history.o ./command/command.o ./redirect/redirect.o
	gcc -g cscd340Lab6.c ./tokenize/makeArgs.o ./utils/myUtils.o ./process/process.o ./pipes/pipes.o ./msshrcHandler/rcHandler.o ./linkedlist/linkedList.o ./linkedlist/listUtils.o ./history/history.o ./command/command.o ./redirect/redirect.o -o mssh

makeArgs.o:	./tokenize/makeArgs.c ./tokenize/makeArgs.h
	gcc -g -c ./tokenize/makeArgs.c

myUtils.o:	./utils/myUtils.o ./utils/myUtils.h
	gcc -g -c ./utils/myUtils.c

process.o:	./process/process.c ./process/process.h
	gcc -g -c ./process/process.c

pipes.o:	./pipes/pipes.c ./pipes/pipes.h
	gcc -g -c ./pipes/pipes.c

rcHandler.o:	./msshrcHandler/rcHandler.c ./msshrcHandler/rcHandler.h
	gcc -g -c ./msshrcHandler/rcHandler.c

linkedList.o:	./linkedlist/linkedList.c ./linkedlist/linkedList.h
	gcc -g -c ./linkedlist/linkedList.c

listUtils.o:	./linkedlist/listUtils.c ./linkedlist/listUtils.h
	gcc -g -c ./linkedlist/linkedList.c

history.o:	./history/history.c ./history/history.h
	gcc -g -c ./history/history.c

command.o:	./command/command.c ./command/command.h
	gcc -g -c ./command/command.c

redirect.o:	./redirect/redirect.c ./redirect/redirect.h
	gcc -g -c ./redirect/redirect.c

clean:
	rm ./pipes/pipes.o
	rm ./utils/myUtils.o
	rm ./process/process.o
	rm ./tokenize/makeArgs.o
	rm ./msshrcHandler/rcHandler.o
	rm ./linkedlist/linkedList.o
	rm ./linkedlist/listUtils.o
	rm ./history/history.o
	rm ./command/command.o
	rm ./redirect/redirect.o
	rm mssh

