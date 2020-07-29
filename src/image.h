/**
 * Author: Stephane Rigaud - @strigaud 
 */

#ifndef __image_h
#define __image_h


#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif

template<class T>
class Image
{

private:

    std::string key;
    std::string type;
    T *data = nullptr;
    unsigned int dimension[3];
    
public:

    Image(T*, unsigned int, unsigned int, unsigned int, std::string, std::string);
    Image(unsigned int, unsigned int, unsigned int, std::string, std::string);

    ~Image();

    void SetKey(std::string);
    void SetData(T*);


    std::string GetKey();
    std::string GetType();
    T* GetData();
    size_t GetDataSize();
    unsigned int GetNbPixels();
    unsigned int* GetDimension();

};

template<class T>
Image<T>::Image(T* arr, unsigned int w, unsigned int h, unsigned int d, std::string _key,  std::string _type)
{
    size_t dim = w*h*d;
    data = new T[dim];
    std::copy(arr, arr + dim, data);
    dimension[0] = w;
    dimension[1] = h;
    dimension[2] = d;
    key = _key;
    type = _type;
}

template<class T>
Image<T>::Image(unsigned int w, unsigned int h, unsigned int d, std::string _key,  std::string _type)
{
    dimension[0] = w;
    dimension[1] = h;
    dimension[2] = d;    
    key = _key;
    type = _type;
}

template<class T>
Image<T>::~Image()
{    
}

template<class T>
void Image<T>::SetKey(std::string nKey)
{
    key = nKey;
}

template<class T>
void Image<T>::SetData(T* arr)
{
    data = new T[this->GetNbPixels()];
    std::copy(arr, arr + this->GetNbPixels(), data);
}

template<class T>
std::string Image<T>::GetKey()
{
    return key;
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
    return sizeof(T) * dimension[0] * dimension[1] * dimension[2];
}

template<class T>
unsigned int Image<T>::GetNbPixels()
{
    return dimension[0] * dimension[1] * dimension[2];
}

template<class T>
unsigned int* Image<T>::GetDimension()
{
    return dimension;
}

#endif // __image_h