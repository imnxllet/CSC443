#include <sys/timeb.h>
#include <stdio.h>
#include "part2.h"
#include<cstdlib>
#include <iostream>

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

        fread(page.data, sizeof(char), page_size, fp_read_page);
        //fread(buf, 1, block_size, file_ptr);

        for (int i = 1; i <= page.total_slot; i ++) {
            records_num++;
            Record record;
            read_fixed_len_page(&page, i, &record);
            printf("Record %d\n", i);
            for (int j = 0; j < record.size(); j++) {
                printf("%s", record.at(j));
                // fputs(record.at(j), dev_null);
                if (j != record.size() - 1){
                    std::cout << ",";
                }
            }
            printf("\n");
            
        }

        pages_num++;
    }
    fclose(fp_read_page);

  


    // stop timer
    ftime(&t);
    unsigned long stop_ms = t.time * 1000 + t.millitm;


    printf("NUMBER OF RECORDS: %d\n", records_num);
    printf("NUMBER OF PAGES:: %d\n", pages_num);
    printf("Time used to write the file: %lums.\n", stop_ms - start_ms);


    return 0;
}