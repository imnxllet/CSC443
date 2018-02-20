#include <sys/timeb.h>
#include <stdio.h>
#include "library.h"
#include <stdlib.h> 
#include "vector.h"
//#include<cstdlib>

int main(int argc, const char * argv[]) {
    if (argc < 4) {
        printf("Usage: write_fixed_len_pages <csv_file> <page_file> <page_size>");
        return 1;
    }
    const char *csv_filename = argv[1];
    const char *page_filename = argv[2] ;
    int page_size = atoi(argv[3]); 

    // Open the page file for writing
    FILE *fp_write_page;
    fp_write_page = fopen(page_filename, "w");
    

    // Read the CSV file line-by-line:
    FILE * fp_read_csv;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;

    fp_read_csv = fopen(csv_filename, "r");
    if (fp_read_csv == NULL){
        printf("Cannot open csv file\n");
        return -1;
    }


    int records_num = 0;
    int pages_num = 0;

    // start timer
    struct timeb t;
    ftime(&t);
    unsigned long start_ms = t.time * 1000 + t.millitm;

    //loop line, each line is a record that has 100 values..
    int new_page = 1;
    Page page;
    while ((read = getline(&line, &len, fp_read_csv)) != -1) {
        records_num++;
        //Doubt this..
        Record record;
        vector_setup(&record, 100, 10*sizeof(char));
        fixed_len_read((void *)line, (int) read, &record);
        //printf("Retrieved line of length %zu :\n", read);
        //printf("The line record is: %s\n", line);

        if(new_page){
            init_fixed_len_page(&page, page_size, fixed_len_sizeof(&record));
            printf("making a new page..\n");
            pages_num++;
            new_page = 0;
            if(page.free_slots == 0){
                printf("The page size is smaller than a record, abort.\n");
                return -1;
            }
        }

        if(page.free_slots == 0){/* Write to file and Initialize a new one */
            printf("Page is full, make a new one and add a new record\n");
            fwrite((const char *)page.data, sizeof(char), page_size, fp_write_page);
            fflush(fp_write_page);
            printf("!!Iam here...\n");

            free(page.data);

            init_fixed_len_page(&page, page_size, fixed_len_sizeof(&record));
            pages_num++;
            new_page = 0;
            if(add_fixed_len_page(&page, &record) == -1){
                printf("Record size > slot size, cannot add to pade.\n");
                return -1;
            }

        }else{
            printf("Adding a new record..\n");
            if(add_fixed_len_page(&page, &record) == -1){
                printf("Record size > slot size, cannot add to pade.\n");
                return -1;
            }

        }
        //free(&record);

    }

    fwrite((const char *)page.data, sizeof(char), page_size, fp_write_page);
    //free(page.data);
    //free(&page);
    
    fflush(fp_write_page);



    fclose(fp_read_csv);
    fclose(fp_write_page);

  


    // stop timer
    ftime(&t);
    unsigned long stop_ms = t.time * 1000 + t.millitm;


    printf("NUMBER OF RECORDS: %d\n", records_num);
    printf("NUMBER OF PAGES:: %d\n", pages_num);
    printf("Time used to write the file: %lums.\n", stop_ms - start_ms);


    return 0;
}