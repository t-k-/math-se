#!/bin/bash
grep -oP '(?<!\\)\\[a-zA-Z]+' $1
