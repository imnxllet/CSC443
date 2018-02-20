CC = gcc
CXXFLAGS= -Wall

all: write_fixed_len_pages read_fixed_len_pages csv2heapfile scan insert update delete select csv2colstore select2 select3




write_fixed_len_pages: write_fixed_len_pages.c library.o vector.o
	$(CC) $(CXXFLAGS) -o $@ $< library.o vector.o

read_fixed_len_pages: read_fixed_len_pages.c library.o vector.o
	$(CC) $(CXXFLAGS) -o $@ $< library.o vector.o

csv2heapfile: csv2heapfile.c library.o vector.o
	$(CC) $(CXXFLAGS) -o $@ $< library.o vector.o
insert: insert.c library.o vector.o
	$(CC) $(CXXFLAGS) -o $@ $< library.o vector.o	

scan: scan.c library.o vector.o
	$(CC) $(CXXFLAGS) -o $@ $< library.o vector.o

update: update.c library.o vector.o
	$(CC) $(CXXFLAGS) -o $@ $< library.o vector.o

delete: delete.c library.o vector.o
	$(CC) $(CXXFLAGS) -o $@ $< library.o vector.o

select: select.c library.o vector.o
	$(CC) $(CXXFLAGS) -o $@ $< library.o vector.o
select2: select2.c library.o vector.o
	$(CC) $(CXXFLAGS) -o $@ $< library.o vector.o

select3: select3.c library.o vector.o
	$(CC) $(CXXFLAGS) -o $@ $< library.o vector.o


csv2colstore: csv2colstore.c library.o vector.o
	$(CC) $(CXXFLAGS) -o $@ $< library.o vector.o

%.o : %.c
	gcc -Wall -c -std=c99 $<

clean:
	rm -f *.o *~ core sr *.dump *.tar tags