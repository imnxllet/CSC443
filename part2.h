#include "vector.h"
#include <stdio.h>

#define ATTRIBUTE_SIZE 10
#define RECORD_REAL_SIZE 1100
#define RECORD_SIZE 1000
#define ATTRIBUTE_NUM 100

//typedef const char* V;
typedef Vector Record;
typedef struct{
	int page_size;
    int slot_size;
    int free_slots;
    int used_slots;
    int total_slot;
    int slot_bitmap_size;
    void *data;

} Page;

typedef struct {
	FILE *file_ptr;
	int page_size;
	int directory_num_pages;
	int d_num;
	int file_size;
	char *filename;
	int slot_size;
	int last_free_page;
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
    Record* next_record;
    int page_num;
    int d_num;
    int total_slot_in_page;


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

void init_heapfile(Heapfile *heapfile, int page_size, FILE *file, int file_size, char *filename);
PageID assignPageID(Heapfile *heapfile, Page *page);
void makeDirectoryPageAndAddpage(Heapfile *heapfile);
PageID alloc_page(Heapfile *heapfile, Page *page);
void read_page(Heapfile *heapfile, PageID pid, Page *page);
void write_page(Page *page, Heapfile *heapfile, PageID pid);
void init_RecordIterator(RecordIterator* iterator, Heapfile *heapfile);
void openToWrite(Heapfile *heapfile);
void openToRead(Heapfile *heapfile);
//PageID alloc_page_insert(Heapfile *heapfile, Page *page);
int getNextUsedPage(RecordIterator* iterator, Page* page,int *directory);
Record next(RecordIterator* iterator);
bool hasnext(RecordIterator* iterator, Page* page, Record *record, int *directory);
//PageID assignPageID_insert(Heapfile *heapfile, Page *page);
//void makeDirectoryPageAndAddpage_insert(Heapfile *heapfile);

PageID alloc_record(Heapfile *heapfile, Record *record);
PageID findPageID(Heapfile *heapfile, Record *record);
int calculate_free(Page *page);
void getIDs(char *page_record, RecordID *record_id);
int min(int a, int b);
