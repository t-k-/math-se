import codecs
# coding=utf-8  

# fi = codecs.open('test.txt', encoding='utf-8')
# s = fi.read()

def find_dollar_tex(s, sf):
	i, j, stack = 0, 0, 0
	while i < len(s):
		#print s[i],
		if s[i] == '$':
			if stack == 0:
				stack = 1
				j = i + 1
			elif stack == 1:
				if i == j:
					stack = 2
					j = i + 1
				else:
					stack = 0
					# print 'single:', s[j:i]
					tex = s[j:i]
					tex_utf8 = tex.encode('utf-8')
					sf.write(tex_utf8 + '\n')
			else:
				stack = 0
				#print 'double:', s[j:i]
				tex = s[j:i]
				tex_utf8 = tex.encode('utf-8')
				sf.write(tex_utf8 + '\n')
				i += 1
		i += 1
