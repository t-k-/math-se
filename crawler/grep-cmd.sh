#!/bin/bash
if [ $# -eq 1 ] 
then
	grep -oP '(?<!\\)\\[a-zA-Z]+' $1
else
	grep --color -P "(?<!\\\\)\\\\${1}[^a-z^A-Z]+" ${2}
fi
