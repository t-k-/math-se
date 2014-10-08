#!/bin/sh
[ $# -ne 3 ] && echo "bad arg. (cmd from_dir script dest_dir)" && exit
from=${1}
scrpt=url-${2}.sh
dest=${3}

[ ! -d $from ] && echo "$from not dir." && exit
[ ! -e $scrpt ] && echo "no $scrpt." && exit
[ ! -d $dest ] && echo "$dest not dir." && exit
[ ! -e foreach-idx.sh ] && echo "no foreach-idx.sh." && exit
[ ! -f ./parser.ln ] && echo "parser.ln not generated." && exit
[ ! -f ./index.py.ln ] && echo "index.py.ln not generated." && exit
[ ! -f ./libbdb_wraper.so ] && echo "libbdb_wraper.so not generated." && exit

find $from \
-exec sh -c "test -f {} && sed -i -e 's/\r//g' {}" \; \
-exec sh -c "echo {} | ./${scrpt} | xargs ./foreach-idx.sh {} $dest" \;
