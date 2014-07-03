#!/bin/bash
ref=`md5sum head.output | awk '{print $1}'`
cat body.output | while read -d $'\n' line
do           
	dir=`echo $line | awk '{print $1}'`
	br_wd=`echo $line | awk '{print $2 $3}'`
	mkdir -p $dir
	cp head.output $dir/$ref
	echo $br_wd " " $ref >> $dir/posting
	echo "record $ref @@ $dir/posting..."
done
