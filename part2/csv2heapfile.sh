#!/bin/bash
make
python mkcsv.py records.txt 1000
echo test 
./csv2heapfile records.txt heap1.txt 1500
echo test 
./csv2heapfile records.txt heap2.txt 5000
echo test 
./csv2heapfile records.txt heap3.txt 10000
echo test 
./csv2heapfile records.txt heap4.txt 40000
echo test 
./csv2heapfile records.txt heap5.txt 80000
echo test 
./csv2heapfile records.txt heap6.txt 250000
echo test 
./csv2heapfile records.txt heap7.txt 750000
echo test 
./csv2heapfile records.txt heap8.txt 1000000
echo test 
./csv2heapfile records.txt heap9.txt 1500000
echo test 
./csv2heapfile records.txt heap10.txt 2000000