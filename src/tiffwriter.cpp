/*  CLIc - version 0.1 - Copyright 2020 Stéphane Rigaud, Robert Haase,
*   Institut Pasteur Paris, Max Planck Institute for Molecular Cell Biology and Genetics Dresden
*
*   CLIc is part of the clEsperanto project http://clesperanto.net 
*
*   This file is subject to the terms and conditions defined in
*   file 'LICENSE.txt', which is part of this source code package.
*/


#include "tiffwriter.h"
#include <iostream>

TiffWriter::TiffWriter(const char* filename)
{
    tiff = TIFFOpen(filename, "w");
}
 
void TiffWriter::write(float* buffer, unsigned int width, unsigned int height, unsigned int depth)
{
    uint16 spp (1), bpp (8);
    size_t buffer_size = width * height * depth;
    uint8* raster = (uint8*)_TIFFmalloc(buffer_size * sizeof(uint8));
    std::copy(buffer, buffer + buffer_size, raster);
    
    for(size_t page = 0; page < depth; page++)
    {
        TIFFSetField(tiff, TIFFTAG_IMAGEWIDTH, width);
        TIFFSetField(tiff, TIFFTAG_IMAGELENGTH, height);
        TIFFSetField(tiff, TIFFTAG_BITSPERSAMPLE, bpp);
        TIFFSetField(tiff, TIFFTAG_SAMPLESPERPIXEL, spp);
        TIFFSetField(tiff, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
        TIFFSetField(tiff, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK);
        TIFFSetField(tiff, TIFFTAG_ORIENTATION, ORIENTATION_TOPLEFT);
        TIFFSetField(tiff, TIFFTAG_PAGENUMBER, page, depth);
        for (size_t j = 0; j < height; j++)
        {
            TIFFWriteScanline(tiff, &raster[j * width + page * (width*height)], j, 0);   
        }   
        TIFFWriteDirectory(tiff);     
    }
}
 
TiffWriter::~TiffWriter()
{
    TIFFClose(tiff);
}
