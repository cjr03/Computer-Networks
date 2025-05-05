#!/bin/bash
echo "----------Make--------"
make;
echo
echo "----------Creating Files----------"
# cat /dev/random | head -c <bytecount>
cat /dev/random | head -c 100000 > src_1.dat
cat /dev/random | head -c 200000 > src_2.dat
cat /dev/random | head -c 300000 > src_3.dat
cat /dev/random | head -c 400000 > src_4.dat
cat /dev/random | head -c 500000 > src_5.dat
echo "----------Running steps----------"
echo "Threading"
./1 src_1.dat src_2.dat src_3.dat src_4.dat src_5.dat dst_1.dat dst_2.dat dst_3.dat dst_4.dat dst_5.dat
echo
echo "Persistance"
./2
echo
make clean;
echo

