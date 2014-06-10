#!/bin/bash
find math.stackexchange.com -type f | xargs grep --color -P "(?<!\\\\)\\\\${1}[^a-z^A-Z]+"
