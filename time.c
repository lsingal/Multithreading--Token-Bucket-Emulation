
#include "warmup2.h"


double total_ms = 0;


void SaveCurTimeOfDay(){
    int errno = -1;
    double s=0.0;
    float micro_s =0;
    struct timeval mytime;
    if((errno = gettimeofday(&mytime, NULL))){
        fprintf(stderr, "PrintTimeOfDay %s",strerror(errno));
    }
    
    micro_s = (float)mytime.tv_usec/THOUSAND;
    s=(double)mytime.tv_sec*THOUSAND;
    
    total_ms = s + micro_s;
    
}


/* Get current time */
double GetMTimeOfDay(){
    int errno = -1;
    double cur_time =0.0;
    double s=0.0;
    double micro_s =0;
    
    struct timeval mytime;
    
    if((errno = gettimeofday(&mytime, NULL))){
        fprintf(stderr, "PrintTimeOfDay %s",strerror(errno));
    }
    
    micro_s = (double)mytime.tv_usec/THOUSAND;
    s=(double)mytime.tv_sec*THOUSAND;
    
    cur_time = s + micro_s - total_ms;
    
    return cur_time;
    
}

