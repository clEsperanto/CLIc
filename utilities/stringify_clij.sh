#!/bin/bash

echo "start script stringify"

FOLDER_IN=$1 # input folder
FOLDER_OU=$2 # output folder

# stringify a kernel file into a header string
convert_kernel_to_header()
{
    IN=$1           # input file .cl
    OU=$2           # ouput folder
    PFX="cle_"      # prefix header name
    INPUT=$(echo $IN | rev | cut -d'/' -f 1 | rev)
    NAME=${INPUT%.cl}
    NAME=${NAME%_x}
    OUT=$OU$PFX$NAME.h

    # if file do not exist
    if [ ! -f "$OUT" ] 
    then
        echo "#ifndef __cle_"$NAME"_h" >>$OUT
        echo "#define __cle_"$NAME"_h" >>$OUT
        echo "\\\n" >>$OUT
        sed -e 's/\\/\\\\/g;s/"/\\"/g;s/^/"/;s/$/\\n"/' $IN>>$OUT
        echo "\\\n\\\n" >>$OUT
        echo "#endif // __cle_"$NAME"_h" >>$OUT
    fi
}


echo "test folder"
# create folder if not existing
if [ ! -d $FOLDER_OU ] 
then
    echo "Creating folder"
    mkdir $FOLDER_OU
fi

echo "start loop"
# stringify all .cl files in input folder
for file in $FOLDER_IN*.cl
do 
    echo "$file"
    convert_kernel_to_header $file $FOLDER_OU 
done
echo "end loop"
