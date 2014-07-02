#!/bin/bash
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
cd ${DIR}/math.stackexchange.com
file=`ls | shuf -n 1`
echo ${file}...
cat $file | tee ${DIR}/../rand
