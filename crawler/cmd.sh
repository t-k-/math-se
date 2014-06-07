#!/bin/bash
cat math.stackexchange.com/* | grep -o -P '\\[a-z]+?{' > tmp
cat tmp | sort | uniq -c > commands
rm tmp
