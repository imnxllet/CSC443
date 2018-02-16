#include <vector>
typedef const char* V;
typedef std::vector<V> record;

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

    for(it = record.begin(); it != record.end(); it++,i++ )    {

        strcat(buf, *it);
    }

}
/**
 * Deserializes `size` bytes from the buffer, `buf`, and
 * stores the data in `record`.
 */
 void fixed_len_read(void *buf, int size, Record *record){
    /* Value is of 10 bytes long(ATTRIBUTE_SIZE) for each attribute. */

    int values_count = size / ATTRIBUTE_SIZE;
    int index = 0;
    for (int i = 0; i < values_count; i++) {
        char value[ATTRIBUTE_SIZE];
        strncpy(value, buf + index, ATTRIBUTE_SIZE);
        value[ATTRIBUTE_SIZE] = '\0';
        index += ATTRIBUTE_SIZE;

        //attribute[ATTRIBUTE_SIZE] = '\0';
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
    page->free_slots = slot_size;
    page->used_slots = 0;

}

/* Calculates the maximal number of records that fit in a page. */
int fixed_len_page_capacity(Page *page){

    return page->page_size / page->slot_size;

}

/* Calculate the free space (number of free slots) in the page */
int fixed_len_page_freelots(Page *page){

    return page->free_slots;

}

/* Add a record to the page
*Return:
*  record slot offset if successful,
*  -1 if unsuccessful (page full)*/
int add_fixed_len_page(Page *page, Record *r){

    int slot = 0;
    std::vector<Record> records = *(page->data);
    for (std::vector<Record>::iterator it = records.begin(); it != records.end(); ++it) {
        if (it->empty()) {
            return slot;
        }
        slot ++;
    }
    return -1;

}

/* Write a record into a given slot. */ 
void write_fixed_len_page(Page *page, int slot, Record *r){

}

/* Read a record from the page from a given shot. */
void read_fixed_len_page(Page *page, int slot, Record *r){

}



