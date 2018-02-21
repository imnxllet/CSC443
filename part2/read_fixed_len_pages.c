#include <sys/timeb.h>
#include <stdio.h>
#include "library.h"
#include <strings.h>
#include "vector.h"
#include <stdlib.h> 

int main(int argc, const char * argv[]) {
    if (argc < 3) {
        printf("Usage: read_fixed_len_pages <page_file> <page_size>");
        return 1;
    }

    const char *page_filename = argv[1] ;
    int page_size = atoi(argv[2]); 

    // Open the page file for writing
    FILE *fp_read_page;
    fp_read_page = fopen(page_filename, "r");
    if(fp_read_page == NULL){
        printf("Error in file opening\n");
    }


    int records_num = 0;
    int pages_num = 0;

    // start timer
    struct timeb t;
    ftime(&t);
    unsigned long start_ms = t.time * 1000 + t.millitm;
    //char buf[page_size];
    
    while(!feof(fp_read_page)) {
        
        
        Page page;
        init_fixed_len_page(&page, page_size, 1000);
        bzero(page.data, page_size);
        int read_bytes = fread(page.data, sizeof(char), page_size, fp_read_page);
        if(!read_bytes){
            free(page.data);
            break;;
        }
        //fread(buf, 1, block_size, file_ptr);
        //printf("Read %d bytes\n", read_bytes);
        for (int i = 1; i <= page.total_slot; i ++) {
            
            Record record;
            vector_setup(&record, 100, 10*sizeof(char));
            /*Empty slot*/
            if(checkValue(&page, i) == 0){
                //printf("Slot %d has no record..\n", i);
                continue;
            }
           // printf("Slot %d has a record..\n", i);
            //printf("hi\n");
            read_fixed_len_page(&page, i, &record);
            records_num++;
            /*
            Iterator iterator = vector_begin(&record);
            Iterator last = vector_end(&record);
            printf("\nRecord #%d\n", records_num);
            for (; !iterator_equals(&iterator, &last); iterator_increment(&iterator)) {
                // *(int*)iterator_get(record) += 1;
                //memcpy((char*)buf + index, (char *)iterator_get(&iterator), ATTRIBUTE_SIZE);
                //printf("Value is %.*s\n", ATTRIBUTE_SIZE, (char *)iterator_get(&iterator));
                //index += ATTRIBUTE_SIZE;

                printf("%.*s, ", ATTRIBUTE_SIZE, (char *)iterator_get(&iterator));
            }
        printf("\n");*/
            
        }

        pages_num++;
        //printf("NUMBER OF RECORDS: %d\n", records_num);
    }

    fclose(fp_read_page);

  


    // stop timer
    ftime(&t);
    unsigned long stop_ms = t.time * 1000 + t.millitm;


    printf("NUMBER OF RECORDS: %d\n", records_num);
    printf("NUMBER OF PAGES:: %d\n", pages_num);
    printf("Time used to read the file: %lums.\n", stop_ms - start_ms);
    printf("RATE(RECORD/S): %lu\n\n", records_num/ (stop_ms - start_ms) * 1000);


    return 0;
}