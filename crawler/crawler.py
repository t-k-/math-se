import re
from bs4 import BeautifulSoup
def tex_find(re_mod, string, str_mod):
	m = re_mod.findall(string)
	i = 0
	for string in m:
		print str_mod,
		print " mode latex", i, ":", string
		i = i + 1

re_dollar = re.compile(r'\$(?!\$)(.+?)\$')
re_inline = re.compile(r'\\\\\((.+?)\\\\\)')
re_display = re.compile(r'\\\[(.+?)\\\]')
# s = BeautifulSoup(open("stackexchange.html"))
s = BeautifulSoup(open("test.html"))
tag_p_array = s.find_all('p')
for obj in tag_p_array:
	string = obj.string
	if string is None:
		continue
	string = string.replace("\n", '')
	print "for { %s }:" % string 
	tex_find(re_dollar, string, 'dollar')
	tex_find(re_inline, string, 'inline')
	tex_find(re_display, string, 'display')
