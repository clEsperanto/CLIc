/**
 * Author: Stephane Rigaud - @strigaud 
 */

#ifndef __tiffreader_h
#define __tiffreader_h

#include <tiffio.h>
#include "image.h"
 
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