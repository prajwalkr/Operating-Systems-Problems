// type '/' in the server code to exit the program properly without force closing
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXSIZE     50
void die(char *s)
{
	// process dies with character sequence as error
	perror(s);
	exit(1);
}
int main()
{
	int shmid;
	key_t key;
	char *shm;

	key = 5682;									// same id as in server process

	if ((shmid = shmget(key, MAXSIZE, 0666)) < 0)
		die("shmget");
	if ((shm = shmat(shmid, NULL, 0)) == (char *) -1)
		die("shmat");

	while(1){
		printf("Server Process: ");
		puts(shm+1);
		if(shm[1]=='/')
			break;								// prints the message in shared memory
		printf("Client Process : ");
		char a[50];
		int i;
		scanf("%s",a);
		strcpy(shm+1,a);
		*shm = '*';
		/*
		* puts '*' in the beginning indicating that
		* the server process has written some text
		*/									// exiting the client process
		while (*shm != '#')
			sleep(1);

		/*
		* Wait until the other process
		* changes the first character of our memory
		* to '#', indicating that it has read what
		* we put there.
		*/
	}
	shmdt(shm);
	exit(0);
}
