# CENG 231 Exercise 4 - SharedMemory

## Collaboration project between Tavid Terzian and Tarek El-Hajjaoui.

## Description of Program:
  -This is a C program that creates an IPC session between 2 processes (producer and consumer).
  - The producer process creates the shared memory messages that consists of the other processe's computer system info (host name, system name, node name, release, and version)
  - The producer writes this info out to the console of the consumer process
  - After the system info is printed, both of the processes are terminated and the IPC session is closed.
  
## Instructions to run the program:
- Compile the producer: gcc shmwrite.c -o write
- Compile the consumer: gcc shmread.c - o read
- Start the producer process via this command: ./write
- Start the consumer process via this command: ./read
- Note: The order of starting which process first does not matter
- View the console outputs of both processes
  
## Sources:
  - srand: https://www.tutorialspoint.com/rand-and-srand-in-c
  - sys/utsname: https://pubs.opengroup.org/onlinepubs/7908799/xsh/sysutsname.h.html
  - shmget: https://www.csl.mtu.edu/cs4411.ck/www/NOTES/process/shm/shmget.html
  - shmop: https://man7.org/linux/man-pages/man2/shmop.2.html
  - shmget: https://pubs.opengroup.org/onlinepubs/007904875/functions/shmget.html
  - strcmp: http://www.cplusplus.com/reference/cstring/strncmp/
  - Linux Man page: https://linux.die.net/man/3/syslog  
  -GNU: https://www.gnu.org/software/libc/manual/html_node/Syslog-Example.html  
