#!/usr/bin/python

import sys, os

# stringify function for clij kernel
def stringify_clij_code(fname, fout, prefix="cle_"):
    # get kernel name
    print("input name {0}".format(fname))
    name = fname.split(os.sep)[-1].split('.cl')[0].split('_x')[0]
    print("isolate name {0}".format(name))
    # generate header name
    new_fname = os.path.join(fout, "{0}{1}.h".format(prefix,name))
    print("new name {0}".format(new_fname))

    # I/O kernel into header
    with open(new_fname, 'w') as output_file, open(fname, 'r') as input_file:
        output_file.write("#ifndef __{0}{1}_h\n".format(prefix, name))
        output_file.write("#define __{0}{1}_h\n".format(prefix, name))
        output_file.write("\n")
        for line in input_file:
            clean_line = list(filter(None, line.split('\n')))
            if len(clean_line) > 0:
                output_file.write("\"{0}\\n\"\n".format(clean_line[0]))
            else:
                output_file.write("\"\\n\"\n")
        output_file.write("\n")
        output_file.write("#endif //__{0}{1}_h\n".format(prefix, name))

# check output folder existance
if not os.path.isdir(sys.argv[2]):
    os.makedirs(sys.argv[2])

# process folder/file
if os.path.isdir(sys.argv[1]):
    for filename in os.listdir(sys.argv[1]):
        if filename.endswith(".cl"):
            stringify_clij_code(os.path.join(sys.argv[1],filename), sys.argv[2])
else:
    stringify_clij_code(sys.argv[1], sys.argv[2])

