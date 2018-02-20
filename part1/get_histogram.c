#include <time.h>
#include <sys/timeb.h>
#include <stdlib.h>
#include <stdio.h>
#include <strings.h>

/**
 * file_ptr : the file pointer, ready to be read from.
 * hist: an array to hold 26 long integers.  hist[0] is the
 *       number of 'A', and hist[1] is the number of 'B', etc.
 * block_size: the buffer size to be used.
 * milliseconds: time it took to complete the file scan
 * total_bytes_read: the amount data in bytes read
 *
 * returns: -1 if there is an error.
 */
int get_histogram(
    FILE *file_ptr,
    long hist[],
    int block_size,
    long *milliseconds,
    long *total_bytes_read
){

    char buf[block_size];

    // start timer
    struct timeb t;
    ftime(&t);
    unsigned long start_ms = t.time * 1000 + t.millitm;

    for (int i = 0; !feof(file_ptr); i += block_size) {
        
        /* Clean the buffer. */
        bzero(buf, block_size);
        fread(buf, sizeof(char), block_size, file_ptr);
        //fread(buf, 1, block_size, file_ptr);

        for (int i = 0; i < block_size; i += 1) {
            // ~~ASCII~~ powers
            int index = (int)(buf[i] - 'A'); 
            if(index < 26 && index >= 0){
                hist[index] += 1;
                //printf("The index is: %d\n", index);
                //printf("The char(ok) is: %c\n", buf[i]);
            }else{
                //printf("The char(>25) is: %c\n", buf[i]);
            }
        }

        *total_bytes_read += (long)block_size;
    }
    //Close the file 
    fclose(file_ptr);

    // stop timer
    ftime(&t);
    unsigned long stop_ms = t.time * 1000 + t.millitm;
    *milliseconds = stop_ms - start_ms;
    
    return 0;

}

int main(int argc, char *argv[]) {

    long hist[26];
    long milliseconds;
    long filelen;
    FILE *file_ptr = fopen(argv[1], "r");

    /*Compute the histogram using 2k buffers*/
    for (int i = 0; i < 26; i += 1) {
        hist[i] = 0;
    }

    int error = get_histogram(file_ptr, hist, atoi(argv[2]), &milliseconds, &filelen);

    if(error < 0){
        printf("error\n");
        return -1;
    }

    printf("Computed the histogram in %d ms.\n", (int)milliseconds);
    for(int i=0; i < 26; i++){
        printf("%c : %d\n", 'A' + i, (int)hist[i]);
    }
    printf("Data rate: %f Bps\n", (double)filelen/milliseconds * 1000);

}