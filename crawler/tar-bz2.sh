#!/bin/bash
dir=math.stackexchange.com-raw-col
if [ ! -e ${dir}.tar.bz2 ];
then
	tar -cjf ${dir}.tar.bz2 ${dir}
else
	echo "already compressed."
fi;
