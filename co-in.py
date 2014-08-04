#!/usr/bin/python2.7
import hashlib
import os
import ctypes
# init DBM
so = ctypes.CDLL("./libbdb_wraper.so") 
so.bdb_init("collection.bdb")
so.bdb_get2.restype = ctypes.c_char_p
old_records = so.bdb_records()

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

	# record the doc into database
	doc = url + '\n' + formula + '\n' + tree
	print '[ record %s into database... ]' % sha1
	print doc
	old_doc = so.bdb_get2(sha1)
	so.bdb_free_last()
	if old_doc:
		print 'same sha-1 key exists, skip...'
		print '[ old document: ]\n%s' % old_doc
		while True:
			line = f.readline()
			if not line or line == "\n":
				break
	else:
		so.bdb_put2(sha1, doc)
		# get the branch words and save them
		while True:
			line = f.readline()
			if not line or line == "\n":
				break
			array = line.split()
			path = array[0]
			brword = " ".join(array[1:]);
			print "post at path: %s" % path
			print "branch word: %s" % brword
			os.system("mkdir -p " + path)
			posting = open(path + '/posting', 'a')
			posting.write(sha1 + ' ' + brword + '\n')
			posting.close()

f.close()
delta_records = so.bdb_records() - old_records
print "[ %s new records in DB. ]" % delta_records
so.bdb_release()
