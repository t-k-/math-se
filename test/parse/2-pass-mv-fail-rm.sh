#!/bin/sh
[ $# -ne 1 ] && echo "bad arg. (cmd dir)" && exit
[ ! -d ${1} ] && echo "${1} not dir." && exit
cat "test-summary.txt" | while read line
do 
	path=`echo $line | awk '{ print $1 }'`
	echo $line | awk '{ exit ($2 == "fail") }' 
	if [ $? -eq 0 ]
	then
		echo "mv ${path} (pass) to ${1}"
		[ -e "${path}" ] && mv "${path}" "${1}"
	else
		echo "rm ${path} (fail)"
		rm -f "${path}"
	fi
done 
> "test-summary.txt"
