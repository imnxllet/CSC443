#include <vector>

typedef const char* V;
typedef std::vector<V> Record;
typedef struct{
    void *data;
    int page_size;
    int slot_size;
    int free_slots;
    int used_slots;
} Page;


int togglePageBitmap(Page *page, int slot_id, int value);
int find_FreeSlot(Page *page);
int fixed_len_page_capacity(Page *page);
void printBit(unsigned char *byte);
