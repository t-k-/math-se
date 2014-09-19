#!/bin/sh
[ $# -ne 1 ] && echo "bad arg. (cmd dir)" && exit
[ ! -e ${1} ] && echo "${1} not file." && exit
./parser.ln nil < ${1} | grep -B 1 '^\['
