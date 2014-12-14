//
//  main.c
//  Token Bucket
//
//  Created by Lavish Singal on 6/12/14.
//  Copyright (c) 2014 Lavish Singal. All rights reserved.
//

#include "warmup2.h"
#include <sys/stat.h>


/*File Pointer */
FILE *fp = NULL;
/* Thread ID */
pthread_t thrd[N];
double emulation_ends = 0.0;
/* Default parameters */
/* Interarrival time */
float lamda= 0.5;
/* Service rate */
float mu = 0.35;
/* Token filling rate */
float r = 1.5;
/* Maximum token bucket size */
int B = 10;
/* Packet required token size */
int P = 3;
/* # of packers to serve */
int num = 20;
/* file address */
const char *filename= NULL;


/* check whether command line is valid or not */

int checkValidity(const char *str, int *ifnegative){
    
    if(str == NULL){
        return 0;
    }
    
    if(*str++ == '-'){
        if(!*ifnegative)
            *ifnegative = 1;
    }
    
    while (*str != '\0') {
        if((*str >= '0' && *str <= '9') || *str == '.')
            str++;
        else
            return 0;
    }
    
    return 1;
}


/*          Read Command Arguments          */

void ReadCommandArguments(int argc, const char *argv[]){
    
    int i=0;
    int ifnegative = 0;
    struct stat statbuf;
    
    
    for (i=1; i<argc; i++) {
        
        if(strcmp(argv[i],"-lambda")==0){
            if(checkValidity(argv[i+1],&ifnegative)){
                sscanf(argv[i+1], "%f", &lamda);
                if(lamda < 0.1)
                    lamda = 0.1;
            }else{
                fprintf(stderr, "Invalid Commandline\n");
                exit(1);
            }
            continue;
        }
        if(strcmp(argv[i],"-mu")==0){
            if(checkValidity(argv[i+1],&ifnegative)){
                sscanf(argv[i+1], "%f", &mu);
                if(mu < 0.1)
                    mu = 0.1;
            }else{
                fprintf(stderr, "Invalid Commandline\n");
                exit(1);
            }
            
            continue;
            
        }
        if(strcmp(argv[i],"-r")==0){
            if(checkValidity(argv[i+1],&ifnegative)){
                sscanf(argv[i+1], "%f", &r);
                if(r < 0.1)
                    r = 0.1;
            }else{
                fprintf(stderr, "Invalid Commandline\n");
                exit(1);
            }
            continue;
        }
        
        if(strcmp(argv[i],"-B")==0){
            if(checkValidity(argv[i+1],&ifnegative)){
                B = atoi(argv[i+1]);
            }else{
                fprintf(stderr, "Invalid Commandline\n");
                exit(1);
            }
            continue;
        }
        
        if(strcmp(argv[i],"-P")==0){
            if(checkValidity(argv[i+1],&ifnegative)){
                P = atoi(argv[i+1]);
            }else{
                fprintf(stderr, "Invalid Commandline\n");
                exit(1);
            }
            continue;
        }
        if(strcmp(argv[i],"-n")==0){
            if(checkValidity(argv[i+1],&ifnegative)){
                    num = atoi(argv[i+1]);
                /* Check whether # is greater than the integer limit */
                    if( strlen(argv[i+1])>10 || (num < 0 && strlen(argv[i+1]) == 10))
                        ifnegative = 1;
            }else{
                fprintf(stderr, "Invalid Commandline\n");
                exit(1);
            }
            continue;
        }
        if(strcmp(argv[i],"-t")==0){
            filename = argv[i+1];
            continue;
        }
        
    }
    if(filename){
        fp = fopen(filename, "r");
        stat(filename, &statbuf);
        
        if(S_ISDIR(statbuf.st_mode)){
            fprintf(stderr, "\"%s\" is a directory\n",argv[2]);
            exit(1);
        }
        
        if(fp == NULL){
            fprintf(stderr, "Cannot open the file!\n");
            exit(1);
        }else{
            fscanf(fp,"%d",&num);
        }
    }else if(ifnegative == 1){
        fprintf(stderr, "Invalid value\n");
        exit(1);
    }
    
}

/* MAIN */

int main(int argc, const char * argv[])
{
    
    sigset(SIGINT, handler);
    
    
    if(argc<1 && argc>15){
        fprintf(stderr, "Error in argumentss/n");
        exit(1);
    }
    
    
    ReadCommandArguments(argc,argv);
    
    PrintParameters();
    SaveCurTimeOfDay();
    
    fprintf(stdout, "00000000.000ms: emulation begins\n");
    
    CreateThreads();
    
    
    emulation_ends = GetMTimeOfDay();
    fprintf(stdout, "%012.3lfms: emulation ends\n ",emulation_ends);
    
    PrintStatistics();
    return 0;
}
