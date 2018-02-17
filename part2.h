#include "vector.h"
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