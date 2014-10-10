#!/bin/sh
[ $# -ne 3 ] && echo "bad arg. (cmd from_dir script dest_dir)" && exit
src_dir=`dirname "$0"`
from=${1}
scrpt=${src_dir}/url-${2}.sh
dest=${3}
forea=${src_dir}/foreach-idx.sh

[ ! -d $from ] && echo "$from not dir." && exit
[ ! -e $scrpt ] && echo "no $scrpt." && exit
[ ! -d $dest ] && echo "$dest not dir." && exit
[ ! -e $foreach ] && echo "no foreach script" && exit

find $from \
-exec sh -c "test -f {} && sed -i -e 's/\r//g' {}" \; \
-exec sh -c "echo {} | ${scrpt} | xargs $forea {} $dest" \;
