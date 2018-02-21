#!/bin/bash
make

echo test 
./read_fixed_len_pages page1.txt 1500

echo test 
./read_fixed_len_pages page2.txt 5000
echo test 
./read_fixed_len_pages page3.txt 10000
echo test 
./read_fixed_len_pages page4.txt 40000
echo test 
./read_fixed_len_pages page5.txt 80000
echo test 
./read_fixed_len_pages page6.txt 250000
echo test 
./read_fixed_len_pages page7.txt 750000
echo test 
./read_fixed_len_pages page8.txt 1000000
echo test 
./read_fixed_len_pages page9.txt 1500000
echo test 
./read_fixed_len_pages page10.txt 2000000