#!/bin/bash
echo $#
if [ $# -ne 3 ]
then
	echo 'bad arg.'
	exit
fi
cd parser
echo $1 >> tmp
echo $2 >> tmp
./parser no_url < tmp
rm tmp
cp parser.output ../index
cd ../index
./index.py
rm -rf ../search/collection
mv collection ../search/
cd ../search
./search $3
