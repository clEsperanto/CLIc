#!/usr/bin/python

import os, sys, glob

def run_clang_format(folder):
    cpp_file = glob.glob("{0}/src/**/*.cpp".format(folder), recursive=True)
    hpp_file = glob.glob("{0}/include/**/*.hpp".format(folder), recursive=True)
    for [h, c] in zip(hpp_file, cpp_file):
        os.system('clang-format-{0} -i style=file {1}'.format(9, c))
        os.system('clang-format-{0} -i style=file {1}'.format(9, h))
     
run_clang_format(sys.argv[1])
