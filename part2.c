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



    return ATTRIBUTE_NUM * ATTRIBUTE_SIZE;
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
        //printf("Value is %.*s\n", ATTRIBUTE_SIZE, (char *)iterator_get(&iterator));
        index += ATTRIBUTE_SIZE;
    }

}
/**
 * Deserializes `size` bytes from the buffer, `buf`, and
 * stores the data in `record`.
 */
 void fixed_len_read(void *buf, int size, Record *record){
    /* Value is of 10 bytes long(ATTRIBUTE_SIZE) for each attribute. */

    //printf("buffer size is %d\n", size);
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

   // if(page->slot_bitmap_size + sizeof(int) + )


    //printf("new page has %d for slot suze\n", page->slot_size);
    //printf("new page has %d free slots\n", page->free_slots);
    //printf("new page has %d bytes reserve for bitmap\n", page->slot_bitmap_size);

}

/* Calculates the maximal number of records that fit in a page. */

int fixed_len_page_capacity(Page *page){
    /*Page size =  M*slot_size + 2 or 1 (to store M) + M/8 (M bits)*/
    //int num_slots_M = (page->page_size - sizeof(int)) / ((1/8) + page->slot_size);


    int num_slots_M = (page->page_size - sizeof(int)) /  page->slot_size;
    int bitmap_byte = num_slots_M / 8 + 1;
    if ((sizeof(int) + bitmap_byte + num_slots_M * page->slot_size) > page->page_size){
        return num_slots_M - 1;
    }
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
    //printf("Record size: %d, slot size %d\n", fixed_len_sizeof(r), page->slot_size);
    if (fixed_len_sizeof(r) <= page->slot_size){
        int slot_id = find_FreeSlot(page);
        printf("Free slot id on this page is: %d\n", slot_id);
        if(slot_id != -1){
            //printf("start writing record...\n");
            write_fixed_len_page(page, slot_id, r);
            return 0;
        }
    }

    return -1;

}

/* Write a record into a given slot. */ 
void write_fixed_len_page(Page *page, int slot, Record *r){

    //printf("start writing record to slot %d...\n", slot);
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
        //printf("pushing record %d\n", i);weird.
       // printf("%.*s, \n", ATTRIBUTE_SIZE, value);
         //value[ATTRIBUTE_SIZE -1] = '\0';
        
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
void init_heapfile(Heapfile *heapfile, int page_size, FILE *file, int file_size, char *filename){

    heapfile->file_ptr = file;
    heapfile->page_size = page_size;
    heapfile->directory_num_pages = 0;
    heapfile->d_num = 0;
    heapfile->file_size = file_size;
    heapfile->filename = filename;
    heapfile->last_free_page = 0;
}


PageID assignPageID(Heapfile *heapfile, Page *page){
    rewind(heapfile->file_ptr);
    int num_of_dslot = heapfile->directory_num_pages;
    int *directory = malloc(heapfile->page_size);

   // int directory[num_of_dslot];
    int next_d_page = 0;


    int found_slot = 0;
    /* Loop over all directory page to find available slot*/
    bzero(directory, heapfile->page_size);
    int d_num = 0;
    while(!found_slot){
        d_num ++;
        //rewind(heapfile->file_ptr);
        fseek(heapfile->file_ptr, next_d_page, SEEK_SET);
        fread(directory, 1, heapfile->page_size, heapfile->file_ptr);
        if(heapfile->d_num < d_num){
            heapfile->d_num ++;
        }
        
       

        for(int i = 0; i < num_of_dslot - 2; i += 2){

            /*Found*/
            if(directory[i] == 0){
                /*Not page store in this offset, store here*/
                printf("Directort Slot id for this page is: %d\n", i / 2 + 1);
                //printf("Offset is %d\n", (i + 1) * (heapfile->page_size));
                //printf("Free slot for the new data page is %d\n", page->free_slots);

                directory[i] = next_d_page + (i / 2 + 1) * (heapfile->page_size);
                directory[i+1] = page->free_slots;

                //rewind(heapfile->file_ptr);
                fseek (heapfile->file_ptr, next_d_page, SEEK_SET);
                fwrite(directory, 1, heapfile->page_size, heapfile->file_ptr);
                fflush(heapfile->file_ptr);
                
                return (next_d_page + (i / 2 + 1) * (heapfile->page_size))/(heapfile->page_size);
            }

        }

        /* Not found in this slot, see go through next directory or make a new one. */
        /* First directory full, check if we have a new */

        if(directory[num_of_dslot - 2] == 0){/*Need to make a new directory page first */
            fseek (heapfile->file_ptr, 0, SEEK_END);
            int size = ftell(heapfile->file_ptr);
            directory[num_of_dslot - 2] = size;
            fseek (heapfile->file_ptr, next_d_page, SEEK_SET);
            fwrite(directory, 1, heapfile->page_size, heapfile->file_ptr);
            fflush(heapfile->file_ptr);
            makeDirectoryPageAndAddpage(heapfile);
            printf("making a new d\n");
            printf("size\n");

        }
        //New one exsit or already made, jump to there.
        
        next_d_page = directory[num_of_dslot - 2];

        
    }

    

    return -1;

}


void makeDirectoryPageAndAddpage(Heapfile *heapfile){
   // heapfile->d_num++;
    
    int *directory = malloc(heapfile->page_size);
    //int directory[num_of_dslot];
    bzero(directory, heapfile->page_size);
    fseek (heapfile->file_ptr, 0, SEEK_END);
    fwrite(directory, 1, heapfile->page_size, heapfile->file_ptr);
    fflush(heapfile->file_ptr);

    int num_of_dslot = ((heapfile->page_size) / sizeof(int)) / 2 * 2;
    if(heapfile->directory_num_pages == 0){
        heapfile->directory_num_pages = num_of_dslot;
    }

}

/**
 * Allocate another page in the heapfile. This grows the file by a page.
 */
PageID alloc_page(Heapfile *heapfile, Page *page) {
    /* If first time allocate, we need to make a new directory page..*/

    if(heapfile->file_ptr == NULL){

        printf("Heapfile file_ptr is NULL!\n");
    }

    /* An empty heap file. make the directory*/
    if (heapfile->file_size == 0){
        heapfile->file_size = 1;
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

    }else{
        printf("The heapfile is built before.\n");
        int num_of_dslot = ((heapfile->page_size) / sizeof(int)) / 2 * 2;
        if(heapfile->directory_num_pages == 0){
            heapfile->directory_num_pages = num_of_dslot;
        }
    }
    /* Allocate a new page to the end of heap file*/
    
    //Page page;
    //init_fixed_len_page(&page, heapfile->page_size, 100 * ATTRIBUTE_SIZE);
    int page_id = assignPageID(heapfile, page);

    return page_id;

}


PageID alloc_record(Heapfile *heapfile, Record *record) {
    /* If first time allocate, we need to make a new directory page..*/

    if(heapfile->file_ptr == NULL){

        printf("Heapfile file_ptr is NULL!\n");
    }

    /* An empty heap file. make the directory*/
    if (heapfile->file_size > 0){
        printf("The heapfile is built before.\n");
        int num_of_dslot = ((heapfile->page_size) / sizeof(int)) / 2 * 2;
        if(heapfile->directory_num_pages == 0){
            heapfile->directory_num_pages = num_of_dslot;
        }
        
    }
    /* Allocate a new page to the end of heap file*/
    
    //Page page;
    //init_fixed_len_page(&page, heapfile->page_size, 100 * ATTRIBUTE_SIZE);
    int page_id = findPageID(heapfile, record);

    return page_id;

}

PageID findPageID(Heapfile *heapfile, Record *record){
    rewind(heapfile->file_ptr);
    int num_of_dslot = heapfile->directory_num_pages;
    int *directory = malloc(heapfile->page_size);

   // int directory[num_of_dslot];
    int next_d_page = 0;


    int found_slot = 0;
    /* Loop over all directory page to find available slot*/
    bzero(directory, heapfile->page_size);
    int d_num = 0;
    while(!found_slot){
        d_num ++;
        //rewind(heapfile->file_ptr);
        fseek(heapfile->file_ptr, next_d_page, SEEK_SET);
        fread(directory, 1, heapfile->page_size, heapfile->file_ptr);
        

        if(heapfile->d_num < d_num){
            heapfile->d_num ++;
        }
        
       

        for(int i = heapfile->last_free_page; i < num_of_dslot - 2; i += 2){

            /*Look at those used page to see if empty spot...*/
            if(directory[i] != 0){
                /*Not page store in this offset, store here*/
                printf("checking.. Page id is: %d\n", i / 2 + 1);


                /* Read from heap and store as a page. */
                Page page;

                init_fixed_len_page(&page, heapfile->page_size, fixed_len_sizeof(record));
                
                if(page.free_slots == 0){
                    printf("The page size is smaller than a record, abort.\n");
                    return -1;
                }    
                
                //bzero(&page, heapfile->page_size);
               
                read_page(heapfile, i / 2 + 1, &page);

                if(page.free_slots == 0){
                    printf("Page is full, check next page.\n");
                    heapfile->last_free_page = i + 2;
                    continue;
                }

                //automatically add to the free slot.
                if(add_fixed_len_page(&page, record) == -1){
                    printf("weird: free slot is : %d\n", page.free_slots);
                    printf("cant find empty slot...\n");

                    page.free_slots = 0;
                    write_page(&page, heapfile, i / 2 + 1);
                    continue;
                }


                /*write the page back to heap file */
                write_page(&page, heapfile, i / 2 + 1);
                
                return (next_d_page + (i / 2 + 1) * (heapfile->page_size))/(heapfile->page_size);
            }else{//No page at this directory slot
                Page page;
                init_fixed_len_page(&page, heapfile->page_size, fixed_len_sizeof(record));
                if(page.free_slots == 0){
                    printf("The page size is smaller than a record, abort.\n");
                    return -1;
                }  

                //automatically add to the free slot.
                add_fixed_len_page(&page, record);
                directory[i] = next_d_page + (i / 2 + 1) * (heapfile->page_size);
                directory[i+1] = page.free_slots;

                //rewind(heapfile->file_ptr);
                fseek (heapfile->file_ptr, next_d_page, SEEK_SET);
                fwrite(directory, 1, heapfile->page_size, heapfile->file_ptr);
                fflush(heapfile->file_ptr);
                
                

                /*write the page back to heap file */
                write_page(&page, heapfile, i / 2 + 1);
                return (next_d_page + (i / 2 + 1) * (heapfile->page_size))/(heapfile->page_size);
            }

        }

        /* Not found in this slot, see go through next directory or make a new one. */
        /* First directory full, check if we have a new */

        if(directory[num_of_dslot - 2] == 0){/*Need to make a new directory page first */
            fseek (heapfile->file_ptr, 0, SEEK_END);
            int size = ftell(heapfile->file_ptr);
            directory[num_of_dslot - 2] = size;
            fseek (heapfile->file_ptr, next_d_page, SEEK_SET);
            fwrite(directory, 1, heapfile->page_size, heapfile->file_ptr);
            fflush(heapfile->file_ptr);
            makeDirectoryPageAndAddpage(heapfile);
            printf("making a new d\n");
            printf("size\n");
            heapfile->last_free_page = 0;

        }
        //New one exsit or already made, jump to there.
        
        next_d_page = directory[num_of_dslot - 2];

        
    }

    

    return -1;

}



/* Read a page into memory. */
void read_page(Heapfile *heapfile, PageID pid, Page *page){
    //rewind(heapfile->file_ptr);
    //fread(directory, sizeof(int), num_of_dslot, heapfile->file_ptr + next_d_page);
    fseek(heapfile->file_ptr, (int)pid * heapfile->page_size, SEEK_SET);
    fread(page->data, heapfile->page_size, 1, heapfile->file_ptr);


    page->slot_size = heapfile->slot_size;
    page->page_size = heapfile->page_size;

    page->total_slot = fixed_len_page_capacity(page);
    //printf("hiii... %d\n", page->total_slot );
    page->free_slots = calculate_free(page);

    page->used_slots = page->total_slot - page->free_slots;

    page->slot_bitmap_size =  (page->total_slot)/8 + ((page->total_slot) % 8 != 0);

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
    //int num_of_dslot = heapfile->directory_num_pages;
    int *directory = malloc(heapfile->page_size);

    //int directory[num_of_dslot];
    rewind(heapfile->file_ptr);
    bzero(directory, heapfile->page_size);
    fread(directory, 1, heapfile->page_size, heapfile->file_ptr);
    iterator->page_id_in_directory = 0;

    /*Current directory*/
    iterator->current_directory_page = directory;

    /*Current page offset*/
    iterator->page_offset = 0;
    iterator->has_next_directory = 1;
    iterator->record_id = 0;
    
    iterator->page_num = 0;
    iterator->d_num = 0;
    //init_fixed_len_page(iterator->current_page, heapfile->page_size, 1000);
    iterator->current_page = NULL;

    
    //Page data_page;
    //init_fixed_len_page(&data_page, heapfile->page_size, 1000);
    //fread(data_page.data, heapfile->page_size, 1, heapfile->file_ptr);
    /* Current Page*/
    //current_page = data_page;
}

int getNextUsedPage(RecordIterator* iterator, Page* page, int *directory){
    /*From the already checked page index, we find the next page in this directory.*/
    Heapfile *heapfile = iterator->hf;

    /* First time, no page*/
    if(iterator->current_page == NULL){
        //printf("Setting up the first directory..\n");
        
        iterator->d_num++;

        //rewind(heapfile->file_ptr);
        
        bzero(directory, (iterator->hf)->page_size);
        rewind(heapfile->file_ptr);
        fread(directory, 1, heapfile->page_size, heapfile->file_ptr);


        iterator->page_id_in_directory = 0;
        heapfile->directory_num_pages =((heapfile->page_size) / sizeof(int)) / 2 * 2;

        /*Update Current directory*/
        iterator->current_directory_page = directory;
        /*for(int j=0; j < heapfile->directory_num_pages - 2;j++){
            printf("Directory #%d: %d\n", j, directory[j]);
        }*/
    }

    for(int i = iterator->page_id_in_directory * 2; i < heapfile->directory_num_pages - 2; i += 2){
        //printf("break #%d\n", i);
        iterator->page_id_in_directory++;

        /*Found*/
        if(iterator->current_directory_page[i] != 0){
            //printf("No #%d\n", i);
            iterator->page_num ++;
            //Page data_page;
            //init_fixed_len_page(&data_page, heapfile->page_size, 1000);
            //rewind((iterator->hf)->file_ptr);

            //fseek(heapfile->file_ptr, iterator->current_directory_page[i], SEEK_SET);
            
            //fread(page->data, (iterator->hf)->page_size, 1, heapfile->file_ptr);

            read_page(iterator->hf, ((i / 2 + 1)) + (iterator->page_offset/heapfile->page_size) ,page);

            //printf("This page has total slot of : %d\n", data_page.total_slot);
            
            //init_fixed_len_page(iterator->current_page, heapfile->page_size, 1000);
            iterator->current_page = page;
            //iterator->total_slot_in_page = (iterator->current_page)->total_slot;
            


            //printf("This page has total slot of : %d\n", (iterator->current_page)->total_slot);
            //printf("Get page id %d\n", ((i / 2 + 1)) + (iterator->page_offset/heapfile->page_size));

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
    
        //int directory[heapfile->directory_num_pages];
        //int *directory = malloc(heapfile->page_size);
        iterator->d_num++;

        //rewind(heapfile->file_ptr);
        //bzero(directory, (iterator->hf)->page_size);
        fseek(heapfile->file_ptr, iterator->current_directory_page[heapfile->directory_num_pages - 2], SEEK_SET);

        printf("@new directory start at %d\n", iterator->current_directory_page[heapfile->directory_num_pages - 2]);
        iterator->page_offset = iterator->current_directory_page[heapfile->directory_num_pages - 2];
        fread(directory, 1, heapfile->page_size, heapfile->file_ptr);
        iterator->page_id_in_directory = 0;

        /*Update Current directory*/
        iterator->current_directory_page = directory;
        iterator->record_id = 0;

        /* A new directory to iterate */
        return 0;  
    }


       
}


Record next(RecordIterator* iterator){

    return *(iterator->next_record);
    



}

bool hasnext(RecordIterator* iterator, Page* page, Record *record, int *directory){
    vector_clear(record);

    //Should move to next page or next directory
    
    //printf("\nStart to find next record..\n");

    int has_next= 0;
    while(has_next == 0){

        /* we need a new page when we first start iterate or the current page is done. */
        //printf("current record_id is %d\n", iterator->record_id);
        if (iterator->current_page == NULL || (iterator->record_id  >= (iterator->current_page)->total_slot)) {
            
            /*if(iterator->current_page == NULL){
                printf("First init\n");
            }else if(iterator->record_id > (iterator->current_page)->total_slot){
                printf("No more record to read in this page..\n");
            }*/


            //printf("Go to next page\n");

            /*1 - has next page, 0 - move to new directory, -1 - No next record*/
            int has_next_page = getNextUsedPage(iterator, page, directory);
            //printf("!!!This page has total slot of : %d\n", (iterator->current_page)->total_slot);
           // printf("Has Nextpage value idicator; %d\n", has_next_page);
            //printf("We are at record id #%d\n", iterator->record_id);
            iterator->record_id = 0;

            if(has_next_page == -1){
                return false;

            /* Check if new directory has page.*/
            }else if(has_next_page == 0){
                //printf("I am here..\n");
                has_next_page = getNextUsedPage(iterator, page, directory);
                /* Assuming a directory that has no available page will not have a linked new directory*/
                if(has_next_page == -1){
                    return false;
                }
            }
            //printf("!!!This page has total slot of : %d\n", (iterator->current_page)->total_slot);
        }

         /* The new page is stored in iterator. */
         /* Check if this page has record to read. */
       // printf("checking record %d\n", iterator->record_id + 1);
        //printf("This page has total slot of : %d\n", (iterator->current_page)->total_slot);

        for(int i = iterator->record_id + 1; i <= (iterator->current_page)->total_slot; i++){
           // printf("checkingggg record %d\n", iterator->record_id + 1);
            /* Has a record in this slot.*/
            iterator->record_id ++;



            /* wrong ...*/
            //read_fixed_len_page((iterator->current_page)->data, i, record);
            if(checkValue(iterator->current_page, i) == 1){
                //Record record;
                //printf("I am here\n");
                
                //printf("\n\nRecordID: %d_%d\n", iterator->page_num, iterator->record_id);
                read_fixed_len_page(iterator->current_page, i, record);

                 //printf("checking record %d\n", iterator->record_id + 1) .;
                iterator->next_record = record;

                //iterator->current_record_slot += 1;
                

                has_next = 1;
                return true;






            }
            
        }
        printf("No next record found...\n");
        return false;
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
    int num_slots = page->total_slot;
    //printf("This page has %d slots\n", num_slots);

    int counter = 0;
    //unsigned char *inode_bitmap = index(disk, group->bg_inode_bitmap);
    //printf("\nInode bitmap:");
    for (int i =0; i < (page->slot_bitmap_size); i++){
        //printf("checking byte...\n");
        unsigned char *byte = slot_bitmap + i;
        //printBit(byte);
        
        for(int j = 0; j < min(num_slots, 8); j++){
            //printf("checking bit...\n");
            counter++;
            if(counter > num_slots){
                return -1;
            }
            printf("num slot is %d\n", counter);
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
        int num_slots = page->total_slot;
        int counter = 0;
        unsigned char *page_bitmap = (unsigned char *)page->data + sizeof(int);

        //printf("\nInode bitmap:");
        for (int i =0; i < (page->slot_bitmap_size); i++){
            unsigned char *byte = page_bitmap + i;
            for(int j = 0; j < min(num_slots, 8); j++){
                counter++;
                if(counter > num_slots){
                    return -1;
                }             
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
int calculate_free(Page *page){
    unsigned char *slot_bitmap = (unsigned char *)page->data + sizeof(int);
    int num_slots = page->total_slot;
    //printf("This page has %d slots\n", num_slots);

    int counter = 0;
    //unsigned char *inode_bitmap = index(disk, group->bg_inode_bitmap);
    //printf("\nInode bitmap:");
    for (int i =0; i < page->slot_bitmap_size; i++){
        //printf("checking byte...\n");
        unsigned char *byte = slot_bitmap + i;
        //printBit(byte);
        for(int j = 0; j < min(num_slots, 8); j++){
            //printf("checking bit...\n");
            counter++;
            if(counter > num_slots){
                return -1;
            }
            int bit = *byte & 1 << j; //check bit 1 or 0
            if(bit){
                continue;
            }else{
                counter++;
            }   
        }
    }
    //No Free Inode found.
    return counter;
}

/*Update inode with id in inode bitmap to value. Adjust sb,group descriptor accordingly.*/
int togglePageBitmap(Page *page, int slot_id, int value){
    //Toggle to 1

        //struct ext2_group_desc *group = (struct ext2_group_desc *)(disk + 2048);
        int num_slots = page->total_slot;
        int counter = 0;
        unsigned char *page_bitmap = (unsigned char *)page->data + sizeof(int);

        //printf("\nInode bitmap:");

        for (int i =0; i < page->slot_bitmap_size; i++){
            unsigned char *byte = page_bitmap + i;
            for(int j = 0; j < min(num_slots, 8); j++){
                counter++;
                if(counter > num_slots){
                    return -1;
                }
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

void getIDs(char *page_record, RecordID *record_id){
    int len = strlen(page_record);
    char page_id[len];
    char slot[len];
    int found_page = 0;
    for(int i =0; i < len; i++){
        if(page_record[i] != '_' && found_page == 0){

            page_id[i] = page_record[i];

        }else if(page_record[i] != '_' && found_page != 0){

            slot[i - found_page - 1] = page_record[i];
        }else{

            found_page = i;
            page_id[i] = '\0';
        }

    }
    slot[len - found_page - 1] = '\0';

    record_id->page_id = atoi(page_id); 
    record_id->slot = atoi(slot); 

}

int min(int a, int b){
    if(a < b){
        return a;
    }else{
        return b;
    }
}


int select3(char *colstore_file, char *attribute_id, char *start, char *end, char *pagesize, int *result, int mode){

    //const char *heap_filename = argv[1];
   // const char *start = argv[3];
   // const char *end = argv[4];
    //int attribute_id = attribute_id;
    
    //printf("strlen.. %d\n", strlen(argv[3]));
    //char start[strlen(start)];
    //memcpy(start, start, strlen(start));
    //char end[strlen(argv[4])];
    //memcpy(end, end, strlen(end));
   /* if(strlen(argv[4]) != ATTRIBUTE_SIZE){
        printf("new value is not 10 bytes\n");
        return -1;
    }


*/      
    int page_size = atoi(pagesize);


    if(atoi(attribute_id) > 99 || atoi(attribute_id) <0){
        printf("Invalid attribute_id\n");
        return -1;
    }

    
    //const char *new_value = argv[4];
    //int page_size = page_size; 

    // Open the page file for writing
    FILE *fp_read_heapfile;
    fp_read_heapfile = fopen(colstore_file, "r+");
    if(fp_read_heapfile == NULL){
        printf("Error in file opening\n");
    }
    //fseek(fp_read_heapfile, 0, SEEK_END);

    //int result[ftell(fp_read_heapfile)];
    //rewind(fp_read_heapfile);


    int records_num = 0;
    //int pages_num = 0;

    // start timer
    struct timeb t;
    ftime(&t);
    unsigned long start_ms = t.time * 1000 + t.millitm;
    //char buf[page_size];

    /* Initialize the heapfile*/
    Heapfile heapfile;
    printf("colstore_file is %s\n", colstore_file);
    init_heapfile(&heapfile, page_size, fp_read_heapfile, 0, (char *)colstore_file);
    
    Page page;
    init_fixed_len_page(&page, heapfile.page_size, 1000);
    Record record;
    vector_setup(&record, 100, 10*sizeof(char));

    RecordIterator record_iterator;
    heapfile.slot_size = 1000;
    //printf("!!!!!%d\n", heapfile.slot_size);
    init_RecordIterator(&record_iterator, &heapfile);

    int *directory = malloc(page_size);

    //RecordID recordID;
    //getIDs((char *)record_id,  &recordID);
    //printf("page %d_ slot %d\n", recordID.page_id, recordID.slot);

    int slot = 0;
    int result_index = 0;

    int to_check = 0;
    while(hasnext(&record_iterator, &page, &record, directory) == true){
        records_num++;

        
        record = next(&record_iterator);
        Iterator iterator = vector_begin(&record);
        Iterator last = vector_end(&record);
        //printf("has next\n");
        for (; !iterator_equals(&iterator, &last); iterator_increment(&iterator)) {
            // *(int*)iterator_get(record) += 1;
            //memcpy((char*)buf + index, (char *)iterator_get(&iterator), ATTRIBUTE_SIZE);
            //printf("Value is %.*s\n", ATTRIBUTE_SIZE, (char *)iterator_get(&iterator));
            char substring[5];
            slot++;
            memcpy(substring, (char *)iterator_get(&iterator) + 1, 5);
            //printf("substring is %.*s, ", 5, substring);

            if(mode == 1){
                if(slot == result[to_check]){
                    printf("\n\nSlotID: %d\n", slot);
                    printf("Extracted value-> %.*s\n", 5 ,substring);
                    to_check++;
                }
            }
            
            if(mode == 0){
                //if(strncmp(substring, start, 5) >= 0 && strncmp(substring, end, 5) <= 0){
                if(strncmp(substring, start, 5) >= 0 && strncmp(substring, end, 5) <= 0){
                    //printf("\n\nRecordID: %d_%d\n", record_iterator.page_num, record_iterator.record_id);
                   // printf("\n\nSlotID: %d\n", slot);
                   // printf("Extracted value.. %.*s\n", 5 ,substring);
                  //printf("start is %.*s, ", 5, start);
                  //printf("start cmp = %d\n", cmpstr(substring, start, 5) >= 0);
                  // printf("end cmp = %d\n", cmpstr(substring, end, 5) <= 0);
          // printf("end is %.*s, ", 5, end);
                    result[result_index] = slot;
                    result_index++;

                }
            }
        }




        

        
    }





    fclose(fp_read_heapfile);

  


    // stop timer
    ftime(&t);
    unsigned long stop_ms = t.time * 1000 + t.millitm;


    //printf("\n\nNUMBER OF RECORDS: %d\n", records_num);
    //printf("NUMBER OF PAGES:: %d\n", record_iterator.page_num);
    //printf("NUMBER OF DIRECTORIES:: %d\n", record_iterator.d_num);
    printf("Time used to write the file: %lums.\n", stop_ms - start_ms);


    return 0;
}


int cmpstr(char *str1, char *str2, int size){
    int sum1 = 0;
    int sum2 = 0;

    for(int i = 0; i < size; i++){
        sum1 += (int)str1[i];
        //printf("%c\n", str1[i]);
    }

    for(int i = 0; i < size; i++){
        sum2 += (int)str2[i];
        //printf("%c\n", str2[i]);
    }

    if(sum1 > sum2){
        return 1;
    }else if(sum1 < sum2){
        return -1;
    }else{
        return 0;
    }
}


/* Insert the page to existing heapfile.*/
/*PageID alloc_page_insert(Heapfile *heapfile, Page *page) {
    // If first time allocate, we need to make a new directory page..

    if(heapfile->file_ptr == NULL){

        printf("Heapfile file_ptr is NULL!\n");
    }

    // An empty heap file. make the directory
    if (heapfile->file_size == 0){
      // print your error message here
        printf("The heap file needs a directory page before adding a data page..\n");
        ///int num_of_dslot = (heapfile->page_size) / sizeof(int);
        //int directory[num_of_dslot];
        //bzero(directory, heapfile->page_size);
        //fwrite(directory, sizeof(int), num_of_dslot, heapfile->file_ptr);
        //fflush(heapfile->file_ptr);
        //heapfile->directory_num_pages = num_of_dslot;
        rewind(heapfile->file_ptr);
        makeDirectoryPageAndAddpage(heapfile);
    }else{
        printf("The heapfile is built before.\n");
        int num_of_dslot = ((heapfile->page_size) / sizeof(int)) / 2 * 2;
        if(heapfile->directory_num_pages == 0){
            heapfile->directory_num_pages = num_of_dslot;
        }
    }
    // Allocate a new page to the end of heap file
    
    //Page page;
    //init_fixed_len_page(&page, heapfile->page_size, 100 * ATTRIBUTE_SIZE);
    int page_id = assignPageID_insert(heapfile, page);

    return page_id;

}*/

/*void openToWrite(Heapfile *heapfile){
    fclose(heapfile->file_ptr);
    heapfile->file_ptr = fopen(heapfile->filename, "r+");

}

void openToRead(Heapfile *heapfile){
    fclose(heapfile->file_ptr);
    heapfile->file_ptr = fopen(heapfile->filename, "r");

}*/
/*
PageID assignPageID_insert(Heapfile *heapfile, Page *page){
    rewind(heapfile->file_ptr);
    int num_of_dslot = heapfile->directory_num_pages;
    int *directory = malloc(heapfile->page_size);

   // int directory[num_of_dslot];
    int next_d_page = 0;


    int found_slot = 0;
    // Loop over all directory page to find available slot
    bzero(directory, heapfile->page_size);
    
    while(!found_slot){
        //rewind(heapfile->file_ptr);
        
        openToRead(heapfile);

        fseek(heapfile->file_ptr, next_d_page, SEEK_SET);
        fread(directory, 1, heapfile->page_size, heapfile->file_ptr);

            for(int j=0; j < (heapfile->page_size / sizeof(int)) / 2 * 2;j++){
        printf("Directory #%d: %d\n", j, directory[j]);
    }
       

        for(int i = 0; i < num_of_dslot - 2; i += 2){

            //Found
            if(directory[i] == 0){
                //Not page store in this offset, store here
                printf("Directort Slot id for this page is: %d\n", i / 2 + 1);
                //printf("Offset is %d\n", (i + 1) * (heapfile->page_size));
                //printf("Free slot for the new data page is %d\n", page->free_slots);

                directory[i] = next_d_page + (i / 2 + 1) * (heapfile->page_size);
                directory[i+1] = page->free_slots;

                //rewind(heapfile->file_ptr);
                openToWrite(heapfile);
                fseek (heapfile->file_ptr, next_d_page, SEEK_SET);
                fwrite(directory, 1, heapfile->page_size, heapfile->file_ptr);
                fflush(heapfile->file_ptr);
                
                return (next_d_page + (i / 2 + 1) * (heapfile->page_size))/(heapfile->page_size);
            }

        }

        // Not found in this slot, see go through next directory or make a new one. 
        // First directory full, check if we have a new 

        if(directory[num_of_dslot - 2] == 0){//Need to make a new directory page first 
            //fseek (heapfile->file_ptr, 0, SEEK_END);
            int size = ftell(heapfile->file_ptr);
            directory[num_of_dslot - 2] = size;
            openToWrite(heapfile);
            fseek (heapfile->file_ptr, next_d_page, SEEK_SET);
            fwrite(directory, 1, heapfile->page_size, heapfile->file_ptr);
            fflush(heapfile->file_ptr);
            makeDirectoryPageAndAddpage(heapfile);
            printf("making a new d\n");
            printf("size\n");

        }
        //New one exsit or already made, jump to there.
        
        next_d_page = directory[num_of_dslot - 2];

        
    }

    

    return -1;

}
void makeDirectoryPageAndAddpage_insert(Heapfile *heapfile){
    heapfile->d_num++;
    
    int *directory = malloc(heapfile->page_size);
    //int directory[num_of_dslot];
    bzero(directory, heapfile->page_size);
    openToWrite(heapfile);
    fseek (heapfile->file_ptr, 0, SEEK_END);
    fwrite(directory, 1, heapfile->page_size, heapfile->file_ptr);
    fflush(heapfile->file_ptr);

    int num_of_dslot = ((heapfile->page_size) / sizeof(int)) / 2 * 2;
    if(heapfile->directory_num_pages == 0){
        heapfile->directory_num_pages = num_of_dslot;
    }
}*/