#!/usr/bin/python

import sys, os

def run_clang_format(folder):
    for tier in os.listdir("{0}/clic/src".format(folder)):
        os.system('clang-format-{0} -i -style=file {1}/clic/src/{2}/*.cpp'.format(14, folder, tier))
        os.system('clang-format-{0} -i -style=file {1}/clic/include/{2}/*.hpp'.format(14, folder, tier))
    os.system('clang-format-{0} -i -style=file {1}/tests/*.cpp'.format(14, folder))
        
run_clang_format(sys.argv[1])
