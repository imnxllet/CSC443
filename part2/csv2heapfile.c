#include <sys/timeb.h>
#include <stdio.h>
#include "library.h"
#include <stdlib.h> 
#include "vector.h"
//#include<cstdlib>
#include <strings.h>

int main(int argc, const char * argv[]) {
    if (argc < 4) {
        printf("Usage: csv2heapfile <csv_file> <heap_file> <page_size>");
        return 1;
    }
    const char *csv_filename = argv[1];
    const char *heap_filename = argv[2] ;
    int page_size = atoi(argv[3]); 

    // Open the page file for writing
    FILE *fp_write_heapfile;
    /*get size of file..*/
    
    /*fp_write_heapfile = fopen(heap_filename, "r");
    fseek(fp_write_heapfile, 0, SEEK_END);
    int size; 
    size = ftell(fp_write_heapfile);
    rewind(fp_write_heapfile);
    printf("size is %d\n", size);

    fclose(fp_write_heapfile);*/

    fp_write_heapfile = fopen(heap_filename, "w+b");
    

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

    /* Initialize the heap file. */
    Heapfile heapfile;
    init_heapfile(&heapfile, page_size, fp_write_heapfile, 0, (char *)heap_filename);
    PageID current_pid;
    while ((read = getline(&line, &len, fp_read_csv)) != -1) {
        //printf("!start writing record #%d...\n", records_num + 1);
        records_num++;
        //Doubt this..
        Record record;
        vector_setup(&record, 100, 10*sizeof(char));
        heapfile.slot_size = fixed_len_sizeof(&record);
        fixed_len_read((void *)line, (int) read, &record);
        //printf("Retrieved line of length %zu :\n", read);
        //printf("The line record is: %s\n", line);

        /* First record, initialize a new directory and page */
        if(new_page){

            init_fixed_len_page(&page, page_size, fixed_len_sizeof(&record));          
            //printf("making a new page..\n");
            pages_num++;
            new_page = 0;
            if(page.free_slots == 0){
                printf("The page size is smaller than a record, abort.\n");
                return -1;
            }
            //printf("Allocating space to this new page in heapfile...\n");
            current_pid = alloc_page(&heapfile, &page);
            //printf("Allocated a page on heap, pid = %d.\n\n", (int) current_pid);
        }

        if(page.free_slots == 0){/* Write to file and Initialize a new one */
            //printf("Page is full, write it to heapfile \n");
            write_page(&page, &heapfile, current_pid);
            fflush(heapfile.file_ptr);


            free(page.data);

            init_fixed_len_page(&page, page_size, fixed_len_sizeof(&record));

            
            current_pid = alloc_page(&heapfile, &page);
            //printf("Allocated a page on heap, pid = %d.\n\n", (int) current_pid);
            pages_num++;
            new_page = 0;
            if(add_fixed_len_page(&page, &record) == -1){
                printf("Record size > slot size, cannot add to pade.\n");
                return -1;
            }

        }else{
            //printf("Adding a new record..\n");
            if(add_fixed_len_page(&page, &record) == -1){
                printf("Record size > slot size, cannot add to pade.\n");
                return -1;
            }

        }
        //free(&record);

    }
    //printf("Last record done.. write page to heapfile \n");


    write_page(&page, &heapfile, current_pid);
    //free(page.data);
    //free(&page);
    
    fflush(heapfile.file_ptr);

    /*    int* directory = malloc(page_size);
    bzero(directory, page_size);
    fseek (fp_write_heapfile, 0, SEEK_SET);
    fread(directory, 1, page_size, fp_write_heapfile);


        
        
    for(int j=0; j < (page_size / sizeof(int)) / 2 * 2;j++){
        printf("Directory #%d: %d\n", j, directory[j]);
    }
*/
    fclose(fp_read_csv);
    fclose(fp_write_heapfile);

  


    // stop timer
    ftime(&t);
    unsigned long stop_ms = t.time * 1000 + t.millitm;


    printf("NUMBER OF RECORDS: %d\n", records_num);
    printf("NUMBER OF PAGES:: %d\n", pages_num);
    printf("NUMBER OF DIRECTORIES:: %d\n", heapfile.d_num);
    printf("NUMBER OF data Page in a D:: %d\n", heapfile.directory_num_pages / 2 - 1);
    
    printf("Time used to write the file: %lums.\n\n", stop_ms - start_ms);


    return 0;
}