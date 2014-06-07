#!/bin/bash
grep -P "(?<!\\\\)\\\\${1}[^a-z^A-Z]+" math.stackexchange.com/*
