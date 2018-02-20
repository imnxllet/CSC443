#include <sys/timeb.h>
#include <stdio.h>
#include "part2.h"
#include <strings.h>
#include "vector.h"
#include <stdlib.h> 

int main(int argc, const char * argv[]) {
    if (argc < 3) {
        printf("Usage: scan <heapfile> <page_size>");
        return 1;
    }

    const char *heap_filename = argv[1] ;
    int page_size = atoi(argv[2]); 

    // Open the page file for writing
    FILE *fp_read_heapfile;
    fp_read_heapfile = fopen(heap_filename, "r");
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

    while(hasnext(&record_iterator, &page, &record, directory) == true){
        records_num++;

        record = next(&record_iterator);
        

        printf("\n\nRecordID: %d_%d\n", record_iterator.page_num, record_iterator.record_id);
        //printf("\nrecord #%d\n", records_num); 
        int index = 0;
        Iterator iterator = vector_begin(&record);
        Iterator last = vector_end(&record);
        for (; !iterator_equals(&iterator, &last); iterator_increment(&iterator)) {
            // *(int*)iterator_get(record) += 1;
            //memcpy((char*)buf + index, (char *)iterator_get(&iterator), ATTRIBUTE_SIZE);
            //printf("Value is %.*s\n", ATTRIBUTE_SIZE, (char *)iterator_get(&iterator));
            index += ATTRIBUTE_SIZE;
            printf("%.*s, ", ATTRIBUTE_SIZE, (char *)iterator_get(&iterator));
        }
        printf("\n");
    }


    fclose(fp_read_heapfile);

  


    // stop timer
    ftime(&t);
    unsigned long stop_ms = t.time * 1000 + t.millitm;


    printf("\n\nNUMBER OF RECORDS: %d\n", records_num);
    printf("NUMBER OF PAGES:: %d\n", record_iterator.page_num);
        printf("NUMBER OF DIRECTORIES:: %d\n", record_iterator.d_num);
    printf("Time used to write the file: %lums.\n", stop_ms - start_ms);


    return 0;

    
}


    