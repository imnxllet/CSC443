#!/bin/bash
make
python mkcsv.py records.txt 1000
echo test 
./write_fixed_len_pages records.txt page1.txt 1500
echo test 
./write_fixed_len_pages records.txt page2.txt 5000
echo test 
./write_fixed_len_pages records.txt page3.txt 10000
echo test 
./write_fixed_len_pages records.txt page4.txt 40000
echo test 
./write_fixed_len_pages records.txt page5.txt 80000
echo test 
./write_fixed_len_pages records.txt page6.txt 250000
echo test 
./write_fixed_len_pages records.txt page7.txt 750000
echo test 
./write_fixed_len_pages records.txt page8.txt 1000000
echo test 
./write_fixed_len_pages records.txt page9.txt 1500000
echo test 
./write_fixed_len_pages records.txt page10.txt 2000000