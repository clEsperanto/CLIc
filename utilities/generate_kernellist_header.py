#!/usr/bin/python

import sys, os, glob

def generate_kernel_list_header(folder, output_folder):
    kernel_list_header_file = os.path.join(output_folder,"cleKernelList.hpp")
    if not os.path.exists(kernel_list_header_file):
        include_list = glob.glob(os.path.join(folder,'**/*.hpp'))
        with open(kernel_list_header_file, 'w') as output_file:
            output_file.write("#ifndef __CORE_CLEKERNELLIST_HPP\n")
            output_file.write("#define __CORE_CLEKERNELLIST_HPP\n")
            output_file.write("\n")
            for include in include_list:
                if include.find('core') == -1:
                    include_file = os.path.basename(include)
                    output_file.write("#include \"{0}\"\n".format(include_file))
            output_file.write("\n")
            output_file.write("#endif //__CORE_CLEKERNELLIST_HPP\n")

generate_kernel_list_header(sys.argv[1], sys.argv[2])
