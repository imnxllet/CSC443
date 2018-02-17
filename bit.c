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
int togglePageBitmap(Page *page, int slot_id, int value){
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




