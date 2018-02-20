#!/bin/bash
make

echo test 
./get_histogram random.txt 100
echo test 
./get_histogram random.txt 349616
echo test 
./get_histogram random.txt 699130
echo test 
./get_histogram random.txt 1048644
echo test 
./get_histogram random.txt 1398158
echo test 
./get_histogram random.txt 1747672
echo test 
./get_histogram random.txt 2097186
echo test 
./get_histogram random.txt 2446700 
echo test 
./get_histogram random.txt 2796214 
echo test 
./get_histogram random.txt 3145728