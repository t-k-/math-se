#!/bin/sh
file=$1
dest=$2
url=$3
log=fail.log
[ ! $# -eq 3 ] && echo "bad # args." && exit
[ ! -d $dest ] && echo "$dest not dir." && exit

./parser.ln $url < $file 1> /dev/null
status="$? / `wc -l $file`"
echo $status | awk '{exit ($1 == $3)}'
if [ $? -eq 0 ];
then
	tput setaf 1; echo "$status \t\t fail" \
	| tee -a ${log}; tput sgr0 
else
	echo -n $status; tput setaf 2; echo "\t\t pass" \
	&& tput sgr0
fi
./index.py.ln -p $dest | tail -1
