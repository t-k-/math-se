#!/bin/sh
[ $# -ne 3 ] && echo "bad arg. (cmd from_dir script dest_dir)" && exit
from=${1}
scrpt=idx-${2}.sh
dest=${3}

[ ! -d $from ] && echo "$from not dir." && exit
[ ! -e $scrpt ] && echo "cannot find script $scrpt." && exit
[ ! -d $dest ] && echo "$dest not dir." && exit
[ ! -f ./parser.ln ] && echo "parser.ln not generated." && exit
[ ! -f ./index.py.ln ] && echo "index.py.ln not generated." && exit
[ ! -f ./libbdb_wraper.so ] && echo "libbdb_wraper.so not generated." && exit

find $from \
-exec sh -c "test -f {} && sed -i -e 's/\r//g' {}" \; \
-exec ./$scrpt {} $dest \;
