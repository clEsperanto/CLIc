/*  CLIc - version 0.1 - Copyright 2020 Stéphane Rigaud, Robert Haase,
*   Institut Pasteur Paris, Max Planck Institute for Molecular Cell Biology and Genetics Dresden
*
*   CLIc is part of the clEsperanto project http://clesperanto.net 
*
*   This file is subject to the terms and conditions defined in
*   file 'LICENSE.txt', which is part of this source code package.
*/


#ifndef __tiffwriter_h
#define __tiffwriter_h

#include <tiffio.h>
 
class TiffWriter
{
public:
    TiffWriter(const char* filename);
    TiffWriter(const TiffWriter&) = delete;
    TiffWriter& operator=(const TiffWriter&) = delete;
    ~TiffWriter();
     
    void write(float* buffer, unsigned int width, unsigned int height, unsigned int depth);
 
private:
    TIFF* tiff;
};
 
#endif // __tiffwriter_h