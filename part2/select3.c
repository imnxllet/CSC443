#include <sys/timeb.h>
#include <stdio.h>
#include "library.h"
#include <strings.h>
#include "vector.h"
#include <stdlib.h> 
#include <string.h>

int main(int argc, const char * argv[]) {
    if (argc < 7) {
        printf("Usage: select3 <heapfile> <attribute_id> <return attribute_id> <start> <end> <page_size>");
        return 1;
    }

    char colstore_file[strlen(argv[1]) + strlen(argv[2]) + 1];
    strcpy(colstore_file, argv[1]);
    strcat(colstore_file, "/");
    strcat(colstore_file, argv[2]);
    

    FILE *fp_read_heapfile;
    fp_read_heapfile = fopen(colstore_file, "r+");
    if(fp_read_heapfile == NULL){
        printf("Error in file opening\n");
    }
    //printf("hi\n");
    fseek(fp_read_heapfile, 0, SEEK_END);
    int size = ftell(fp_read_heapfile);
    int result[MAX_BUF];
    
    rewind(fp_read_heapfile);
    //printf("hi\n");
    char start[strlen(argv[4])];
    memcpy(start, argv[4], strlen(argv[4]));
    char end[strlen(argv[5])];
    memcpy(end, argv[5], strlen(argv[5]));

    bzero(result, size);
    //printf("hi\n");
     struct timeb t;
    ftime(&t);
    unsigned long start_ms = t.time * 1000 + t.millitm;
    select3(colstore_file, (char *)argv[2], start, end, (char *)argv[6], result, 0);

   /* int i = 0;
    while(result[i] != 0){
        printf("slot: %d\n", result[i]);
        i++;
    }*/

    strcpy(colstore_file, argv[1]);
    strcat(colstore_file, "/");
    strcat(colstore_file, argv[3]);
    //printf("hi\n");
    select3(colstore_file, (char *)argv[3], start, end, (char *)argv[6], result, 1);
    ftime(&t);
    unsigned long stop_ms = t.time * 1000 + t.millitm;


    //printf("\n\nNUMBER OF RECORDS: %d\n", records_num);
    //printf("NUMBER OF PAGES:: %d\n", record_iterator.page_num);
    //printf("NUMBER OF DIRECTORIES:: %d\n", record_iterator.d_num);
    printf("\n\nTime used to do select3: %lums.\n", stop_ms - start_ms);

    return 0;




}


