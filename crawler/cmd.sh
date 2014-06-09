#!/bin/bash
# grep -P "(?<!\\\\)\\\\[a-zA-Z]+" ./ok/question-summary-274633
> tex-cmd.list
find ./ok -type f -exec bash -c 'echo grep -P "\"(?<!\\\\\\\\)\\\\\\\\[a-zA-Z]+\"" {} > tmp' \; -exec bash -c 'bash tmp >> tex-cmd.list' \; -exec rm tmp \;
# find ./ok -type f | xargs  >> tmp
# cat tmp | sort | uniq -c > commands
