#!/bin/bash
cd math.stackexchange.com
file=`ls | shuf -n 1`
echo ${file}...
cat $file
