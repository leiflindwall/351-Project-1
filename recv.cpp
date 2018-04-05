#include <sys/shm.h>
#include <sys/msg.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "msg.h"    /* For the message struct */


/* The size of the shared memory chunk */
#define SHARED_MEMORY_CHUNK_SIZE 1000

/* The ids for the shared memory segment and the message queue */
int shmid, msqid;

// the key_t
key_t key;

// the flag needed for the queue
int shmflg= IPC_CREAT | 0666;

/* The pointer to the shared memory */
void *sharedMemPtr;

/* The name of the received file */
const char recvFileName[] = "recvfile";


/**
 * Sets up the shared memory segment and message queue
 * @param shmid - the id of the allocated shared memory
 * @param msqid - the id of the shared memory
 * @param sharedMemPtr - the pointer to the shared memory
 */
void init(int& shmid, int& msqid, void*& sharedMemPtr)
{

	/* TODO: 1. Create a file called keyfile.txt containing string "Hello world" (you may do
 		    so manually or from the code).
	         2. Use ftok("keyfile.txt", 'a') in order to generate the key.
		 3. Use the key in the TODO's below. Use the same key for the queue
		    and the shared memory segment. This also serves to illustrate the difference
		    between the key and the id used in message queues and shared memory. The id
		    for any System V object (i.e. message queues, shared memory, and sempahores)
		    is unique system-wide among all System V objects. Two objects, on the other hand,
		    may have the same key.
	 */

	 /* TODO: Allocate a piece of shared memory. The size of the segment must be SHARED_MEMORY_CHUNK_SIZE. */

 	 key = ftok("keyfile.txt", 'a');

 	 // try to access shared memory
 	 if ((shmid = shmget (key, SHARED_MEMORY_CHUNK_SIZE, shmflg)) < 0)
 	 {
 		 perror("shmget: shmget failed");
 		 exit(1);
 	 }
 	 else
 	 {
 		 (void) fprintf(stderr, "shmget: shmget returned %d\n", shmid);
 	 }


	/* TODO: Attach to the shared memory */
	 if((sharedMemPtr = shmat(shmid, NULL, 0)) == (char *) - 1)
	 {
		 perror("shmat");
		 exit(1);
	 }

	/* TODO: Create a message queue */
	/* Store the IDs and the pointer to the shared memory region in the corresponding parameters */
	msqid = msgget(key, shmflg);
}


/**
 * The main loop
 */
void mainLoop()
{
	/* The size of the mesage */
	int msgSize = 0;

	/* Open the file for writing */
	FILE* fp = fopen(recvFileName, "w");

	/* Error checks */
	if(!fp)
	{
		perror("fopen");
		exit(-1);
	}

    /* TODO: Receive the message and get the message size. The message will
     * contain regular information. The message will be of SENDER_DATA_TYPE
     * (the macro SENDER_DATA_TYPE is defined in msg.h).  If the size field
     * of the message is not 0, then we copy that many bytes from the shared
     * memory region to the file. Otherwise, if 0, then we close the file and
     * exit.
     *
     * NOTE: the received file will always be saved into the file called
     * "recvfile"
     */

	/* Keep receiving until the sender set the size to 0, indicating that
 	 * there is no more data to send
 	 */

   /* A buffer to store message we will send to the sender. */
 	message sndMsg;

 	/* A buffer to store message received from the sender. */
 	message rcvMsg;

  msgSize = 1;

	while(msgSize != 0)
	{

    printf("receiving message...\n");
    // Error checking system call
    if(msgrcv(msqid, &rcvMsg, sizeof(rcvMsg) - sizeof(long), SENDER_DATA_TYPE, 0) == -1)
    {
      perror("msgrcv");
      exit(1);
    }

    msgSize = rcvMsg.size;

		/* If the sender is not telling us that we are done, then get to work */
		if(msgSize != 0)
		{
			/* Save the shared memory to file */
			if(fwrite(sharedMemPtr, sizeof(char), msgSize, fp) < 0)
			{
				perror("fwrite");
			}
      printf("memory written to file.\n");

			/* TODO: Tell the sender that we are ready for the next file chunk.
 			 * I.e. send a message of type RECV_DONE_TYPE (the value of size field
 			 * does not matter in this case).
 			 */

       sndMsg.mtype = RECV_DONE_TYPE;
       sndMsg.size = 0;
       printf("telling sender we are ready for the next chunk...\n");
       // Error checking system call
       if(msgsnd(msqid, &sndMsg, 0, 0) == -1)
       {
         perror("msgsnd");
       }
       printf("message received by sender.\n");
		}
		/* We are done */
		else
		{
			/* Close the file */
			fclose(fp);
		}
	}
}

/**
 * Perfoms the cleanup functions
 * @param sharedMemPtr - the pointer to the shared memory
 * @param shmid - the id of the shared memory segment
 * @param msqid - the id of the message queue
 */

void cleanUp(const int& shmid, const int& msqid, void* sharedMemPtr)
{
  printf("Cleaing up:\n");
  printf("Detaching from shared memory...\n");
	/* TODO: Detach from shared memory */
  shmdt(sharedMemPtr);

  printf("Deallocating shared memory chunk...\n");
	/* TODO: Deallocate the shared memory chunk */
  shmctl(shmid, IPC_RMID, NULL);

  printf("Deallocating message queue...\n");
	/* TODO: Deallocate the message queue */
  msgctl(msqid, IPC_RMID, NULL);

  printf("All done!\n");
}

/**
 * Handles the exit signal
 * @param signal - the signal type
 */
void ctrlCSignal(int signal)
{
	/* Free system V resources */
  printf("\n");
	cleanUp(shmid, msqid, sharedMemPtr);
}

int main(int argc, char** argv)
{

	/* TODO: Install a singnal handler (see signaldemo.cpp sample file).
 	 * In a case user presses Ctrl-c your program should delete message
 	 * queues and shared memory before exiting. You may add the cleaning functionality
 	 * in ctrlCSignal().
 	 */
  signal(SIGINT, ctrlCSignal);

	/* Initialize */
	init(shmid, msqid, sharedMemPtr);

	/* Go to the main loop */
	mainLoop();

	/** TODO: Detach from shared memory segment, and deallocate shared memory and message queue (i.e. call cleanup) **/
  /* Cleanup */
	cleanUp(shmid, msqid, sharedMemPtr);

	return 0;
}
