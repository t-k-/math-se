#!/bin/bash
find math.stackexchange.com -type f | xargs -I % bash -c "./grep-cmd.sh ${1} % && echo '(%)'; sleep 0.2"
