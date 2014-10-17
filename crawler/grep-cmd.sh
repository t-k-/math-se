#!/bin/bash
if [ $# -eq 1 ] 
then
	# example
	# ./grep-cmd.sh 'math.stackexchange.com-raw-col/*'
	grep -oP '(?<!\\)\\[a-zA-Z]+' $1
else
	# ./grep-cmd.sh frac 'math.stackexchange.com-raw-col/*'
	grep --color -P "(?<!\\\\)\\\\${1}[^a-z^A-Z]+" ${2}
fi
