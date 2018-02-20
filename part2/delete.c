#include <sys/timeb.h>
#include <stdio.h>
#include "library.h"
#include <strings.h>
#include "vector.h"
#include <stdlib.h> 

int main(int argc, const char * argv[]) {
    if (argc < 4) {
        printf("Usage: update <heapfile> <record_id> <page_size>");
        return 1;
    }

    const char *heap_filename = argv[1];
    const char *record_id = argv[2];



    //memcpy(new_value, argv[4], ATTRIBUTE_SIZE);
    //const char *new_value = argv[4];
    int page_size = atoi(argv[3]); 

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

    RecordID recordID;
    getIDs((char *)record_id,  &recordID);
    //printf("page %d_ slot %d\n", recordID.page_id, recordID.slot);

    bool found = false;
    while(hasnext(&record_iterator, &page, &record, directory) == true){
        records_num++;


        /* Locate the record..*/
        if(record_iterator.page_num == recordID.page_id && record_iterator.record_id == recordID.slot){
            printf("Deleting the record at pageID=%d, @slot=%d\n", recordID.page_id, recordID.slot);
            record = next(&record_iterator);

            //vector_assign(&record, attribute_id, (char *)new_value);
            togglePageBitmap(&page, recordID.slot, 0);
    
            //write_fixed_len_page(&page, recordID.slot, &record);
            //printf("Delete attribute @%d", attribute_id);
            //printf(", to new value: %.*s\n", ATTRIBUTE_SIZE ,argv[4]);
            write_page(&page, &heapfile, recordID.page_id);


            found = true;
            break;


        }else{
            continue;
        }
        
        


        
    }

    if(!found){
        printf("Cannot locate the record, update fail.\n");
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