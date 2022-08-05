#!/usr/bin/python

import sys, os

def get_includes_list(folder):
    includes_list = []
    for root, _, files in os.walk(folder):
        if root.split(os.sep)[-1] != "core": 
            for file in files:
                    includes_list.append(file)
    return sorted(includes_list)


def generate_kernel_list_header(folder):
    kernel_list_header_file = os.path.join(folder,os.path.join("core","cleKernelList.hpp"))
    include_list = get_includes_list(folder)
    with open(kernel_list_header_file, 'w') as output_file:
        output_file.write("#ifndef __CORE_CLEKERNELLIST_HPP\n")
        output_file.write("#define __CORE_CLEKERNELLIST_HPP\n")
        output_file.write("\n")
        for include in include_list:
            output_file.write("#include \"{0}\"\n".format(include))
        output_file.write("\n")
        output_file.write("#endif //__CORE_CLEKERNELLIST_HPP\n")

generate_kernel_list_header(sys.argv[1])
