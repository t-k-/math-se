#!/bin/bash
cat math.stackexchange.com/* | grep -o -P '(?<!\\)\\[a-zA-Z]+' > tmp
cat tmp | sort | uniq -c > commands
rm tmp
