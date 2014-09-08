#!/bin/bash
export QUERY_STRING=s=0
# query = a + b
echo -n "q=a%2Bb" | ./rank.cgi
