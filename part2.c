#include "part2.h"
#include <time.h>
#include <sys/timeb.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "vector.h"
#include <strings.h>


/*use open source vector for C https://github.com/goldsborough/vector */
/* Compute the number of bytes required to serialize record.*/
int fixed_len_sizeof(Record *record){
    /* number of items inside the vector */

    /*Record::iterator it;  // declare an iterator to a vector of strings
    int size = 0;
    int i = 0;

    for(it = record->begin(); it != record->end(); it++)    {

        size += sizeof(char) * strlen((char *)*it);
        printf("This value has strlen %d\n", strlen((char *) *it));
        printf("Get valuye %s\n", (char *)*it);
        i++;
    }

    printf("This record has %d attributes...\n", record->size());*/
    int sum = 0;
    Iterator iterator = vector_begin(record);
    Iterator last = vector_end(record);
    for (; !iterator_equals(&iterator, &last); iterator_increment(&iterator)) {
        sum++;
    }


    return sum * ATTRIBUTE_SIZE;
    //return size;
}

/*Serialize the record to a byte array to be stored in buf*/
void fixed_len_write(Record *record, void *buf){
/* Write the bytes in the buf */    
    /*Record::iterator it;  // declare an iterator to a vector of strings
    //int size = 0;
    int index = 0;
    for(it = record->begin(); it != record->end(); it++)    {

        memcpy((char*)buf + index, *it, ATTRIBUTE_SIZE);
        index += ATTRIBUTE_SIZE;
    }*/
    int index = 0;
    Iterator iterator = vector_begin(record);
    Iterator last = vector_end(record);
    for (; !iterator_equals(&iterator, &last); iterator_increment(&iterator)) {
        //*(int*)iterator_get(record) += 1;
        memcpy((char*)buf + index, (char *)iterator_get(&iterator), ATTRIBUTE_SIZE);
        printf("Value is %.*s\n", ATTRIBUTE_SIZE, (char *)iterator_get(&iterator));
        index += ATTRIBUTE_SIZE;
    }

}
/**
 * Deserializes `size` bytes from the buffer, `buf`, and
 * stores the data in `record`.
 */
 void fixed_len_read(void *buf, int size, Record *record){
    /* Value is of 10 bytes long(ATTRIBUTE_SIZE) for each attribute. */

    printf("buffer size is %d\n", size);
    int values_count = size / (ATTRIBUTE_SIZE + 1); //=100
    //printf("The buf has %d attributes.\n", values_count);
    int index = 0;
    for (int i = 0; i < values_count; i++) {
        char value[ATTRIBUTE_SIZE];
        //memcpy(value, (char*)buf + index, (ATTRIBUTE_SIZE + 1));
        memcpy(value, (char*)buf + index, (ATTRIBUTE_SIZE));
        //value[ATTRIBUTE_SIZE] = '\0';
        //printf("Get valuye %s\n", value);
        index += ATTRIBUTE_SIZE + 1;

        //attribute[ATTRIBUTE_SIZE] = '\0';
        //printf("pushing record %d\n", i);
        vector_push_back(record, value);
        //record->push_back(value);
        //printf("Value is %.*s\n", ATTRIBUTE_SIZE, value);


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
    page->total_slot = fixed_len_page_capacity(page);
    page->free_slots = fixed_len_page_capacity(page);
    page->used_slots = 0;
    page->data = malloc(page_size);

    page->slot_bitmap_size =  (page->total_slot)/8 + ((page->total_slot) % 8 != 0);
    memset ((char*)page->data, 0, page_size);
    memset((int*)page->data, page->free_slots, sizeof(int));


    printf("new page has %d for slot suze\n", page->slot_size);
    printf("new page has %d free slots\n", page->free_slots);
    printf("new page has %d bytes reserve for bitmap\n", page->slot_bitmap_size);

}

/* Calculates the maximal number of records that fit in a page. */

int fixed_len_page_capacity(Page *page){
    /*Page size =  M*slot_size + 2 or 1 (to store M) + M/8 (M bits)*/
    int num_slots_M = (page->page_size - sizeof(int)) / ((1/8) + page->slot_size);
    return num_slots_M;

}

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
    printf("Record size: %d, slot size %d\n", fixed_len_sizeof(r), page->slot_size);
    if (fixed_len_sizeof(r) <= page->slot_size){
        int slot_id = find_FreeSlot(page);
        printf("Free slot id is: %d\n", slot_id);
        if(slot_id != -1){
            printf("start writing record...\n");
            write_fixed_len_page(page, slot_id, r);
            return 0;
        }
    }

    return -1;

}

/* Write a record into a given slot. */ 
void write_fixed_len_page(Page *page, int slot, Record *r){
    printf("start writing record#2...\n");
    char* next_free_slot = (char *)((char *)page->data + sizeof(int) + page->slot_bitmap_size + ((slot - 1) * page->slot_size));
    togglePageBitmap(page, slot, 1);
    fixed_len_write(r, (void *)next_free_slot);

}

/* Read from a page's slot and store it to Record r. */
void read_fixed_len_page(Page *page, int slot, Record *r){

    char* record_slot = (char *)((char *)page->data + sizeof(int) + page->slot_bitmap_size + ((slot - 1) * page->slot_size));
    // serialize the data at the dataslot and store in r
    //fixed_len_read((void *)record_slot, page->slot_size, r);

    int values_count = page->slot_size / (ATTRIBUTE_SIZE); //=100
    //printf("The buf has %d attributes.\n", values_count);
    int index = 0;
    for (int i = 0; i < values_count; i++) {
        char value[ATTRIBUTE_SIZE];
        //memcpy(value, (char*)buf + index, (ATTRIBUTE_SIZE + 1));
        memcpy(value, (char*)record_slot + index, ATTRIBUTE_SIZE);
        //value[ATTRIBUTE_SIZE] = '\0';
        //printf("Get valuye %s\n", value);
        index += ATTRIBUTE_SIZE;

        //
        //printf("pushing record %d\n", i);
         printf("Value is %.*s\n", ATTRIBUTE_SIZE, value);
         value[ATTRIBUTE_SIZE -1] = '\0';
        
        //r->push_back(value);
        vector_push_back(r, value);
       


        /*if (strlen(value) > 0) {
            record->push_back(attribute);
        }*/
    }

}

/**
 * Initialize a heapfile to use the file and page size given.
 typedef struct {
    FILE *file_ptr;
    int page_size;
} Heapfile;


1- A directory that holds page offsets and number of empty slots in each page. 
If the number of data pages becomes high and your directory requires more than 
one page then you need to create another directory page and store the offset of 
that page in the previous directory page (linked-list)

 

2- Data pages are fixed-length record data pages. Each data page has its own 
directory of slots (as discussed in the lecture). In this directory every bit 
belongs to a slot and shows if the slot is empty or not. You also store the 
number of slots (M) in the beginning or the end of this directory too so you 
know how many bits you need to read.
 */
void init_heapfile(Heapfile *heapfile, int page_size, FILE *file){

    heapfile->file_ptr = file;
    heapfile->page_size = page_size;
    heapfile->directory_num_pages = 0;
}

PageID assignPageID(Heapfile *heapfile, Page *page){
    rewind(heapfile->file_ptr);
    int num_of_dslot = heapfile->directory_num_pages;
    int directory[num_of_dslot];
    int next_d_page = 0;


    int found_slot = 0;
    /* Loop over all directory page to find available slot*/
    while(!found_slot){
        //rewind(heapfile->file_ptr);
        bzero(directory, heapfile->page_size);
        fseek(heapfile->file_ptr, next_d_page, SEEK_SET);
        fread(directory, sizeof(int), num_of_dslot, heapfile->file_ptr);

        for(int i = 0; i < num_of_dslot - 2; i += 2){

            /*Found*/
            if(directory[i] == 0){
                /*Not page store in this offset, store here*/
                printf("To allocate the page at %d\n", i + 1);
                printf("Offset is %d\n", (i + 1) * (heapfile->page_size));
                printf("Free slot for the new data page is %d\n", page->free_slots);

                directory[i] = next_d_page + (i + 1) * (heapfile->page_size);
                directory[i+1] = page->free_slots;

                rewind(heapfile->file_ptr);
                fwrite(directory, sizeof(int), num_of_dslot, heapfile->file_ptr);
                fflush(heapfile->file_ptr);
                
                return (next_d_page + (i + 1) * (heapfile->page_size))/(heapfile->page_size);
            }

        }

        /* Not found in this slot, see go through next directory or make a new one. */
        /* First directory full, check if we have a new */
        if(directory[num_of_dslot - 2] == 0){/*Need to make a new directory page first */
            fseek (heapfile->file_ptr, 0, SEEK_END);
            int size = ftell(heapfile->file_ptr);
            directory[num_of_dslot - 2] = size;
            makeDirectoryPageAndAddpage(heapfile);

        }
        //New one exsit or already made, jump to there.

        next_d_page = directory[num_of_dslot - 2];

        
    }

    

    return -1;

}


void makeDirectoryPageAndAddpage(Heapfile *heapfile){
    int num_of_dslot = (heapfile->page_size) / sizeof(int);
    int directory[num_of_dslot];
    bzero(directory, heapfile->page_size);

    fwrite(directory, sizeof(int), num_of_dslot, heapfile->file_ptr);
    fflush(heapfile->file_ptr);
    heapfile->directory_num_pages = num_of_dslot;
}

/**
 * Allocate another page in the heapfile. This grows the file by a page.
 */
PageID alloc_page(Heapfile *heapfile) {
    /* If first time allocate, we need to make a new directory page..*/
    int size = 0;
    if(heapfile->file_ptr != NULL){
        fseek (heapfile->file_ptr, 0, SEEK_END);

        size = ftell(heapfile->file_ptr);
        rewind(heapfile->file_ptr);

    }else{
        printf("Heapfile file_ptr is NULL!\n");
    }

    /* An empty heap file. */
    if (size == 0){
      // print your error message here
        printf("The heap file needs a directory page before adding a data page..\n");
        /*int num_of_dslot = (heapfile->page_size) / sizeof(int);
        int directory[num_of_dslot];
        bzero(directory, heapfile->page_size);
        fwrite(directory, sizeof(int), num_of_dslot, heapfile->file_ptr);
        fflush(heapfile->file_ptr);
        heapfile->directory_num_pages = num_of_dslot;*/
        rewind(heapfile->file_ptr);
        makeDirectoryPageAndAddpage(heapfile);
    }
    /* Allocate a new page to the end of heap file*/
    
    Page page;
    init_fixed_len_page(&page, heapfile->page_size, 100 * ATTRIBUTE_SIZE);
    int page_id = assignPageID(heapfile, &page);

    return page_id;

}


/* Read a page into memory. */
void read_page(Heapfile *heapfile, PageID pid, Page *page){
    //rewind(heapfile->file_ptr);
    //fread(directory, sizeof(int), num_of_dslot, heapfile->file_ptr + next_d_page);
    fseek(heapfile->file_ptr, (int)pid * heapfile->page_size, SEEK_SET);
    fread(page->data, heapfile->page_size, 1, heapfile->file_ptr);
}

/**
 * Write a page from memory to disk
 * TODO: do we use freespace in the data entries for anything?
 */
void write_page(Page *page, Heapfile *heapfile, PageID pid){
    //rewind(heapfile->file_ptr);
    //fread(directory, sizeof(int), num_of_dslot, heapfile->file_ptr + next_d_page);
    fseek(heapfile->file_ptr, (int)pid * heapfile->page_size, SEEK_SET);
    fwrite(page->data, heapfile->page_size, 1, heapfile->file_ptr);
    fflush(heapfile->file_ptr);
}


/*
class RecordIterator {
    public:
    RecordIterator(Heapfile *heapfile);
    Record next();
    bool hasNext();
    Heapfile *hf;
    Page* current_page;
    int page_offset; 
    int record_id;
    int* directory_page;
    int page_id_in_directory
}*/




void init_RecordIterator(RecordIterator* iterator, Heapfile *heapfile){
    iterator->hf = heapfile;

    /*Number of pages offset in a directory*/
    int num_of_dslot = heapfile->directory_num_pages;
    int directory[num_of_dslot];
    rewind(heapfile->file_ptr);
    bzero(directory, heapfile->page_size);
    fread(directory, sizeof(int), num_of_dslot, heapfile->file_ptr);
    iterator->page_id_in_directory = 0;

    /*Current directory*/
    iterator->current_directory_page = directory;

    /*Current page offset*/
    iterator->page_offset = directory[0];
    iterator->has_next_directory = 1;
    iterator->record_id = 0;
    iterator->current_page = NULL;



    
    //Page data_page;
    //init_fixed_len_page(&data_page, heapfile->page_size, 1000);
    //fread(data_page.data, heapfile->page_size, 1, heapfile->file_ptr);
    /* Current Page*/
    //current_page = data_page;
}

int getNextUsedPage(RecordIterator* iterator){
    /*From the already checked page index, we find the next page in this directory.*/
    Heapfile *heapfile = iterator->hf;
    for(int i = iterator->page_id_in_directory * 2; i < heapfile->directory_num_pages - 2; i += 2){
        iterator->page_id_in_directory++;

        /*Found*/
        if(iterator->current_directory_page[i] != 0){
            Page data_page;
            init_fixed_len_page(&data_page, heapfile->page_size, 1000);
            //rewind((iterator->hf)->file_ptr);

            fseek(heapfile->file_ptr, iterator->current_directory_page[i], SEEK_SET);
            fread(data_page.data, (iterator->hf)->page_size, 1, heapfile->file_ptr);

            iterator->current_page = &data_page;

            /* Sucessfully find a page.*/
            return 1;
        }

    }

    /* We didn't find any more page in this directory.*/
    /* Check if we do have a next directory to check*/
    if(iterator->current_directory_page[heapfile->directory_num_pages - 2] == 0){
        iterator->has_next_directory = 0;

        /* No more to iterate */
        return -1;
    }else{
    /* Has a new directory to check..*/  
    
        int directory[heapfile->directory_num_pages];
        //rewind(heapfile->file_ptr);
        bzero(directory, (iterator->hf)->page_size);
        fseek(heapfile->file_ptr, iterator->current_directory_page[heapfile->directory_num_pages - 2], SEEK_SET);

        fread(directory, sizeof(int), heapfile->directory_num_pages, heapfile->file_ptr);
        iterator->page_id_in_directory = 0;

        /*Update Current directory*/
        iterator->current_directory_page = directory;

        /* A new directory to iterate */
        return 0;  
    }


       
}


Record next(RecordIterator* iterator){

    return iterator->next_record;
    



}

bool hasnext(RecordIterator* iterator){

    //Should move to next page or next directory
    while(1){
        /* we need a new page when we first start iterate or the current page is done. */
        if (iterator->current_page == NULL || (iterator->record_id > (iterator->current_page)->total_slot)) {

            /*1 - has next page, 0 - move to new directory, -1 - No next record*/
            int has_next_page = getNextUsedPage(iterator);
            iterator->record_id = 0;

            if(has_next_page == -1){
                return false;

            /* Check if new directory has page.*/
            }else if(has_next_page == 0){
                has_next_page = getNextUsedPage(iterator);
                /* Assuming a directory that has no available page will not have a linked new directory*/
                if(has_next_page == -1){
                    return false;
                }
            }
        }

         /* The new page is stored in iterator. */
         /* Check if this page has record to read. */
        for(int i = iterator->record_id + 1; i <= (iterator->current_page)->total_slot; i++){
            /* Has a record in this slot.*/
            if(checkValue(iterator->current_page, i)){

                Record record;
                read_fixed_len_page((iterator->current_page)->data, i, &record);

                iterator->next_record = record;

                //iterator->current_record_slot += 1;
                iterator->record_id ++;

                return true;

            }
        }
    }

    return false;
    
    
}



/*Helper function for exercise to print bitmap.*/
void printBit(unsigned char *byte){
    printf(" ");
    for(int i = 0; i < 8; i++){
        int bit = *byte & 1 << i; //check bit 1 or 0
        if(bit) printf("1"); 
        else printf("0");
    }     
}




/*Find next free node from bitmap*/
int find_FreeSlot(Page *page){
    unsigned char *slot_bitmap = (unsigned char *)page->data + sizeof(int);
    int num_slots = fixed_len_page_capacity(page);
    printf("# of slots is: %data\n", num_slots);

    int counter = 0;
    //unsigned char *inode_bitmap = index(disk, group->bg_inode_bitmap);
    //printf("\nInode bitmap:");
    for (int i =0; i < ((float)num_slots / (float)8); i++){
        printf("checking byte...\n");
        unsigned char *byte = slot_bitmap + i;
        printBit(byte);
        for(int j = 0; j < 8; j++){
            //printf("checking bit...\n");
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

int checkValue(Page *page, int slot_id){
    //Toggle to 1

        //struct ext2_group_desc *group = (struct ext2_group_desc *)(disk + 2048);
        //int num_slots = fixed_len_page_capacity(page);
        int counter = 0;
        unsigned char *page_bitmap = (unsigned char *)page->data + sizeof(int);

        //printf("\nInode bitmap:");
        for (int i =0; i < (float)page->total_slot / (float)8; i++){
            unsigned char *byte = page_bitmap + i;
            for(int j = 0; j < 8; j++){
                counter++;             
                if(counter == slot_id){
                    int bit = *byte & 1 << j;
                    if(bit){//Set high
                        return 1;
                    }else{
                        return 0;
                    }
                }
               
            }
        }
        //error
        return -1;
}


/*Update inode with id in inode bitmap to value. Adjust sb,group descriptor accordingly.*/
int togglePageBitmap(Page *page, int slot_id, int value){
    //Toggle to 1

        //struct ext2_group_desc *group = (struct ext2_group_desc *)(disk + 2048);
        //int num_slots = fixed_len_page_capacity(page);
        int counter = 0;
        unsigned char *page_bitmap = (unsigned char *)page->data + sizeof(int);

        //printf("\nInode bitmap:");
        for (int i =0; i < (float)page->total_slot / (float)8; i++){
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


