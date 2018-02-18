#include <time.h>
#include <sys/timeb.h>
#include <stdlib.h>
#include <stdio.h>

/**
 * populate a random array (which is already
 * allocated with enough memory to hold n bytes. 
 */


int main(int argc, char *argv[]) {

    if (argc < 2) {
        printf("Usage: create_random_file <filename> <total_size> <block_size>");
        return -1;
    } 

    char *filename = argv[1];
   
    FILE *fp;
    fp = fopen(filename, "w+b");
    char buff[3];
    char buf[3];
    buf[0] = 'i';
    buf[1] = 'b';
    buf[2] = '\0';

    rewind(fp);
    fwrite(buf, sizeof(char), 3, fp);
    fflush(fp);

    fseek(fp, 3, SEEK_SET);
    fwrite(buf, sizeof(char), 3, fp);
    fflush(fp);
    rewind(fp);
    fread(buff, sizeof(char), 3, fp);
    printf("Read: %s\n", buff);
  
    fclose(fp);

    return 0;

}