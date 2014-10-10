#!/usr/bin/python2.7
import hashlib
import os
import ctypes
import sys
import getopt

def help(arg0):
	print '%s [-p, --path=<collection path>]' % arg0
	sys.exit(1)

# set collection path
argv = sys.argv
co_path = "./collection"
try: 
	opts, args = getopt.getopt(argv[1:], "p:", ['path='])
	if len(opts) == 0 and len(argv) != 1:
		raise
except:
	help(argv[0])
	exit(0)

for opt, arg in opts:
	if opt in ("-p", "--path"):
		co_path = arg

print "target path: %s" % co_path

# init DBM
src_dir = os.path.dirname(argv[0])
so = ctypes.CDLL(src_dir + "/libbdb_wraper.so") 
os.system("mkdir -p " + co_path)
bdb_doc = so.bdb_init(co_path + "/documents.bdb")
bdb_num = so.bdb_init(co_path + "/brw-number.bdb")

old_records = so.bdb_records(bdb_doc)
DOC_HASH_LEN = 40

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
	print '\n[ ======= record ======= ]' 
	print "formula %s (%s)" % (formula, doc_sha1)
	print doc
	p = so.bdb_get2(bdb_doc, doc_sha1)
	if p:
		old_doc = ctypes.string_at(p)
	else:
		old_doc = None 
	so.c_free(p)
	
	if old_doc:
		print 'same sha-1 key exists, skip...'
		print '[ old document: ]\n%s' % old_doc
		while True:
			line = f.readline()
			if not line or line == "\n":
				break
	else:
		so.bdb_put2(bdb_doc, doc_sha1, doc)
		# get the branch words and save them
		brw_num = 0
		while True:
			line = f.readline()
			if not line or line == "\n":
				break
			# get the dir and brword from lines 
			array = line.split()
			path = co_path + '/' + array[0]
			brword = " ".join(array[1:]);
			# calculate the sha1 for brword
			m = hashlib.sha1()
			m.update(doc_sha1)
			m.update(str(brw_num))
			brw_num += 1
			brword_sha1 = m.hexdigest()
			# mkdir directory and write the posting file
			print "post at path: %s" % path
			print "#%d branch word: %s (%s)" % (brw_num, 
			                         brword, brword_sha1)
			os.system("mkdir -p " + path)
			posting = open(path + '/posting', 'a')
			line_fmt = brword_sha1 + ' ' + doc_sha1 + ' ' + brword 
			posting.write(line_fmt + '\n')
			posting.close()
		print '[brws = ' + str(brw_num) + ']'
		so.bdb_put_int(bdb_num, doc_sha1, DOC_HASH_LEN, brw_num)

f.close()
delta_records = so.bdb_records(bdb_doc) - old_records
print "[ %s new records in DB. ]" % delta_records
so.bdb_release(bdb_doc)
so.bdb_release(bdb_num)
