
#include "warmup2.h"

/* Print the parameters which are used for application */

void PrintParameters(){
    
    
    fprintf(stdout, "\nEmulation Parameters:\n");
    
    if(fp == NULL){
        fprintf(stdout, "\tnumber to arrive = %d\n",num);
        
        fprintf(stdout, "\tlambda = %.2f\n",lamda);
        
        fprintf(stdout, "\tmu = %.2f\n",mu);
        fprintf(stdout, "\tr = %.2f\n",r);
        fprintf(stdout, "\tB = %d\n",B);
        fprintf(stdout, "\tP = %d\n",P);
        fprintf(stdout, "\n");
    }else{
        fprintf(stdout, "\tnumber to arrive = %d\n",num);
        fprintf(stdout, "\tr = %.2f\n",r);
        fprintf(stdout, "\tB = %d\n",B);
        
        fprintf(stdout, "\ttsfile = %s\n\n",filename);
    }
    
}

/* Print final statistics */

void PrintStatistics(){
    float token_prob = 0.0, packet_prob=0.0;
    float stand_dev =0;
    
    
    fprintf(stdout, "\nStatistics:\n");
    
    if(total_packets_prod == 0){
        fprintf(stdout, "\taverage packet inter-arrival time = N/A : No packets produced\n");
    }else{
        fprintf(stdout, "\taverage packet inter-arrival time = %.6g\n",total_i_a_time/total_packets_prod/THOUSAND); //
    }
    if (packet_served == 0) {
        fprintf(stdout, "\taverage packet service time = N/A : No packets arrived/served\n\n"); //
    }else{
        fprintf(stdout, "\taverage packet service time = %.6g\n\n",total_ser_time/packet_served/THOUSAND); //
    }
    
    if (emulation_ends == 0) {
        fprintf(stdout, "\taverage number of packets in Q1 = N/A : emulation ends at invalid time\n");
        fprintf(stdout, "\taverage number of packets in Q2 = N/A : emulation ends at invalid time\n");
        fprintf(stdout, "\taverage number of packets in S = N/A : emulation ends at invalid time\n");
        
    }else{
        fprintf(stdout, "\taverage number of packets in Q1 = %.6g\n",time_in_Q1/emulation_ends);
        fprintf(stdout, "\taverage number of packets in Q2 = %.6g\n",time_in_Q2/emulation_ends); //
        fprintf(stdout, "\taverage number of packets at S = %.6g\n\n",total_ser_time/emulation_ends); //
    }
    
    if (packet_served == 0) {
        fprintf(stdout, "\taverage time a packet spent in system = N/A : No packets arrived/served\n");
        fprintf(stderr,"\tstandard deviation for time spent in system = N/A : No packets arrived/served\n\n");
        
    }else{
        fprintf(stdout, "\taverage time a packet spent in system = %.6g\n",total_sys_time/packet_served/THOUSAND); //
        stand_dev = sqrt((sys_variance/packet_served) - (total_sys_time/packet_served * total_sys_time/packet_served));
        
        fprintf(stdout, "\tstandard deviation for time spent in system = %.6g\n\n",stand_dev/THOUSAND);
        
    }
    
    
    if(total_tokens_prod){
        token_prob = (float)tokens_drop/total_tokens_prod;
        fprintf(stdout, "\ttoken drop probability = %.6g\n",token_prob); //
    }
    else{
        fprintf(stdout,"\ttoken drop probability = N/A : No tokens arrived\n");
    }
    
    
    if(total_packets_prod){
        packet_prob = (float)packets_drop/total_packets_prod;
        fprintf(stdout, "\tpacket drop probability = %.6g\n\n",packet_prob); //
    }
    else{
        fprintf(stdout,"\tpacket drop probability = N/A : No packets arrived\n\n");
    }
    
    
}
