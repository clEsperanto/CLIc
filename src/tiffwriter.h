/**
 * Author: Stephane Rigaud - @strigaud 
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