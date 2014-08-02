#!/usr/bin/python2.7
import hashlib
f = open('parser.output', 'r')
url = f.readline()
print "url: %s" % url
while True:
	formula = f.readline()
	if not formula:
		break
	print "formula: %s" % formula,
	
	m = hashlib.sha1()
	m.update(url)
	m.update(formula)
	sha1 = m.hexdigest()
	print "sha1: %s" % sha1 

	tree = ''
	while True:
		res = f.readline()
		if not res or res == "\n":
			break
		tree += res
	print "tree: \n %s" % tree,

	while True:
		brword = f.readline()
		if not brword or brword == "\n":
			break
		print "branch word: %s" % brword,
