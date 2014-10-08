#!/bin/sh
read file_name
num=`echo $file_name | grep -P -o '(?<=summary-)[0-9]+'`
url="http://math.stackexchange.com/questions/$num"
echo $url
