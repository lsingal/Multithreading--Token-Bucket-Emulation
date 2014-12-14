
#include "warmup2.h"

/*Global Variables for threads */

int token_limit = 0;
int curr_q2_size = 0;
int packet_served = 0;
int tokens_drop = 0;
int  total_tokens_prod =0;
int packets_drop = 0;
int total_packets_prod =0;
double total_i_a_time = 0;
double total_ser_time = 0;
double total_sys_time = 0;
double time_in_Q1 =0, time_in_Q2 =0;
int packet_arrived = 0;

/* Initialize the mutex lock and condition wait */
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t serverQ = PTHREAD_COND_INITIALIZER;

/* To calculate Variance */
double sys_variance = 0;


/* Enque the packet in Q1 when arrived */
void EnquePacket(Packet_desc *p){
    //   int j = i+1;
    p->Q1_time_enters = GetMTimeOfDay(); //lavish
    
    //Q1_time_enters[p->name_ID] = GetMTimeOfDay();
    fprintf(stdout, "%012.3lfms: p%d enters Q1\n",GetMTimeOfDay(), p->name_ID);
    
    My402ListAppend(&Q1_node, (void*)p);
    
}



/* Process the pakcets in Q1 and transfer to Q2 when enough tokens */
void ProcessPacket(){
    My402ListElem *find = NULL;
    Packet_desc *p = NULL;
    
    find=My402ListFirst(&Q1_node);
    
    if(find->obj == NULL){
        fprintf(stderr,"Error: Obj is NULL");
        pthread_exit(0);
    }
    
    p = (Packet_desc*)find->obj;
    
    
    if(p->tokens <= token_limit){
        // move this to Q2
        
        token_limit -= p->tokens;
        
        // total time in Q1
        
        p->total_Q1_time = GetMTimeOfDay()- p->Q1_time_enters;
        
        
        fprintf(stdout, "%012.3lfms: p%d leaves Q1, time in Q1 = %.3lfms, tokens bucket now has %d token\n",GetMTimeOfDay(),p->name_ID,p->total_Q1_time,token_limit);
        
        
        if(My402ListEmpty(&Q2_node)){
            // signal the condition about the queue not empty
            p->Q2_time_enters = GetMTimeOfDay();
            // Q2_time_enters[p->name_ID] = GetMTimeOfDay();
            fprintf(stdout, "%012.3lfms: p%d enters Q2\n",GetMTimeOfDay(),p->name_ID);
            
            My402ListAppend(&Q2_node, (void*)p);
            curr_q2_size = 1;
            /* signal the condition variable queue*/
            pthread_cond_signal(&serverQ);
            find = My402ListFind(&Q1_node, (void*)p);
            My402ListUnlink(&Q1_node, find);
            
        }else{
            p->Q2_time_enters = GetMTimeOfDay();
            
            //   Q2_time_enters[p->name_ID] = GetMTimeOfDay();
            
            fprintf(stdout, "%012.3lfms: p%d enters Q2\n",GetMTimeOfDay(),p->name_ID);
            
            My402ListAppend(&Q2_node, (void*)p);
            /* gaurd set true */
            curr_q2_size = 1;
            find = My402ListFind(&Q1_node, (void*)p);
            My402ListUnlink(&Q1_node, find);
            
        }
        
    }
}

/* Arrival thread */

void *Arrival(void* arg){
    
    
    int i =0;
    double prev_itime = 0 ;
    double inter_a_time = 0.0;
    Packet_desc *p = NULL;
    
    //Init the list
    My402ListInit(&Q1_node);
    
    
    //create packet
    while(i != num) {
        
        p = (Packet_desc*)malloc(sizeof(Packet_desc));
        
        if(fp != NULL){
            ReadFile(p);
        }
        else{
            p->int_time = 1/lamda*THOUSAND ; // interarrival time
            p->tokens = P;
            p->ser_time = 1/mu*THOUSAND; // service time
        }
        p->name_ID = ++i;
        
        
        usleep(p->int_time*THOUSAND);
        
        inter_a_time = GetMTimeOfDay()- prev_itime;
        
        prev_itime = GetMTimeOfDay() ;
        
        total_i_a_time += inter_a_time;
        
        pthread_mutex_lock(&lock);
        
        if(p->tokens > B){
            
            fprintf(stdout, "%012.3lfms: p%d arrives, needs %d tokens, inter-arrival time = %.3fms, dropped\n",GetMTimeOfDay(),p->name_ID,p->tokens, inter_a_time);
            packets_drop++;
            total_packets_prod++;
            free(p);
            
            pthread_mutex_unlock(&lock);
            
            continue;
        }
        
        fprintf(stdout, "%012.3lfms: p%d arrives, needs %d tokens, inter-arrival time = %.3fms\n",GetMTimeOfDay() ,p->name_ID,p->tokens,inter_a_time);
        
        total_packets_prod++;
        
        pthread_mutex_unlock(&lock);
        
        pthread_mutex_lock(&lock);
        
        packet_arrived++;
        
        //critical section
        
        EnquePacket(p);
        
        ProcessPacket();
        
        pthread_mutex_unlock(&lock);
    }
    
    if(filename){
        fclose(fp);
    }
    
    return 0;
}

/* Token bucket thread */

void *Token(void* arg){
    int j=0;
    useconds_t wait_time = 0;
    
    if(r < 0.1)
        r = 0.1;
    
    wait_time = (1/r)*SECTOMSEC;
    
    
    while( 1 ) {
        
        pthread_mutex_lock(&lock);
        
        if( total_packets_prod == num && My402ListEmpty(&Q1_node)){
            curr_q2_size = 1;
            pthread_cond_signal(&serverQ);
            pthread_mutex_unlock(&lock);
            break;
        }
        
        pthread_mutex_unlock(&lock);
        
        usleep(wait_time);
        
        pthread_mutex_lock(&lock);
        
        if( total_packets_prod == num && My402ListEmpty(&Q1_node)){
            pthread_mutex_unlock(&lock);
            continue;
        }
        pthread_mutex_unlock(&lock);
        
        pthread_mutex_lock(&lock);
        if(token_limit < B){
            j++;
            total_tokens_prod = j;
            token_limit++;
            fprintf(stdout, "%012.3lfms: token t%d arrives, token bucket now has %d token\n",GetMTimeOfDay(),j,token_limit);
        }else{
            j++;
            total_tokens_prod = j;
            tokens_drop++;
            fprintf(stdout, "%012.3lfms: token t%d arrives, dropped\n",GetMTimeOfDay(), j );
        }
        //check if it can move first packet from Q1 to Q2
        if(!My402ListEmpty(&Q1_node)){
            ProcessPacket();
            
        }
        pthread_mutex_unlock(&lock);
        
    }
    
    return 0;
}



void DequePacket(Packet_desc *p,My402ListElem *first_elem){
    
    p->total_Q2_time = GetMTimeOfDay()- p->Q2_time_enters;
    
    fprintf(stdout, "%012.3lfms: p%d leaves Q2, time in Q2 = %.3lfms, begin service at S\n",GetMTimeOfDay(), p->name_ID,p->total_Q2_time);
    
    My402ListUnlink(&Q2_node, first_elem);
    
    if(My402ListEmpty(&Q2_node))
        curr_q2_size = 0;
    
}

void *Service(void* arg){
    My402ListInit(&Q2_node);
    double time_in_system = 0.0;
    
    while(1){
        My402ListElem *first_elem = NULL ;
        Packet_desc *p = NULL;
        
        
        if(total_packets_prod == num && My402ListEmpty(&Q2_node) && My402ListEmpty(&Q1_node))
            break;
        
        
        pthread_mutex_lock(&lock);
        
        while(curr_q2_size == 0 && !interrupt_called){
            pthread_cond_wait(&serverQ, &lock);
        }
        if((total_packets_prod == num && My402ListEmpty(&Q2_node) && My402ListEmpty(&Q1_node)) || interrupt_called){
            pthread_exit(0);
        }
        
        first_elem = My402ListFirst(&Q2_node);
        
        if(first_elem->obj == NULL){
            fprintf(stderr,"Error: Obj is NULL");
            pthread_exit(0);
        }
        
        p = (Packet_desc*)first_elem->obj;
        
        
        DequePacket(p,first_elem);
        p->S_time_enters = GetMTimeOfDay();
        pthread_mutex_unlock(&lock);
        
        
        usleep(p->ser_time*THOUSAND);
        
        pthread_mutex_lock(&lock);
        
        packet_served++;
        
        p->total_S_time = GetMTimeOfDay()- p->S_time_enters;
        time_in_system = GetMTimeOfDay()- p->Q1_time_enters;
        sys_variance += time_in_system * time_in_system ;
        
        /* Calculate total time for the packets served */
        total_ser_time += p->total_S_time;
        total_sys_time += time_in_system;
        time_in_Q1 += p->total_Q1_time;
        time_in_Q2 += p->total_Q2_time;
        
        
        fprintf(stdout, "%012.3lfms: p%d departs from S, service time = %.3fms, time in system = %.3fms\n",GetMTimeOfDay(), p->name_ID,p->total_S_time,time_in_system);
        
        free(p);
        
        pthread_mutex_unlock(&lock);
        
    }
    
    return 0;
}

void ReadFile(Packet_desc *p){
    
    fscanf(fp,"%f %d %f",&p->int_time,&p->tokens,&p->ser_time);
    
}


void CreateThreads(){
    
    int errno = 0,i=0;
    if((errno = pthread_create(&thrd[0], 0,Arrival, (void*)0 ))){
        fprintf(stderr, "pthread_create[0] %s\n",strerror(errno));
        pthread_exit(0);
    }
    if((errno = pthread_create(&thrd[1], 0,Token, (void*)1 ))){
        fprintf(stderr, "pthread_create[1] %s\n",strerror(errno));
        pthread_exit(0);
    }
    if((errno = pthread_create(&thrd[2], 0,Service, (void*)2 ))){
        fprintf(stderr, "pthread_create[2] %s\n",strerror(errno));
        pthread_exit(0);
    }
    
    for (i=0; i<N; i++) {
        if((errno = pthread_join(thrd[i], 0))){
            fprintf(stderr, "pthread_join[i] %s\n",strerror(errno));
            pthread_exit(0);
            
        }
        
    }
    
}




