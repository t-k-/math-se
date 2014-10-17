#!/bin/bash
web=math.stackexchange.com-raw-col
> tmp
find $web -type f -exec bash -c './grep-cmd.sh {} >> tmp' \;
sort tmp | uniq -c > tex-cmd.list
rm tmp
