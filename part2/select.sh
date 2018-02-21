#!/bin/bash
make
python mkcsv.py records.txt 1000
echo test-csv2heapfile 
./csv2heapfile records.txt heap1.txt 1500
echo test-csv2heapfile 
./csv2heapfile records.txt heap2.txt 5000
echo test-csv2heapfile 
./csv2heapfile records.txt heap3.txt 10000
echo test-csv2heapfile  
./csv2heapfile records.txt heap4.txt 40000
echo test-csv2heapfile 
./csv2heapfile records.txt heap5.txt 80000
echo test-csv2heapfile 
./csv2heapfile records.txt heap6.txt 250000
echo test-csv2heapfile  
./csv2heapfile records.txt heap7.txt 750000
echo test-csv2heapfile 
./csv2heapfile records.txt heap8.txt 1000000
echo test-csv2heapfile 
./csv2heapfile records.txt heap9.txt 1500000
echo test-csv2heapfile 
./csv2heapfile records.txt heap10.txt 2000000

echo test-select
./select heap1.txt 0 AAAAA ZZZZZ 1500
echo test-select
./select heap2.txt 0 AAAAA ZZZZZ 5000
echo test-select 
./select heap3.txt 0 AAAAA ZZZZZ 10000
echo test-select 
./select heap4.txt 0 AAAAA ZZZZZ 40000
echo test-select 
./select heap5.txt 0 AAAAA ZZZZZ 80000
echo test-select 
./select heap6.txt 0 AAAAA ZZZZZ 250000
echo test-select 
./select heap7.txt 0 AAAAA ZZZZZ 750000
echo test-select 
./select heap8.txt 0 AAAAA ZZZZZ 1000000
echo test-select 
./select heap9.txt 0 AAAAA ZZZZZ 1500000
echo test-select 
./select heap10.txt 0 AAAAA ZZZZZ 2000000

echo test-select
./select heap1.txt 0 KIDWU KIDWU 1500
echo test-select
./select heap2.txt 0 KIDWU KIDWU 5000
echo test-select 
./select heap3.txt 0 KIDWU KIDWU 10000
echo test-select 
./select heap4.txt 0 KIDWU KIDWU 40000
echo test-select 
./select heap5.txt 0 KIDWU KIDWU 80000
echo test-select 
./select heap6.txt 0 KIDWU KIDWU 250000
echo test-select 
./select heap7.txt 0 KIDWU KIDWU 750000
echo test-select 
./select heap8.txt 0 KIDWU KIDWU 1000000
echo test-select 
./select heap9.txt 0 KIDWU KIDWU 1500000
echo test-select 
./select heap10.txt 0 KIDWU KIDWU 2000000