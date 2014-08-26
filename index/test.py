#!/usr/bin/python2.7
import ctypes
so = ctypes.CDLL("./libbdb_wraper.so") 
so.bdb_records.restype = ctypes.c_uint64

bdb_doc = so.bdb_init("./test_doc.bdb")
bdb_num = so.bdb_init("./test_num.bdb")

if not bdb_doc or not bdb_num:
	print 'bdb open error.'	
	quit()

print so.bdb_records(bdb_doc)
so.bdb_put2(bdb_doc, "abc", "some string")
so.bdb_put_int(bdb_num, "abc", 3, 123)
print so.bdb_records(bdb_doc)

p = so.bdb_get2(bdb_doc, "abc")
save = ctypes.string_at(p)
so.c_free(p)
print save

p = so.bdb_get_int(bdb_num, "abc", 3)
INTP = ctypes.POINTER(ctypes.c_int)
py_p = ctypes.cast(p, INTP)
print 'value:', py_p[0] + 1
so.c_free(p)

so.bdb_release(bdb_doc)
so.bdb_release(bdb_num)
