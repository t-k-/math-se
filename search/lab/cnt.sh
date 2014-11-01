#!/bin/bash
cnt=0
while read line
do
	if [ -e "$line" ]
	then
		res=$(wc -l $line | awk '{print $1}')
		let 'cnt=res+cnt'
		echo "[$res] $line"
#		if [ $cnt -ge 15 ]
#		then
#			exit
#		fi
	fi
done <<EOF
$(cat dir.txt)
EOF
echo $cnt in total 
