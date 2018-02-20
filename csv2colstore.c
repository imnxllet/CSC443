#include <sys/timeb.h>
#include <stdio.h>
#include "part2.h"
#include <stdlib.h> 
#include "vector.h"
//#include<cstdlib>
#include <strings.h>
#include <sys/stat.h>

int main(int argc, const char * argv[]) {
    if (argc < 4) {
        printf("Usage: csv2colstore <csv_file> <colstore_name> <page_size>");
        return 1;
    }
    char filename[strlen(argv[2]) + 3];
    const char *csv_filename = argv[1];
    //const char *heap_filename = argv[2] ;
    int page_size = atoi(argv[3]); 
    mkdir(argv[2],S_IRWXU);
    strcpy(filename, argv[2]);
    strcat(filename, "/");



    

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

    fseek(fp_read_csv, 0, SEEK_END);
    int size = ftell(fp_read_csv);
    rewind(fp_read_csv);
    int num_records = size / RECORD_REAL_SIZE;

    printf("There is %d records in csv\n", num_records);

    Record all_records;
    vector_setup(&all_records, num_records, RECORD_SIZE);


    ;
   // int pages_num = 0;

    // start timer
    struct timeb t;
    ftime(&t);
    unsigned long start_ms = t.time * 1000 + t.millitm;

    //loop line, each line is a record that has 100 values..
  //  int new_page = 1;
   // Page page;

    /* Initialize the heap file. */
   // Heapfile heapfile;
    //init_heapfile(&heapfile, page_size, fp_write_heapfile, 0, (char *)heap_filename);
   // PageID current_pid;


    while ((read = getline(&line, &len, fp_read_csv)) > 0) {
        //printf("!start writing record #%d...\n", records_num + 1);
        //records_num++;
        //Doubt this..
        Record record;
        vector_setup(&record, 100, ATTRIBUTE_SIZE*sizeof(char));

        /* read the record */
        fixed_len_read((void *)line, (int) read, &record);
        vector_push_back(&all_records, &record);


    }
    //printf("get %d records\n", records_num);

    //char attribute[ATTRIBUTE_SIZE] = "0123456789";
    for( int i = 0; i < ATTRIBUTE_SIZE ; i++){
        char file_name[strlen(argv[2]) + 3];
        int records_num = 0;
        strcpy(file_name, filename);
        char name[2];
        sprintf(name, "%d", i);
        name[1] = '\0';
        int pages_num = 0;
        int new_page = 1;
        FILE *fp_write_heapfile;
        strncat(file_name, name, 2);
        //strcat(filename, "\0");
        printf("Creating heapfile: %s\n", file_name);
        fp_write_heapfile = fopen(file_name, "w+b");
        if (fp_write_heapfile == NULL){
            printf("Cannot open csv file\n");
            return -1;
        }
        Heapfile heapfile;
        //int pages_num = 0;
        init_heapfile(&heapfile, page_size, fp_write_heapfile, 0, name);
        PageID current_pid;
        Page page;
        /* loop over all the row record, and put specific attribute value from a tuple to a record */
        /* Every 100 record make a record for that attribute..*/
        int record_processed = 0;
        int record_left = num_records;
        while(record_processed < num_records){
            /* try to make a record*/
            Record record;
            records_num++;
            vector_setup(&record, 100, 10*sizeof(char));
            heapfile.slot_size = fixed_len_sizeof(&record);
            
            //for(int j = record_processed; j < ; j++){
            int j = 0;
            //printf("hi\n");
            while(j < min(100, record_left) ){
                //printf("%d\n", j);
                j++;
                
                //printf("record_processed: 5d\n", record_processed;);
                Record *sub_record = (Record *)vector_get(&all_records, record_processed);
                record_processed ++;
                //printf("hi\n");
               // printf("hi\n");
                vector_push_back(&record, (char *)vector_get(sub_record, i));
                 
               // printf("hi\n");

                //printf("(%d)\n", j);

            }

            
            record_left -= j;

            printf("processed %d records\n", record_processed);

            /* First record, initialize a new directory and page */
            if(new_page){

                init_fixed_len_page(&page, page_size, fixed_len_sizeof(&record));
                //printf("record size is...... %d\n", fixed_len_sizeof(&record));          
                //printf("making a new page..\n");
                pages_num++;
                new_page = 0;
                if(page.free_slots == 0){
                    printf("The page size is smaller than a record, abort.\n");
                    return -1;
                }
                //printf("Allocating space to this new page in heapfile...\n");
                current_pid = alloc_page(&heapfile, &page);
                printf("Allocated a page on heap, pid = %d.\n\n", (int) current_pid);
            }

            if(page.free_slots == 0){/* Write to file and Initialize a new one */
                printf("Page is full, write it to heapfile \n");
                write_page(&page, &heapfile, current_pid);
                fflush(heapfile.file_ptr);


                free(page.data);

                init_fixed_len_page(&page, page_size, fixed_len_sizeof(&record));

                
                current_pid = alloc_page(&heapfile, &page);
                printf("Allocated a page on heap, pid = %d.\n\n", (int) current_pid);
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




        }

        write_page(&page, &heapfile, current_pid);
        fflush(heapfile.file_ptr);
        fclose(fp_write_heapfile);
        printf("!start writing record #%d...\n", records_num + 1);
        
        //Doubt this..
        printf("NUMBER OF RECORDS: %d\n", records_num);
        printf("NUMBER OF PAGES:: %d\n", pages_num);
        printf("NUMBER OF DIRECTORIES:: %d\n", heapfile.d_num);
        //printf("NUMBER OF data Page in a D:: %d\n", heapfile.directory_num_pages / 2 - 1);
        
        //free(&record);
    }



   // 
    //free(page.data);
    //free(&page);
    
    

    /*    int* directory = malloc(page_size);
    bzero(directory, page_size);
    fseek (fp_write_heapfile, 0, SEEK_SET);
    fread(directory, 1, page_size, fp_write_heapfile);


        
        
    for(int j=0; j < (page_size / sizeof(int)) / 2 * 2;j++){
        printf("Directory #%d: %d\n", j, directory[j]);
    }
*/
    fclose(fp_read_csv);
    

    // stop timer
    ftime(&t);
    unsigned long stop_ms = t.time * 1000 + t.millitm;


    
    
    printf("Time used to write the file: %lums.\n", stop_ms - start_ms);


    return 0;
}