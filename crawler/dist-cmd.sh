#!/bin/bash
web=math.stackexchange.com
# web=www.test.com
> tmp
find $web -type f -exec bash -c './grep-cmd.sh {} >> tmp' \;
sort tmp | uniq -c > tex-cmd.list
rm tmp
