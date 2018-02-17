CC=g++
CXXFLAGS=-std=c++11 -Wall -g

all: write_fixed_len_pages



library.o: part2.cpp part2.h
	$(CC) $(CXXFLAGS) -o $@ -c $<


write_fixed_len_pages: part2-write_fixed_len_pages.cpp library.o
	$(CC) $(CXXFLAGS) -o $@ $< library.o

clean:
	rm -f *.o *~ core sr *.dump *.tar tags