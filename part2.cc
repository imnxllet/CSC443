#include "bit.h"
#include "part2.h"
#include <time.h>
#include <sys/timeb.h>
#include <stdlib.h>
#include <stdio.h>
#include <strings.h>

/* Compute the number of bytes required to serialize record.*/
int fixed_len_sizeof(Record *record){
    /* number of items inside the vector */

    Record::iterator it;  // declare an iterator to a vector of strings
    int size = 0;

    for(it = record.begin(); it != record.end(); it++,i++ )    {

        size += sizeof(char) * strlen(*it);
    }

    return size;
}

/*Serialize the record to a byte array to be stored in buf*/
void fixed_len_write(Record *record, void *buf){
/* Write the bytes in the buf */    
    Record::iterator it;  // declare an iterator to a vector of strings
    int size = 0;
    int index = 0;
    for(it = record.begin(); it != record.end(); it++)    {

        memcpy((char*)buf + index, *it, ATTRIBUTE_SIZE);
        index += ATTRIBUTE_SIZE;
    }

}
/**
 * Deserializes `size` bytes from the buffer, `buf`, and
 * stores the data in `record`.
 */
 void fixed_len_read(void *buf, int size, Record *record){
    /* Value is of 10 bytes long(ATTRIBUTE_SIZE) for each attribute. */

    int values_count = size / (ATTRIBUTE_SIZE + 1); //=100
    printf("The buf has %d record.\n", values_count);
    int index = 0;
    for (int i = 0; i < values_count; i++) {
        char value[ATTRIBUTE_SIZE + 1];
        memcpy(value, (char*)buf + index, (ATTRIBUTE_SIZE + 1));
        value[ATTRIBUTE_SIZE] = '\0';
        index += ATTRIBUTE_SIZE + 1;

        //attribute[ATTRIBUTE_SIZE] = '\0';
        printf("pushing record %d\n", i);
        record->push_back(value);
        /*if (strlen(value) > 0) {
            record->push_back(attribute);
        }*/
    }
 }


/*Part2.2: Page layout

typedef struct{
    void *data;
    int page_size;
    int slot_size;
    int free_slots;
    int used_slots;
} Page;

*/



 /*Initializes a page using the given slot size. */
void init_fixed_len_page(Page *page, int page_size, int slot_size){
    page->slot_size = slot_size;
    page->page_size = page_size;
    page->free_slots = fixed_len_page_capacity(page);
    page->used_slots = 0;
    page->data = malloc(page_size);
}

/* Calculates the maximal number of records that fit in a page. */

//int fixed_len_page_capacity(Page *page){
    /*Page size =  M*slot_size + 2 or 1 (to store M) + M/8 (M bits)*/
 //   int num_slots_M = (page->page_size - sizeof(int)) / ((1/8) + page->slot_size);
   // return num_slots_M;

//}

/* Calculate the free space (number of free slots) in the page */
int fixed_len_page_freelots(Page *page){

    return page->free_slots;

}

/* Add a record to the page
*Return:
*  record slot offset if successful,
*  -1 if unsuccessful (page full)


Page size =  M*slot_size + 2 or 1 (to store M) + M/8 (M bits)
*/
int add_fixed_len_page(Page *page, Record *r){

    /* To-do: should we check slot size fit record r..??*/
    if (fixed_len_sizeof(r) <= page->slot_size){
        int slot_id = find_FreeSlot(page);
        if(slot_id != -1){
            write_fixed_len_page(page, slot_id, r);
            return 0;
        }
    }

    return -1;

}

/* Write a record into a given slot. */ 
void write_fixed_len_page(Page *page, int slot, Record *r){

    unsigned char* next_free_slot = (unsigned char *)page->data + slot * page->slot_size;
    fixed_len_write(r, (void *)next_free_slot);

}

/* Read from a page's slot and store it to Record r. */
void read_fixed_len_page(Page *page, int slot, Record *r){

    unsigned char* record_slot = (unsigned char *)page->data + (page->slot_size * slot);
    // serialize the data at the dataslot and store in r
    fixed_len_read((void *)record_slot, page->slot_size, r);

}



