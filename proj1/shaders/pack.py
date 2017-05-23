#!/usr/bin/python3
# -*- coding: utf-8 -*-

"""
Pack up the .sdr files and make shaderSrc.h

[insert] if the .sdr source code isn't in shaderSrc.h
[update] if the code has been in shaderSrc.h
And will not change the rest of shaderSrc.h

Args:
    [1,2,3...] file path

by MoebiusMeow
"""

import sys
import os
from functools import *

for argi in range(1,len(sys.argv)):
    fn_bas = os.path.basename(sys.argv[argi])
    fn_dir = os.path.dirname(sys.argv[argi])
    print('  packing',fn_bas)
    fsrc = open(os.path.join(fn_dir,'./',fn_bas),'r')
    ftar = open(os.path.join(fn_dir,'./')+'shaderSrc.h.tmp','r')
    ctar = ftar.read().split('\n')
    ftar.close()

    csrc = list(map(lambda s:'"'+s+'\\n"',fsrc.read().split('\n')))
    while len(csrc)>0 and csrc[-1]=='"\\n"':
        csrc.pop()
    sres = '\t// <'+fn_bas+'>\n'
    sres += '\tlis[string("'+fn_bas+'")] = string\n\t(\n\t\t'
    sres += '\n\t\t'.join(csrc)
    sres += '\n\t);\n'

    def findnx(pos,str):
        while pos<len(ctar):
            if ctar[pos][:len(str)]==str:
                return pos
            pos += 1
        return -1

    pos = findnx(0,'\t// <'+fn_bas+'>')
    if pos==-1:
        pos = findnx(0,'\t// <Begin>')+1
        res_f = '\n'.join(ctar[:pos])+'\n'
        res_b = '\n'.join(ctar[pos:])
    else:
        res_f = '\n'.join(ctar[:pos])+'\n'
        pos = findnx(pos+1,'\t// <')
        res_b = '\n'.join(ctar[pos:])

    res = res_f+sres+res_b

    ftar = open(os.path.join(fn_dir,'./')+'shaderSrc.h.tmp','w')
    ftar.write(res)
    ftar.close()
    fsrc.close()
