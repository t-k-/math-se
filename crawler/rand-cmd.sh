#!/bin/bash
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
[ $# -ne 1 ] && echo "bad arg." && exit
[ ! -d ${1} ] && echo "not dir." && exit
cd ${DIR}/${1}
echo "pick random file @ `pwd`"
file=`ls | shuf -n 1`
echo ${file} | tee -a ${DIR}/rand-hist.log
