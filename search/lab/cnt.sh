#!/bin/bash
if [ $# -ne 1 ]
then
	echo 'bad arg.'
	exit
fi

cnt=0
while read line
do
	if [ -e "$line" ]
	then
		res=$(wc -l $line | awk '{print $1}')
		let 'cnt=res+cnt'
		#echo "[$res] $line"
	fi
done <<EOF
$(cat ${1})
EOF
echo $cnt in total 
