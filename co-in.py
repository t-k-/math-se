#!/usr/bin/python2.7
import hashlib
import os
f = open('parser.output', 'r')
url = f.readline()
while True:
	# get the formula
	formula = f.readline()
	if not formula:
		break
	# get the sha-1
	m = hashlib.sha1()
	m.update(url)
	m.update(formula)
	sha1 = m.hexdigest()
	# get the tree 
	tree = ''
	while True:
		res = f.readline()
		if not res or res == "\n":
			break
		tree += res
	# get the branch words and save them
	while True:
		line = f.readline()
		if not line or line == "\n":
			break
		array = line.split()
		path = array[0]
		brword = array[1] + ' ' + array[2];
		print "path: %s/%s" % (path, sha1)
		os.system("mkdir -p " + path)
		print "branch word: %s" % brword
		posting = open(path + '/posting', 'a')
		document = open(path + '/' + sha1, 'w')
		posting.write(sha1 + ' ' + brword + '\n')
		posting.close()
		document.write(url)
		document.write(formula)
		document.write(tree)
		document.close()
f.close()
