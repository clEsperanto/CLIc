#!/usr/bin/python

import sys, os, glob

# stringify function for clij kernel
def stringify_ocl_code(filename, name, out_folder, prefix="cle_"):
    # generate header name
    new_fname = os.path.join(out_folder, "{0}{1}.h".format(prefix, name))
    if not os.path.exists(new_fname):
        # I/O kernel into header
        with open(new_fname, 'w') as output_file, open(filename, 'r') as input_file:
            output_file.write("#ifndef __{0}{1}_h\n".format(prefix, name))
            output_file.write("#define __{0}{1}_h\n".format(prefix, name))
            output_file.write("\n")
            for line in input_file:
                clean_line = list(filter(None, line.split('\n')))
                if len(clean_line) > 0:
                    clean_line = clean_line[0].replace(r'"', r'\"')
                    output_file.write("\"{0}\\n\"\n".format(clean_line))
                else:
                    output_file.write("\"\\n\"\n")
            output_file.write("\n")
            output_file.write("#endif //__{0}{1}_h\n".format(prefix, name))

# check output folder existance
if not os.path.exists(sys.argv[2]):
    os.makedirs(sys.argv[2])
    
if os.path.isdir(sys.argv[1]):
    file_list = glob.glob( os.path.join(sys.argv[1],'**/*.cl'), recursive=True)
    for file in file_list:
        filename = os.path.basename(file)
        name, ext = os.path.splitext(filename)
        stringify_ocl_code(file, name, sys.argv[2])
