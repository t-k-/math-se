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

def curl(sub_url, c):
	buf = cStringIO.StringIO()
	c.setopt(c.URL, 'http://' + root_url + sub_url)
	c.setopt(c.WRITEFUNCTION, buf.write)
	errs = 0
	while True:
		try:
			c.perform()
		except:
			errs = errs + 1
			if errs > 5:
				print "stop trying."
				break
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


def find_q_page(navi_page, c):
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
		find_p(curl(tag_a['href'], c), save_file)
		save_file.close()

def crawl(start_page, end_page):
	c = pycurl.Curl()
	c.setopt(c.CONNECTTIMEOUT, 8)
	c.setopt(c.TIMEOUT, 10)

	if not os.path.exists(root_url):
		os.makedirs(root_url)

	for i in range(start_page, end_page):
		print "page %d/%d..." % (i, end_page)
		sub_url = '/questions?sort=newest&page='
		navi_page = curl(sub_url + str(i), c)
		find_q_page(navi_page, c)
                time.sleep(0.5)

# crawl(1, 19420)

import threading
import time

new_thread = threading.Thread(target = crawl, args=(4080, 5000))
new_thread.start()
time.sleep(5)

new_thread = threading.Thread(target = crawl, args=(9080, 10000))
new_thread.start()
time.sleep(5)

new_thread = threading.Thread(target = crawl, args=(14080, 15000))
new_thread.start()
time.sleep(5)

new_thread = threading.Thread(target = crawl, args=(19080, 20000))
new_thread.start()
