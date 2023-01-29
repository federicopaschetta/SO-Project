	#define _GNU_SOURCE 
	#include <sys/msg.h>
	#define SO_BLOCK_SIZE 5
	#define SO_REGISTRY_SIZE 500
	#include <stdio.h>
	#include <signal.h>
	#include <stdlib.h>
	#include <string.h>
	#include <sys/ipc.h>
	#include <sys/sem.h>
	#include <sys/msg.h>
	#include <sys/shm.h>
	#include <unistd.h>
	#include <errno.h>
	#include <time.h>
	#include <sys/time.h>
	#include <sys/uio.h>
	#include <math.h>
	#include <malloc.h>
	#include <fcntl.h>
	#define NODE_SENDER -1

	#define MSG_SIZE (sizeof(struct Transazione))

	#define TEST_ERROR    if (errno) {dprintf(STDERR_FILENO,		\
		"%s:%d: PID=%5d: Error %d (%s)\n", \
		__FILE__,			\
		__LINE__,			\
		getpid(),			\
		errno,			\
		strerror(errno));}

	struct Transazione{
		int timestamp;
		pid_t sender;
		pid_t receiver;
		int quantita;
		int reward;
		int hops;
	}; 


	struct LibroMastro{
		int posizione;
		struct Transazione t[SO_REGISTRY_SIZE][SO_BLOCK_SIZE];
	};

	struct mpid{
			int numeronodi;    
			pid_t PID_USERS[10000];
			pid_t  PID_NODES[10000];
			int BUDGET_USERS[10000];
			int BUDGET_NODES[10000];
			int TP_REMAINING[10000];
			int DEAD_USERS[10000];
			pid_t FRIENDS[10000][10000];

		};

