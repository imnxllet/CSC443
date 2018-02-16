#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <sys/mman.h>
#include "bit.h"
#include <time.h>

unsigned char *disk;


/*Helper function for exercise to print bitmap.*/
void printBit(unsigned char *byte){
    printf(" ");
    for(int i = 0; i < 8; i++){
        int bit = *byte & 1 << i; //check bit 1 or 0
        if(bit) printf("1"); 
        else printf("0");
    }     
}



int fixed_len_page_capacity(Page *page){
    /*Page size =  M*slot_size + 2 or 1 (to store M) + M/8 (M bits)*/
    int num_slots_M = (page->page_size - sizeof(int)) / ((1/8) + page->slot_size);
    return num_slots_M;

}



/*Find next free node from bitmap*/
int find_FreeSlot(Page *page){
    unsigned char *slot_bitmap = (unsigned char *)page->data + sizeof(int);
    int num_slots = fixed_len_page_capacity(page);

    int counter = 0;
    //unsigned char *inode_bitmap = index(disk, group->bg_inode_bitmap);
    //printf("\nInode bitmap:");
    for (int i =0; i < num_slots / 8; i++){

        unsigned char *byte = slot_bitmap + i;
        for(int j = 0; j < 8; j++){
            counter++;
            int bit = *byte & 1 << j; //check bit 1 or 0
            if(bit){
                continue;
            }else{
                return counter;
            }   
        }
    }
    //No Free Inode found.
    return -1;
}


/*Update inode with id in inode bitmap to value. Adjust sb,group descriptor accordingly.*/
int toggleInodeBitmap(Page *page, int slot_id, int value){
    //Toggle to 1

        //struct ext2_group_desc *group = (struct ext2_group_desc *)(disk + 2048);
        int num_slots = fixed_len_page_capacity(page);
        int counter = 0;
        unsigned char *page_bitmap = (unsigned char *)page->data + sizeof(int);

        //printf("\nInode bitmap:");
        for (int i =0; i < num_slots / 8; i++){
            unsigned char *byte = page_bitmap + i;
            for(int j = 0; j < 8; j++){
                counter++;
                //set bit
                if(counter == slot_id){
                    if(value == 1){//Set high
                        page->free_slots--;
                        page->used_slots++;
                        *byte |= (1 << j);
                        return 0;
                    }else{
                        page->free_slots++;
                        page->used_slots--;
                        *byte &= ~(1 << j);
                        return 0;
                    }
                }
               
            }
        }
        //error
        return -1;
}


/*****
*useless ..
*
******/



/*Check if inode with inode_id is mark as used in bitmap. 1 as used, 0 as unused*/

int checkInodeBit(unsigned char *disk, int inode_id){
    //be aware that inode_num and inode_index are different.
    inode_id = inode_id - 1;
    struct ext2_group_desc *group = (struct ext2_group_desc *)(disk + EXT2_BLOCK_SIZE * 2 );
    //struct ext2_super_block *sb = (struct ext2_super_block *)(disk + EXT2_BLOCK_SIZE);

    unsigned char *inode_bitmap = index(disk, group->bg_inode_bitmap);

    unsigned int count_of_eight = inode_id / 8;
    unsigned int left = inode_id % 8 ;

    unsigned char *byte = inode_bitmap + count_of_eight;
    //e.g. 1 << 5 == 100000
    int bit = *byte & 1 << left; //check bit 1 or 0
    if(bit){
        return 1;
    }else{
        return 0;
    }
}




/*helper to extract string from beginning until it reaches length.*/
char * substring(char *string, int length){
    //char substring[length + 1];
    //printf("length is %d\n", length);
    char* substring = 0;
    substring = (char *) malloc(sizeof(char) * (length + 1));

    /*int c = 0;
     while (c < length) {
      substring[c] = string[c];
      c++;
    }
    */

    memcpy(substring,string,length);
    //otherString[length] = 0;
    substring[length] = '\0';
    return substring;
}

