CC = gcc
CXXFLAGS= -Wall -g

all: write_fixed_len_pages read_fixed_len_pages csv2heapfile scan insert




write_fixed_len_pages: part2-write_fixed_len_pages.c part2.o vector.o
	$(CC) $(CXXFLAGS) -o $@ $< part2.o vector.o

read_fixed_len_pages: part2-read_fixed_len_pages.c part2.o vector.o
	$(CC) $(CXXFLAGS) -o $@ $< part2.o vector.o

csv2heapfile: csv2heapfile.c part2.o vector.o
	$(CC) $(CXXFLAGS) -o $@ $< part2.o vector.o
insert: insert.c part2.o vector.o
	$(CC) $(CXXFLAGS) -o $@ $< part2.o vector.o	

scan: scan.c part2.o vector.o
	$(CC) $(CXXFLAGS) -o $@ $< part2.o vector.o

%.o : %.c
	gcc -Wall -c -std=c99 $<

clean:
	rm -f *.o *~ core sr *.dump *.tar tags