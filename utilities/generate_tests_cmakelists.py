#!/usr/bin/python

import sys, os

def generate_script(folder):
    constant = "${PROJECT_SOURCE_DIR}"
    with open(os.path.join(folder, "CMakeLists.txt"), 'w') as output_file:
        for filename in sorted(os.listdir(folder)):
            if filename.endswith(".cpp"):
                name = os.path.split(filename)[-1].split('.cpp')[0]
                output_file.write("add_executable({0} {0}.cpp)\n".format(name))
                output_file.write("add_dependencies({0} CLIc)\n".format(name))
                output_file.write("target_link_libraries({0} PRIVATE CLIc::CLIc)\n".format(name))
                output_file.write("set_target_properties({0} PROPERTIES FOLDER \"Tests\")\n".format(name))
                output_file.write("\n")
                output_file.write("add_test(NAME {0} ".format(name))
                output_file.write("WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} ")
                if name == "custom_test":
                    output_file.write("COMMAND {0} {1}/thirdparty/clesperanto-kernels/kernels)\n".format(name, constant))
                else:     
                    output_file.write("COMMAND {0})\n".format(name))
                output_file.write("set_tests_properties({0} PROPERTIES LABELS \"KERNELS\" WILL_FAIL FALSE)\n".format(name))
                output_file.write("\n")

generate_script(sys.argv[1])