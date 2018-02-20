#!/bin/bash
make

echo test 
./create_random_file random.txt 10000000 100
echo test 
./create_random_file random.txt 10000000 349616
echo test 
./create_random_file random.txt 10000000 699130
echo test 
./create_random_file random.txt 10000000 1048644
echo test 
./create_random_file random.txt 10000000 1398158
echo test 
./create_random_file random.txt 10000000 1747672
echo test 
./create_random_file random.txt 10000000 2097186
echo test 
./create_random_file random.txt 10000000 2446700 
echo test 
./create_random_file random.txt 10000000 2796214 
echo test 
./create_random_file random.txt 10000000 3145728