#include "cachelab.h"

#include <getopt.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <math.h>

struct tagLine{
    unsigned long tag;
    unsigned int id;
    unsigned char block[1];
};

char *cache = NULL;

int num_s = 0;
int num_E = 0;
int num_b = 0;

int bit_s = 0;
int bit_b = 0;

int hit = 0;
int miss = 0;
int evict = 0;


unsigned int b_mask = 0;
unsigned long time_stamp = 0;

struct tagLine * getLineAddr(unsigned int group, unsigned int LineIndex){
    return (struct tagLine*)(cache + group * num_E * sizeof(struct tagLine) + LineIndex * sizeof(struct tagLine));
}

unsigned int getGroup(unsigned int addr){
    unsigned int temp = 1;
    temp = (unsigned int)((temp << bit_s)-1);
    return (addr >> bit_b)&temp;
}

unsigned int getID(unsigned int addr){
    return (addr >> (bit_s+bit_b));
}

void process(unsigned int addr, unsigned int size){

    int indexLine = 0;
    for(;;){
        ++time_stamp;
        for(indexLine = 0;indexLine < num_E;++indexLine){
            struct tagLine * temp = getLineAddr(getGroup(addr), indexLine);
            // evict or hit
            if(temp->tag){
                unsigned int tempID = getID(addr);
                // hit
                if(tempID == temp->id){
                    hit++;
                    temp->tag=time_stamp;
                    break;
                }
            }
        }
        // miss
        if(indexLine == num_E){
            // find LRU
            ++miss;
            unsigned int Least = (unsigned int)(-1);
            unsigned int index = 0;
            for(indexLine = 0;indexLine < num_E;++indexLine){
                struct tagLine * temp = getLineAddr(getGroup(addr), indexLine);
                if(temp->tag <= Least){
                    index = indexLine;
                    Least = temp->tag;
                }
            }
            struct tagLine * temp = getLineAddr(getGroup(addr), index);
            if(temp->tag){
                evict++;
            }
            temp->tag = time_stamp;
            temp->id = getID(addr);
        }

        if(addr+size > (addr&b_mask)+num_b){
            size -= ((addr&b_mask)+num_b - addr);
            addr = (addr&b_mask)+num_b;
        }else break;
    }
}

int main(int argc, char * argv[])
{
    char filename[100];
    char ch = 0;
	printf("test sizeof int:%f",1.*sizeof(int));
    while ((ch = getopt(argc, argv, "s:E:b:t:")) != -1)
    {
        switch (ch)
        {
            case 's':
                bit_s = atoi(optarg);
                num_s = (1<<bit_s);
                //printf("num_s:%d\n",num_s);
                break;
            case 'E':
                num_E = atoi(optarg);
                //printf("num_E:%d\n",num_E);
                break;
            case 'b':
                bit_b = atoi(optarg);
                num_b = (1<<bit_b);
                //bit_b = (int)(log(num_b+1.)/log(2));
                //printf("num_b:%d\n",num_b);
                break;
            case 't':
                strcpy(filename, optarg);
                //printf("filename is %s\n", filename);
                 break;
            case '?':
                //printf("Unknown option: %c\n",(char)optopt);
                break;
        }
    }

    b_mask = (unsigned int)(~(  ( ((unsigned int)1) << bit_b )-1  ));
    cache = (char *)calloc( num_s * num_E, sizeof(struct tagLine) );

    hit = 0;
    miss = 0;
    evict = 0;

    FILE* fp = NULL;
    char temp[128]={0};
    unsigned int addr = 0;
    unsigned int size = 0;
    char type = 0;
    if((fp=fopen(filename,"r"))!=NULL){
        while(!feof(fp)){
            if(fgets(temp,127,fp)!=NULL){
                if(temp[0]==' '){
                    type = temp[1];
                    sscanf(temp, "%*s %x,%d", &addr, &size);
                    //printf("addr:%x, size:%d\n",addr,size);
                    process(addr,size);
                    if(type=='M') process(addr,size);
                }
            }
        }
    }else exit(1);

    fclose(fp);

    if(cache != NULL) free(cache);
    cache = NULL;
    printf("%d %d %d\n", hit, miss, evict);
    printSummary(hit, miss, evict);
    //printSummary(0,0,0);
    return 0;
}
