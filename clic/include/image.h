/*  CLIc - version 0.1 - Copyright 2020 Stéphane Rigaud, Robert Haase,
*   Institut Pasteur Paris, Max Planck Institute for Molecular Cell Biology and Genetics Dresden
*
*   CLIc is part of the clEsperanto project http://clesperanto.net 
*
*   This file is subject to the terms and conditions defined in
*   file 'LICENSE.txt', which is part of this source code package.
*/


#ifndef __image_h
#define __image_h

#include <array>
#include <iostream>

template<class T>
class Image
{

private:

    std::string type = "";
    T *data = nullptr;
    std::array<unsigned int,3> dimensions = {0, 0, 0};
    
public:
    Image(){};
    Image(T*, unsigned int=1, unsigned int=1, unsigned int=1, std::string ="float");
    Image(T*, unsigned int*, std::string ="float");
    ~Image();

    void SetKey(std::string);
    void SetData(T*);


    std::string GetType();
    T* GetData();
    size_t GetDataSize();
    unsigned int GetNbPixels();
    unsigned int* GetDimensions();

};

template<class T>
Image<T>::Image(T* arr, unsigned int w, unsigned int h, unsigned int d, std::string _type)
{
    size_t dim = size_t(w*h*d);
    data = new T[dim];
    std::copy(arr, arr + dim, data);
    dimensions[0] = w;
    dimensions[1] = h;
    dimensions[2] = d;
    type = _type;
}

template<class T>
Image<T>::Image(T* arr, unsigned int* dim, std::string _type)
{
    size_t nbDim = sizeof(dim) / sizeof(dim[0]) +1;
    if (nbDim > 3)
    {
        nbDim = 3;
        std::cerr << "warning: Image maximum dimensions exeeded,"; 
        std::cerr << "only the three first values are considered.";
        std::cerr << std::endl;
    }
    for (size_t i = 0; i < nbDim; i++)
    {
        dimensions[i] = dim[i];
    }
    size_t nbElements = dimensions[0] * dimensions[1] * dimensions[2];
    data = new T[nbElements];
    std::copy(arr, arr + nbElements, data);
    type = _type;
}

template<class T>
Image<T>::~Image()
{    
}

template<class T>
void Image<T>::SetData(T* arr)
{
    data = new T[this->GetNbPixels()];
    std::copy(arr, arr + this->GetNbPixels(), data);
}

template<class T>
std::string Image<T>::GetType()
{
    return type;
}

template<class T>
T* Image<T>::GetData()
{
    return data;
}

template<class T>
size_t Image<T>::GetDataSize()
{
    return sizeof(T) * dimensions[0] * dimensions[1] * dimensions[2];
}

template<class T>
unsigned int Image<T>::GetNbPixels()
{
    return dimensions[0] * dimensions[1] * dimensions[2];
}

template<class T>
unsigned int* Image<T>::GetDimensions()
{
    return dimensions.data();
}

#endif // __image_h