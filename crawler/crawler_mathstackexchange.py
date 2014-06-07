import re
import os
import sys
import pycurl
import cStringIO
from bs4 import BeautifulSoup

re_sdollar = re.compile(r'(?<!\$)\$(?!\$)(.+?)(?<!\$)\$(?!\$)')
re_ddollar = re.compile(r'\$\$(.+?)\$\$')
re_inline = re.compile(r'\\\\\((.+?)\\\\\)')
re_display = re.compile(r'\\\[(.+?)\\\]')

root_url = 'math.stackexchange.com'

c = pycurl.Curl()
c.setopt(c.CONNECTTIMEOUT, 8)
c.setopt(c.TIMEOUT, 10)

def curl(sub_url):
	buf = cStringIO.StringIO()
	c.setopt(c.URL, 'http://' + root_url + sub_url)
	c.setopt(c.WRITEFUNCTION, buf.write)
	while True:
		try:
			c.perform()
		except:
			print "try again..."
			continue
		break
	res_str = buf.getvalue()
	buf.close()
	return res_str

def find_tex(re_mod, string, sf):
	m = re_mod.findall(string)
	for tex in m:
		tex_utf8 = tex.encode('utf-8')
		sf.write(tex_utf8 + '\n') 

def find_p(q_page, sf):
	s = BeautifulSoup(q_page)
	# s = BeautifulSoup(open("test.html"))
	tag_p_array = s.find_all('p')
	for obj in tag_p_array:
		string = obj.string
		if string is None:
			continue
                # a newline is equavalent to a space in Tex
		string = string.replace("\n", ' ') 
		find_tex(re_sdollar, string, sf)
		find_tex(re_ddollar, string, sf)
		find_tex(re_inline, string, sf)
		find_tex(re_display, string, sf)


def find_q_page(navi_page):
	# find question page url
	s = BeautifulSoup(navi_page)
	tag_sums = s.find_all('div', {"class": "question-summary"})
	for tag_sum in tag_sums:
		tag_a = tag_sum.find('a', {"class": "question-hyperlink"})
		print 'crawling', tag_sum['id'], '...'
		# curl it...
		save_path = root_url + '/' + tag_sum['id']
		if os.path.isfile(save_path):
			print "aready exists."
			continue
		save_file = open(save_path, 'w')
		find_p(curl(tag_a['href']), save_file)
		save_file.close()

def crawl(start_page, end_page):
	if not os.path.exists(root_url):
		os.makedirs(root_url)

	for i in range(start_page, end_page):
		print "page %d/%d..." % (i, end_page)
		navi_page = curl('/questions?sort=newest&page=' + str(i))
		find_q_page(navi_page)

crawl(1, 19420)
