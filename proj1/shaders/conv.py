#!/usr/bin/python3
import sys
from functools import *

fn_src = sys.argv[1]
fn_tar = sys.argv[2]
fn_bas = sys.argv[3]
fsrc = open(fn_src,'r')
ftar = open(fn_tar,'w')

li = list(map(lambda s:'"'+s.strip()+'\\n"',fsrc.read().split('\n')))
while len(li)>0 and li[-1]=='"\\n"':
    li.pop()
res = '// mew~ this file is auto generated from '+fn_src+'\n'
res = res+'GL_char SRC_'+fn_bas+'[] = {\n'
res = res+'\n'.join(li)
res = res+'\n};'

ftar.write(res)
