/**
 * Author: Stephane Rigaud - @strigaud 
 */

#include "tiffreader.h"
#include <iostream>

TiffReader::TiffReader(const char* filename)
{
    tiff = TIFFOpen(filename, "r");
}

float* TiffReader::read(unsigned int* width, unsigned int* height, unsigned int* depth)
{
    float* buffer;
    size_t global_cpt = 0;
    size_t npixels = 0;

    if (tiff)
    {
        TIFFGetField(tiff, TIFFTAG_IMAGEWIDTH, width);
		TIFFGetField(tiff, TIFFTAG_IMAGELENGTH, height);
        *depth = 0;
        do {
            (*depth)++;
        } while (TIFFReadDirectory(tiff));

        // get the total number of pixels
        npixels = (*width) * (*height) * (*depth); 
        buffer = new float[npixels];
        
        // read directories for data
        for (size_t d = 0; d < (*depth); d++)
        {
            TIFFSetDirectory(tiff, d);
            size_t local_nb_pixels = (*width) * (*height);
            uint32* raster = (uint32*)_TIFFmalloc(local_nb_pixels * sizeof(uint32));
            if (raster != nullptr) // check the raster's memory was allocaed
            {
                if (TIFFReadRGBAImageOriented(tiff, *width, *height, raster, 0))
                {
                    // iterate through all the pixels of the tif
                    for (size_t i = 0; i < local_nb_pixels; i++)
                    {
                        uint32& TiffPixel = raster[i]; 
                        buffer[global_cpt] = TIFFGetR(TiffPixel);
                        global_cpt++;
                    }
                    _TIFFfree(raster);
                }
            }
        }
    }
    else
    {
        std::cerr << "Could not access TIFF image" << std::endl;
    }
    return buffer;
}
 
TiffReader::~TiffReader()
{
    TIFFClose(tiff);
}