#!/usr/bin/python2.7
import re
import os
import sys
import pycurl
import time 
import cStringIO
import getopt
from dollar import find_dollar_tex
from bs4 import BeautifulSoup

re_inline = re.compile(r'\\\\\((.+?)\\\\\)')
re_display = re.compile(r'\\\[(.+?)\\\]')

root_url = 'math.stackexchange.com'

def curl(sub_url, c):
	buf = cStringIO.StringIO()
	url = 'http://' + root_url + sub_url
	url = url.encode('iso-8859-1')
	c.setopt(c.URL, url)
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
	tag_p_array = s.find_all('p')
	for obj in tag_p_array:
		string = obj.string
		if string is None:
			continue
		# a newline is equavalent to a space in Tex
		string = string.replace("\n", ' ') 
		string = string.replace("\r", ' ') 
		find_dollar_tex(string, sf)
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

def crawl_force(id):
	c = pycurl.Curl()
	c.setopt(c.CONNECTTIMEOUT, 8)
	c.setopt(c.TIMEOUT, 10)

	if not os.path.exists(root_url):
		os.makedirs(root_url)

	save_path = root_url + '/question-summary-' + id 
	if os.path.isfile(save_path):
		print "overwrite existing file..."
	else:
		print "new file..."
	save_file = open(save_path, 'w')

	url_redir = "/questions/" + id
	s = BeautifulSoup(curl(url_redir, c))
	tag_a = s.find('a')
	url = tag_a['href']
	print 'url: ', url
	find_p(curl(url, c), save_file)
	save_file.close()
	print "at: %s" % save_path

def crawl_all():
	crawl(1, 5000)

def help(arg0):
	print '%s [-a, --all] [-f, --force <post id>] [-t, --test <file path>]' % arg0
	sys.exit(1)

def main(arg):
	argv = arg[1:]
	try:
		opts, args = getopt.getopt(argv, "af:t:", ['all', 'force=', 'test='])
		if len(opts) == 0:
			raise
	except:
		help(arg[0])

	for opt, arg in opts:
		if opt in ("-a", "--all"):
			print "crawling all pages..."
			crawl_all()
			break
		if opt in ("-t", "--test"):
			print "crawling test page %s ..." % arg
			find_p(open(arg), sys.stdout)
			break
		if opt in ("-f", "--force"):
			print "crawling %s..." % arg
			crawl_force(arg)

if __name__ == "__main__":
	main(sys.argv)
