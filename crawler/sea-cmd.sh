#!/bin/bash
find math.stackexchange.com -type f | xargs -I % bash -c 'grep --color -P "(?<!\\\\)\\\\${1}[^a-z^A-Z]+" % && sleep 0.2;'
