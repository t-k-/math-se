#!/bin/bash
web=math.stackexchange.com
# web=www.test.com
> tmp
find $web -type f -exec bash -c './grep-cmd.sh {} >> tmp' \;
sort tmp | uniq -c > tex_cmd.list
rm tmp
