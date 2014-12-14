#include <stdio.h>
#include "pthread.h"
#include "my402list.h"
#include "unistd.h"
#include <sys/time.h>
#include "stdlib.h"
#include <string.h>
#include "signal.h"
#include <math.h>

#define SECTOMSEC 1000000 // second to microsecond
#define THOUSAND 1000
#define N 3 // # of threads


/* Default parameters */
/* Interarrival time */
extern float lamda;
/* Service rate */
extern float mu ;
/* Token filling rate */
extern float r ;
/* Maximum token bucket size */
extern int B ;
/* Packet required token size */
extern int P ;
/* # of packers to serve */
extern int num ;
/* file address */
extern const char *filename;//"/Users/lavishsingal/Project_C/Token Bucket/Token Bucket/tsfile.txt";

/* track of tokens currently in bucket */
extern int token_limit ;
/* To calculate total time in millisecond */
extern double total_ms ;
/* Q2 Size gaurd */
extern int curr_q2_size ;
/* # of packets serverd */
extern int packet_served ;
/* total tokens dropped */
extern int tokens_drop ;
/* Total tokens Produced */
extern int  total_tokens_prod;
/* Total packets droppped */
extern int packets_drop ;
/* total packets produced */
extern int total_packets_prod ;
/* total interarrival time */
extern double total_i_a_time ;
/*total service time */
extern double total_ser_time ;
/* total system time for application to completer*/
extern double total_sys_time ;
/* time when emulation is ended */
extern double emulation_ends ;
/* Total time packers spent in Q1/Q1 */
extern double time_in_Q1 ;
extern double time_in_Q2 ;
/* variace for calculating Standard Deviation */
extern double sys_variance ;
/* when user press cntrl+C */
extern int interrupt_called;
/* # of packets arrived */
extern int packet_arrived ;
/*File pointer */
extern FILE *fp ;


/* Thread ID array */
extern pthread_t thrd[N];

/* Initialization */
extern My402List Q1_node;
extern My402List Q2_node;
extern pthread_mutex_t lock ;
extern pthread_cond_t serverQ ;

/* structure for pointer */
typedef struct{
    float int_time ; /* arrival time */
    int tokens;     /* tokens required */
    float ser_time; /* service time */
    int name_ID;    /* Id */
    double Q1_time_enters;
    double total_Q1_time;
    double Q2_time_enters;
    double total_Q2_time;
    double S_time_enters;
    double total_S_time;
}Packet_desc;

/* Printing functions */
void PrintParameters();
void PrintStatistics();

/*Time Utility functions*/
void SaveCurTimeOfDay();
double GetMTimeOfDay();

/* Read from file */
void ReadFile(Packet_desc *p);

/* Processing fuctions */
void EnquePacket(Packet_desc *p);
void ProcessPacket();
void StartThreads();
void DequePacket(Packet_desc *p,My402ListElem *first_elem);

/*Thread functions */
void *Arrival(void*);
void *Token(void*);
void *Service(void*);
void CreateThreads();

/*Remove packerts from Q1 and Q1 after interrupt is called */
void RemoveAllPackets(My402List *queue , int queue_no);

/* Signal handler */
void handler(int signo);

/*Check validity of command line arguments */
int checkValidity(const char *str, int *ifnegative);

/* Read input from command line arguments */
void ReadCommandArguments(int argc, const char *argv[]);

