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
    void *shared_memory = (void *)0; // var that represents the shared memory segment
    struct shared_use_st *shared_stuff; // c shared memory segment struct
    struct utsname info; // c system name struct
    char buffer[BUFSIZ]; // buffer where shared data is temporarily stored
    char infoPackage[256];
    int shmid; // var that will store id of shared memory segment

    char hostbuffer[256];
    int hostname; // var storing the id number of the host
    errno = 0;

    /* 
     * @name shmid 
     * @param (key_t)1234 - assigns key, key_t 1234, to the returned shared memory segment
     * @param sizeof(sturct shared_use_st) - size of the returned shared memory segment
     * @param 0666 | IPC_CREAT - specifies that memory segment will be used as a server
    */
    shmid = shmget((key_t)1234, sizeof(struct shared_use_st), 0666 | IPC_CREAT);

    // shmid error handling
    if (shmid == -1) {
        fprintf(stderr, "shmget failed\n");
        exit(EXIT_FAILURE);
    }
    
    if(uname(&info)!=0)
    {
       perror("uname doesn't return 0, so there is an error");
       exit(EXIT_FAILURE);
    }

    // shmat - attaches shared memory segment
    shared_memory = shmat(shmid, (void *)0, 0);
    
    // shmat error handling
    if (shared_memory == (void *)-1) {
        fprintf(stderr, "shmat failed\n");
        exit(EXIT_FAILURE);
    }
    
    // print to write program that memory has been attached
    printf("Memory attached at %X\n", (int)shared_memory);

    // init hostname via gethostname()
    hostname = gethostname(hostbuffer, sizeof(hostbuffer));

    // assign host values to the hostbuffer
    strcpy(infoPackage, "Hostname = "); // computer host name
    strcat(infoPackage, hostbuffer);
    strcat(infoPackage, "\nSystem Name = "); // computer host system name
    strcat(infoPackage, info.sysname);
    strcat(infoPackage, "\nNode Name = "); // computer host node name
    strcat(infoPackage, info.nodename);
    strcat(infoPackage, "\nVersion = "); // computer host version number
    strcat(infoPackage, info.version);
    strcat(infoPackage, "\n");

    // assign shared_stuff memory segment struct to the shared memory
    shared_stuff = (struct shared_use_st *)shared_memory;
    while(running) {
        
        while(shared_stuff->written_by_you == 1) {
            sleep(1);            
            printf("waiting for client...\n");
        }
        
        // if writen_by_you = 2 then the session should be stopped
        if(shared_stuff->written_by_you == 2) {
            printf("IPC Session has ended.\n");
            running = 0;
        }
        // copy system info into the buffer
        strncpy(shared_stuff->some_text, infoPackage, TEXT_SZ);
        shared_stuff->written_by_you = 1; // assign to 1 bc system info has been assigned
        
    }
    
    // assigning shared memory error handling
    if (shmdt(shared_memory) == -1) {
        fprintf(stderr, "shmdt failed\n");
        exit(EXIT_FAILURE);
    }
    exit(EXIT_SUCCESS);
}
