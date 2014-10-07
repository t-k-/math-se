#!/bin/sh
[ $# -ne 2 ] && echo "bad arg. (cmd test_dir num)" && exit
[ ! -d ${1} ] && echo "${1} not dir." && exit
output="test-summary.txt"

> rand-hist.log
for rep in `seq ${2}`
do
	echo random file $rep
	./rand-cmd.sh.ln ./${1}
done

echo "[ test summary ]"
while read rfile
do
	path=./${1}/$rfile
	echo -n "${path} \t\t" | tee -a ${output};
	sed -i -e 's/\r//g' $path # make sure no <CR>
	./parser.ln nil < $path > /dev/null
	status="$? / `wc -l $path`"
	echo $status | awk '{exit ($1 == $3)}'
	if [ $? -eq 0 ];
	then
		tput setaf 1; echo fail \
		| tee -a ${output}; tput sgr0 
	else
		tput setaf 2; echo pass \
		| tee -a ${output}; tput sgr0
	fi
done < rand-hist.log
