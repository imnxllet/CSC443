CC = gcc
CXXFLAGS= -Wall -g

all: write_fixed_len_pages read_fixed_len_pages csv2heapfile scan insert update delete select csv2colstore select2 select3




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

update: update.c part2.o vector.o
	$(CC) $(CXXFLAGS) -o $@ $< part2.o vector.o

delete: delete.c part2.o vector.o
	$(CC) $(CXXFLAGS) -o $@ $< part2.o vector.o

select: select.c part2.o vector.o
	$(CC) $(CXXFLAGS) -o $@ $< part2.o vector.o
select2: select2.c part2.o vector.o
	$(CC) $(CXXFLAGS) -o $@ $< part2.o vector.o

select3: select3.c part2.o vector.o
	$(CC) $(CXXFLAGS) -o $@ $< part2.o vector.o


csv2colstore: csv2colstore.c part2.o vector.o
	$(CC) $(CXXFLAGS) -o $@ $< part2.o vector.o

%.o : %.c
	gcc -Wall -c -std=c99 $<

clean:
	rm -f *.o *~ core sr *.dump *.tar tags