CC = gcc
CXXFLAGS= -Wall

all: create_random_file get_histogram




create_random_file: create_random_file.c 
	$(CC) $(CXXFLAGS) -o $@ $< 

get_histogram: get_histogram.c 
	$(CC) $(CXXFLAGS) -o $@ $< 


clean:
	rm -f *.o *~ core sr *.dump *.tar tags