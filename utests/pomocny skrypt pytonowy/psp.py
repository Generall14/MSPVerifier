#!/usr/bin/python3
fi = open("in.s43", "r")
fo = open("out.txt", 'w')

fo.write("\tQList<Line> list = {\n")
lineCounter = 0

for line in fi.readlines():
	lineCounter += 1
	nline = line.strip()
	nline = nline.replace('\t', ' ')
	while nline.find('  ') >= 0:
		nline = nline.replace('  ', ' ')
	nline = ' '+nline+' '
	fo.write('\t\t\t\t{0} "{1}", "{2}", {3} {4},\n'.format('{', "file", nline, lineCounter, '}'))

fo.write('\t\t\t\t};\n')
fo.write('\tFun f(list);\n')

fi.close()
fo.close()
