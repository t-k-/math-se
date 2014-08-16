#!/usr/bin/python2.7
import hashlib
import os
import ctypes
# init DBM
so = ctypes.CDLL("./libbdb_wraper.so") 
os.system("mkdir -p ./collection")
so.bdb_init("./collection/documents.bdb")
so.bdb_get2.restype = ctypes.c_char_p
old_records = so.bdb_records()

f = open('parser.output', 'r')
url = f.readline()
while True:
	# get the formula
	formula = f.readline()
	if not formula:
		break

	# get the sha-1 of doc
	m = hashlib.sha1()
	m.update(url)
	m.update(formula)
	doc_sha1 = m.hexdigest()

	# get the tree 
	tree = ''
	while True:
		res = f.readline()
		if not res or res == "\n":
			break
		tree += res

	# record the doc into database
	doc = url + '\n' + formula + '\n' + tree
	print '[ record %s into database... ]' % doc_sha1
	print doc
	old_doc = so.bdb_get2(doc_sha1)
	so.bdb_free_last()
	if old_doc:
		print 'same sha-1 key exists, skip...'
		print '[ old document: ]\n%s' % old_doc
		while True:
			line = f.readline()
			if not line or line == "\n":
				break
	else:
		so.bdb_put2(doc_sha1, doc)
		# get the branch words and save them
		while True:
			line = f.readline()
			if not line or line == "\n":
				break
			# get the dir and brword from lines 
			array = line.split()
			path = array[0]
			brword = " ".join(array[1:]);
			# calculate the sha1 for brword
			m = hashlib.sha1()
			m.update(doc_sha1)
			m.update(brword)
			brword_sha1 = m.hexdigest()
			# mkdir directory and write the posting file
			print "post at path: %s" % path
			print "branch word: %s" % brword
			os.system("mkdir -p " + path)
			posting = open(path + '/posting', 'a')
			line_fmt = brword_sha1 + ' ' + doc_sha1 + ' ' + brword 
			posting.write(line_fmt + '\n')
			posting.close()

f.close()
delta_records = so.bdb_records() - old_records
print "[ %s new records in DB. ]" % delta_records
so.bdb_release()
