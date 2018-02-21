#!/bin/bash
make
python mkcsv.py records.txt 1000
echo test-csv2colstore 
./csv2colstore records.txt colstore 1500
echo test-select2
./select2 colstore 0 AAAAA ZZZZZ 1500
echo test-select3
./select3 colstore 1 0 AAAAA ZZZZZ 1500


echo test-csv2colstore 
./csv2colstore records.txt colstore 5000
echo test-select2
./select2 colstore 0 AAAAA ZZZZZ 5000
echo test-select3
./select3 colstore 1 0 AAAAA ZZZZZ 5000
echo test-csv2colstore 
./csv2colstore records.txt colstore 10000
echo test-select2 
./select2 colstore 0 AAAAA ZZZZZ 10000
echo test-select3 
./select3 colstore 1 0 AAAAA ZZZZZ 10000
echo test-csv2colstore  
./csv2colstore records.txt colstore 40000
echo test-select2 
./select2 colstore 0 AAAAA ZZZZZ 40000
echo test-select3 
./select3 colstore 1 0 AAAAA ZZZZZ 40000
echo test-csv2colstore 
./csv2colstore records.txt colstore 80000
echo test-select2 
./select2 colstore 0 AAAAA ZZZZZ 80000
echo test-select3 
./select3 colstore 1 0 AAAAA ZZZZZ 80000
echo test-csv2colstore 
./csv2colstore records.txt colstore 250000
echo test-select2 
./select2 colstore 0 AAAAA ZZZZZ 250000
echo test-select3 
./select3 colstore 1 0 AAAAA ZZZZZ 250000
echo test-csv2colstore  
./csv2colstore records.txt colstore 750000
echo test-select2 
./select2 colstore 0 AAAAA ZZZZZ 750000
echo test-select3 
./select3 colstore 1 0 AAAAA ZZZZZ 750000
echo test-csv2colstore 
./csv2colstore records.txt colstore 1000000
echo test-select2 
./select2 colstore 0 AAAAA ZZZZZ 1000000
echo test-select3 
./select3 colstore 1 0 AAAAA ZZZZZ 1000000
echo test-csv2colstore 
./csv2colstore records.txt colstore 1500000
echo test-select2 
./select2 colstore 0 AAAAA ZZZZZ 1500000
echo test-select3 
./select3 colstore 1 0 AAAAA ZZZZZ 1500000
echo test-csv2colstore 
./csv2colstore records.txt colstore 2000000
echo test-select2 
./select2 colstore 0 AAAAA ZZZZZ 2000000
echo test-select3 
./select3 colstore 1 0 AAAAA ZZZZZ 2000000





















