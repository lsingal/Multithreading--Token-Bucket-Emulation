
#include "warmup2.h"

/* will be true when signal interrupt is called */
int interrupt_called =0;
/* Structure object for Q1/Q2 packets*/
My402List Q1_node;
My402List Q2_node;


/* Remove all the packets when signal is generated */

void RemoveAllPackets(My402List *queue , int queue_no){
    My402ListElem *elem = My402ListFirst(queue);
    Packet_desc *p = NULL;
    
    while (elem != NULL) {
        p = (Packet_desc*)elem->obj;
        
        fprintf(stdout, "%012.3lfms: p%d removed from Q%d\n",GetMTimeOfDay(),p->name_ID,queue_no);
        
        My402ListUnlink(queue,elem);
        elem = My402ListFirst(queue);
        
        if(queue_no ==  1)
            packet_arrived--;
        
        /* since packets are dropped increase counter */
        packets_drop++;
        
        free(p);
        
    }
    
}

/* Function called when SIGINT is evoked */

void handler(int signo) {
    int errno=0;
    interrupt_called = 1;
    
    fprintf(stdout,"\n");
    
    if((errno = pthread_cancel(thrd[0]))){
        fprintf(stderr, "pthread_cancel[0] %s\n",strerror(errno));
        
    }
    if((errno = pthread_cancel(thrd[1]))){
        fprintf(stderr, "pthread_cancel[0] %s\n",strerror(errno));
        
    }
    
    RemoveAllPackets(&Q1_node,1);
    RemoveAllPackets(&Q2_node,2);
    
    pthread_cond_signal(&serverQ);
    
    
}

