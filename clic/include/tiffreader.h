/*  CLIc - version 0.1 - Copyright 2020 Stéphane Rigaud, Robert Haase,
*   Institut Pasteur Paris, Max Planck Institute for Molecular Cell Biology and Genetics Dresden
*
*   CLIc is part of the clEsperanto project http://clesperanto.net 
*
*   This file is subject to the terms and conditions defined in
*   file 'LICENSE.txt', which is part of this source code package.
*/


#ifndef __tiffreader_h
#define __tiffreader_h

#include <tiffio.h>
 
class TiffReader
{
public:
    TiffReader(const char* filename);
    TiffReader(const TiffReader&) = delete;
    TiffReader& operator=(const TiffReader&) = delete;
    ~TiffReader();
     
    float* read(unsigned int* width, unsigned int* height, unsigned int* depth);

private:
    TIFF* tiff;
};
 
#endif // __tiffReader_h