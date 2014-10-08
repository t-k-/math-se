#!/bin/sh
[ $# -ne 2 ] && echo "bad arg. (cmd src_file dest_dir)" && exit
[ ! -e ${1} ] && echo "${1} not file." && exit
[ ! -d ${2} ] && echo "${2} not dir." && exit
file="${1}"
dest=`pwd`/"${2}"
num=`echo $file | grep -P -o '(?<=summary-)[0-9]+'`
url="http://math.stackexchange.com/questions/$num"

./parser.ln $url < $file 1> /dev/null
status="$? / `wc -l $file`"
echo $status | awk '{exit ($1 == $3)}'
if [ $? -eq 0 ];
then
	echo -n $status; tput setaf 1; echo "\t\t fail" \
	| tee -a ${output}; tput sgr0 
else
	echo -n $status; tput setaf 2; echo "\t\t pass" \
	| tee -a ${output}; tput sgr0
fi
./index.py.ln -p $dest | tail -1
