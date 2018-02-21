#include <sys/timeb.h>
#include <stdio.h>
#include "library.h"
#include <strings.h>
#include "vector.h"
#include <string.h>
#include <stdlib.h> 

int main(int argc, const char * argv[]) {
    if (argc < 6) {
        printf("Usage: select <heapfile> <attribute_id> <start> <end> <page_size>");
        return 1;
    }

    char colstore_file[strlen(argv[1]) + strlen(argv[2]) + 1];
    strcpy(colstore_file, argv[1]);
    strcat(colstore_file, "/");
    strcat(colstore_file, argv[2]);
    //const char *heap_filename = argv[1];
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
    fp_read_heapfile = fopen(colstore_file, "r+");
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
    printf("colstore_file is %s\n", colstore_file);
    init_heapfile(&heapfile, page_size, fp_read_heapfile, 0, (char *)colstore_file);
    
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
    int slot = 0;
    while(hasnext(&record_iterator, &page, &record, directory) == true){
        records_num++;

        
        record = next(&record_iterator);
        Iterator iterator = vector_begin(&record);
        Iterator last = vector_end(&record);
        //printf("has next\n");
        for (; !iterator_equals(&iterator, &last); iterator_increment(&iterator)) {
            slot++;
            // *(int*)iterator_get(record) += 1;
            //memcpy((char*)buf + index, (char *)iterator_get(&iterator), ATTRIBUTE_SIZE);
            //printf("Value is %.*s\n", ATTRIBUTE_SIZE, (char *)iterator_get(&iterator));
            char substring[5];

            memcpy(substring, (char *)iterator_get(&iterator) + 1, 5);
            //printf("substring is %.*s, ", 5, substring);
           // printf("start is %.*s, ", 5, start);
           // printf("end is %.*s, ", 5, end);

            if(strncmp(substring, start, 5) >= 0 && strncmp(substring, end, 5) <= 0){
                //printf("\n\nRecordID: %d_%d\n", record_iterator.page_num, record_iterator.record_id);
               // printf("\n\nSlot #: %d\n", slot);
                //printf("Extracted value-> %.*s\n", 5 ,substring);
                //printf("%.*s, ", 5 ,substring);

            }
        }




        

        
    }





    fclose(fp_read_heapfile);

  


    // stop timer
    ftime(&t);
    unsigned long stop_ms = t.time * 1000 + t.millitm;


    //printf("\n\nNUMBER OF RECORDS: %d\n", records_num);
    //printf("NUMBER OF PAGES:: %d\n", record_iterator.page_num);
    //printf("NUMBER OF DIRECTORIES:: %d\n", record_iterator.d_num);
    printf("\nTime used to do select2: %lums.\n", stop_ms - start_ms);


    return 0;
}