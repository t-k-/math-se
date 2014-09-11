#!/bin/bash
curdir="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
rootdir=`cd ${curdir}/../.. && pwd`
echo rootdir=${rootdir}
if [ $# -ne 3 ]
then
	echo 'bad arg.'
	exit
fi
# parser directory
cd $rootdir/parser
>tmp
echo $1 >> tmp
echo $2 >> tmp
./parser no_url < tmp
rm tmp

# root
cd $rootdir
cp ./parser/parser.output ./index

# index directory
cd ./index
rm -rf collection
./index.py

# root
cd $rootdir
echo $rootdir
rm -rf collection
mv ./index/collection .
./search.ln "$3"
