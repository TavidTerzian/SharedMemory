/* The second program is the producer and allows us to enter data for consumers.
 It's very similar to shm1.c and looks like this. */

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <sys/shm.h>
#include <sys/utsname.h>
#include <errno.h>

#include "shm_com.h"

int main()
{
    int running = 1;
    void *shared_memory = (void *)0;
    struct shared_use_st *shared_stuff;
    struct utsname info;
    char buffer[BUFSIZ];
    char infoPackage[256];
    int shmid;

    char hostbuffer[256];
    int hostname;
    errno = 0;

    shmid = shmget((key_t)1234, sizeof(struct shared_use_st), 0666 | IPC_CREAT);

    
    if(uname(&info)!=0)
    {
       perror("uname doesn't return 0, so there is an error");
       exit(EXIT_FAILURE);
    }

    if (shmid == -1) {
        fprintf(stderr, "shmget failed\n");
        exit(EXIT_FAILURE);
    }

    shared_memory = shmat(shmid, (void *)0, 0);
    if (shared_memory == (void *)-1) {
        fprintf(stderr, "shmat failed\n");
        exit(EXIT_FAILURE);
    }

    printf("Memory attached at %X\n", (int)shared_memory);

    hostname = gethostname(hostbuffer, sizeof(hostbuffer));

    strcpy(infoPackage, "Hostname = ");
    strcat(infoPackage, hostbuffer);
    strcat(infoPackage, "\nSystem Name = ");
    strcat(infoPackage, info.sysname);
    strcat(infoPackage, "\nNode Name = ");
    strcat(infoPackage, info.nodename);
    strcat(infoPackage, "\nVersion = ");
    strcat(infoPackage, info.version);
    strcat(infoPackage, "\n");

    shared_stuff = (struct shared_use_st *)shared_memory;
    while(running) {
        while(shared_stuff->written_by_you == 1) {
            sleep(1);            
            printf("waiting for client...\n");
        }

        strncpy(shared_stuff->some_text, infoPackage, TEXT_SZ);
        shared_stuff->written_by_you = 1;

        if (strncmp(buffer, "end", 3) == 0) {
                running = 0;
        }
    }

    if (shmdt(shared_memory) == -1) {
        fprintf(stderr, "shmdt failed\n");
        exit(EXIT_FAILURE);
    }
    exit(EXIT_SUCCESS);
}
