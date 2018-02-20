#include <time.h>
#include <sys/timeb.h>
#include <stdlib.h>
#include <stdio.h>

/**
 * populate a random array (which is already
 * allocated with enough memory to hold n bytes. 
 */
void random_array(char *array, long bytes){
    
    int i;
    for (i=0;i<bytes;i++){
        array[i] = 'A' + (rand() % 26);      
    }
    printf("Generated random Array: %s \n",array);
 
    
}

int main(int argc, char *argv[]) {

    if (argc < 4) {
        printf("Usage: create_random_file <filename> <total_size> <block_size>");
        return -1;
    } 

    char *filename = argv[1];
    printf("File size is %s\n", argv[2]);
    int file_size = atoi(argv[2]);
    printf("File size is %d\n", file_size);
    int bytes = atoi(argv[3]);
    printf("Buffer size is %d\n", bytes);
    //long bytes = strtol(argv[2], NULL, 10);

    char buffer[bytes];

    if (file_size < bytes){
        printf("File size is smaller that buffer size.");
        return -1;
    }

    // calculate remainder as block_size/total_size may not be clean
    int remaining_bytes = file_size % bytes;
    printf("Remainig byte is: %d.\n", remaining_bytes);

    // start timer
    struct timeb t;
    ftime(&t);
    unsigned long start_ms = t.time * 1000 + t.millitm;
    
    FILE *fp;
    fp = fopen(filename, "w");
    for (int i = 0; i < file_size; i += bytes) {
        random_array(buffer, bytes);
        fwrite(buffer, sizeof(char), bytes, fp);
        fflush(fp);
    }

    // write remainder
    if(remaining_bytes != 0){
        random_array(buffer, remaining_bytes);
        fwrite(buffer, sizeof(char), remaining_bytes, fp);
        fflush(fp);
    }

    // stop timer
    ftime(&t);
    unsigned long stop_ms = t.time * 1000 + t.millitm;

    fclose(fp);
    printf("Time used to write the file: %lums.\n", stop_ms - start_ms);

    return 0;

}