#!/bin/bash
export QUERY_STRING=s=0
# query = (1+\frac{1}{n})^n
echo -n 'q=(1%2B\frac{1}{n})^n' | ./rank.cgi
