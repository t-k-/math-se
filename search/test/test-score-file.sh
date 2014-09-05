#!/bin/bash
if [ $# -ne 1 ]
then
	echo "bad arg."
	exit
fi
dir="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
test="${dir}/test-score-stdin.sh"
. $1
