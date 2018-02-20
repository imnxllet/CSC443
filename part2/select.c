#include <sys/timeb.h>
#include <stdio.h>
#include "library.h"
#include <strings.h>
#include "vector.h"
#include <stdlib.h> 
#include <string.h>

int main(int argc, const char * argv[]) {
    if (argc < 6) {
        printf("Usage: select <heapfile> <attribute_id> <start> <end> <page_size>");
        return 1;
    }

    const char *heap_filename = argv[1];
   // const char *start = argv[3];
   // const char *end = argv[4];
    int attribute_id = atoi(argv[2]);
    
    //printf("strlen.. %d\n", strlen(argv[3]));
    char start[strlen(argv[3])];
    memcpy(start, argv[3], strlen(argv[3]));
    char end[strlen(argv[4])];
    memcpy(end, argv[4], strlen(argv[4]));
   /* if(strlen(argv[4]) != ATTRIBUTE_SIZE){
        printf("new value is not 10 bytes\n");
        return -1;
    }
*/  
    if(attribute_id > 99 || attribute_id <0){
        printf("Invalid attribute_id\n");
        return -1;
    }

    
    //const char *new_value = argv[4];
    int page_size = atoi(argv[5]); 

    // Open the page file for writing
    FILE *fp_read_heapfile;
    fp_read_heapfile = fopen(heap_filename, "r+");
    if(fp_read_heapfile == NULL){
        printf("Error in file opening\n");
    }


    int records_num = 0;
    //int pages_num = 0;

    // start timer
    struct timeb t;
    ftime(&t);
    unsigned long start_ms = t.time * 1000 + t.millitm;
    //char buf[page_size];

    /* Initialize the heapfile*/
    Heapfile heapfile;
    init_heapfile(&heapfile, page_size, fp_read_heapfile, 0, (char *)heap_filename);
    
    Page page;
    init_fixed_len_page(&page, heapfile.page_size, 1000);
    Record record;
    vector_setup(&record, 100, 10*sizeof(char));

    RecordIterator record_iterator;
    heapfile.slot_size = 1000;
    //printf("!!!!!%d\n", heapfile.slot_size);
    init_RecordIterator(&record_iterator, &heapfile);

    int *directory = malloc(page_size);

    //RecordID recordID;
    //getIDs((char *)record_id,  &recordID);
    //printf("page %d_ slot %d\n", recordID.page_id, recordID.slot);

    while(hasnext(&record_iterator, &page, &record, directory) == true){
        records_num++;

        
        record = next(&record_iterator);
        char *value = (char*)vector_get(&record, attribute_id);

        char substring[5];
        memcpy(substring, value + 1, 5);



        if(strncmp(substring, start, 5) >= 0 && strncmp(substring, end, 5) <= 0){
            printf("\n\nRecordID: %d_%d\n", record_iterator.page_num, record_iterator.record_id);
            printf("Extracted value-> %.*s\n", 5 ,substring);

        }

        
    }





    fclose(fp_read_heapfile);

  


    // stop timer
    ftime(&t);
    unsigned long stop_ms = t.time * 1000 + t.millitm;


    //printf("\n\nNUMBER OF RECORDS: %d\n", records_num);
    //printf("NUMBER OF PAGES:: %d\n", record_iterator.page_num);
    //printf("NUMBER OF DIRECTORIES:: %d\n", record_iterator.d_num);
    printf("Time used to write the file: %lums.\n", stop_ms - start_ms);


    return 0;
}