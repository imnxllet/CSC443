#include "vector.h"
#include <stdio.h>

#define ATTRIBUTE_SIZE 10


//typedef const char* V;
typedef Vector Record;
typedef struct{
    void *data;
    int page_size;
    int slot_size;
    int free_slots;
    int used_slots;
    int total_slot;
    int slot_bitmap_size;
} Page;

typedef struct {
	FILE *file_ptr;
	int page_size;
	int directory_num_pages;
} Heapfile;

typedef int PageID;

typedef struct {
	int page_id;
	int slot;
} RecordID;

typedef struct  {
    Heapfile *hf;
    int page_offset; 
    int record_id;
    int* current_directory_page;
    Page* current_page;
    int page_id_in_directory;
    int has_next_directory;
    Record next_record;
} RecordIterator;

void init_fixed_len_page(Page *page, int page_size, int slot_size);
int fixed_len_page_freelots(Page *page);
int add_fixed_len_page(Page *page, Record *r);
void write_fixed_len_page(Page *page, int slot, Record *r);
void read_fixed_len_page(Page *page, int slot, Record *r);
int fixed_len_sizeof(Record *record);
void fixed_len_write(Record *record, void *buf);
void fixed_len_read(void *buf, int size, Record *record);
int togglePageBitmap(Page *page, int slot_id, int value);
int find_FreeSlot(Page *page);
int fixed_len_page_capacity(Page *page);
void printBit(unsigned char *byte);
int checkValue(Page *page, int slot_id);
PageID assignPageID(Heapfile *heapfile, Page *page);
void makeDirectoryPageAndAddpage(Heapfile *heapfile);
PageID alloc_page(Heapfile *heapfile);
void read_page(Heapfile *heapfile, PageID pid, Page *page);
void write_page(Page *page, Heapfile *heapfile, PageID pid);
void init_RecordIterator(RecordIterator* iterator, Heapfile *heapfile);

int getNextUsedPage(RecordIterator* iterator);
Record next(RecordIterator* iterator);
bool hasnext(RecordIterator* iterator);






