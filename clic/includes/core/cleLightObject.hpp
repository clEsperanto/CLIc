#ifndef __cleLightObject_hpp
#define __cleLightObject_hpp

#include "clic.hpp"
#include <string>
#include <memory>
#include <algorithm>
#include <iterator>
#include <array>

namespace cle
{

/**
 * Abstract class defining object to be processed by a kernel 
 *
 * LightObject define the general information that an object must have
 * to be process through GPU.
 * The class is abstract and define the pure base from which Image, Buffer,
 * and Scalar will inherit.
 */
class LightObject
{
public: 
    /**
     * @brief Default constructor.
     */
    LightObject() =default;

    /**
     * @brief Virtual default destructor.
     */
    virtual ~LightObject() =default;

    /**
     * @brief Virtual get object dimension.
     * 
     * @return dimensionality.
     */
    virtual const int nDim() const =0;

    /**
     * @brief Virtual get object size as number of elements.
     * 
     * @return number of elements.
     */
    virtual const int Size() const =0;

    /**
     * @brief Virtual get object shape (width, height, depth).
     * 
     * @return shape array of size 3.
     */
    virtual const std::array<size_t,3> Shape() const =0;

    /**
     * @brief Virtual get object origin coordinate (x, y, z).
     * 
     * @return coordinate array of size 3.
     */
    virtual const std::array<size_t,3> Origin() const =0;

    /**
     * @brief Virtual get object region shape (width, height, depth).
     * 
     * @return shape array of size 3.
     */
    virtual const std::array<size_t,3> Region() const =0;

    /**
     * @brief Virtual get object data type (float, int, char, etc.).
     * 
     * @return data type as string.
     */
    virtual const char* GetDataType() const =0;

    /**
     * @brief Virtual compare object data type (float, int, char, etc.).
     * 
     * @param t_dtype data type to compare with
     * @return true if same data type, false otherwise.
     */
    virtual const bool IsDataType(const char* t_dtype) const =0; 

    /**
     * @brief Virtual get object type (Buffer, Image, etc.).
     * @return data type as string.
     */   
    virtual const char* GetObjectType() const =0;

    /**
     * @brief Virtual compare object type (Buffer, Image, etc.).
     * 
     * @param t_otype object type to compare with
     * @return true if same object type, false otherwise.
     */
    virtual const bool IsObjectType(const char* t_otype) const =0;
};

} // namespace cle

#endif //__cleLightObject_hpp