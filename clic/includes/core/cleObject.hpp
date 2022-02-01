#ifndef __cleObject_hpp
#define __cleObject_hpp

#include "clic.hpp"
#include <string>
#include <memory>
#include <algorithm>
#include <iterator>
#include <array>
#include <functional>
#include <numeric>

#include "cleLightObject.hpp"

namespace cle
{

/**
 * generic class defining object properties
 *
 * ! cl::buffer, cl::image1d, cl::image2d, and cl::image3d, as the rest of opencl memory object
 * ! inherit of cl::memory. Possibility to make an gpu object generic for all type and apply 
 * ! object convertion when only when needed (a.k.a when defining sources and setting arguments of kernel)
 * todo: v0.6 future refont of data class for code clarity and scalability. PoC needed before commiting. 
 */
class Object : public LightObject
{
public: 

    /**
     * enum for DataType.
     */
    enum DataType { FLOAT, DOUBLE, INT, UINT, CHAR, UCHAR, SHORT, USHORT };

    /**
     * Default constructor.
     */
    Object() = default;

    /**
     * constructor.
     * @param t_shape the shape of the image as an array of size 3.
     * @param t_dtype the data type of the image.
     * @param t_otype the object type, buffer or image,
     */
    Object(const std::array<size_t,3>& t_shape, const DataType t_dtype, const char* t_otype): LightObject(), m_dType(t_dtype), m_oType(t_otype), m_Shape(t_shape), m_Region(t_shape)
    {
        if (this->m_Shape[2]>1) this->m_ndim = 3;
        else if (this->m_Shape[1]>1) this->m_ndim = 2;
        else this->m_ndim = 1;
    };   
    
    /**
     * Default destructor.
     */
    ~Object() =default;

    /**
     * Get object dimension.
     * @return dimensionality.
     */
    const int nDim() const { return m_ndim; };

    /**
     * Get object size as number of elements.
     * @return number of elements.
     */
    const int Size() const { return static_cast<int>(std::accumulate(m_Shape.begin(), m_Shape.end(), 1, std::multiplies<size_t>())); };
    
    /**
     * Get object shape (width, height, depth).
     * @return shape array of size 3.
     */
    const std::array<size_t,3> Shape() const { return m_Shape; };

    /**
     * Get object origin coordinate (x, y, z).
     * @return coordinate array of size 3.
     */
    const std::array<size_t,3> Origin() const { return m_Origin; };

    /**
     * Get object region shape (width, height, depth).
     * @return shape array of size 3.
     */
    const std::array<size_t,3> Region() const { return m_Region; };

    /**
     * Get object data type.
     * @return data type as Object::DataType.
     */
    const DataType Type() const { return m_dType; };

    /**
     * Get object data type.
     * @return data type as string.
     */
    const char* GetDataType() const 
    { 
        if (this->m_dType == cle::Object::DataType::FLOAT) return "float"; 
        if (this->m_dType == cle::Object::DataType::DOUBLE) return "double"; 
        if (this->m_dType == cle::Object::DataType::INT) return "int"; 
        if (this->m_dType == cle::Object::DataType::CHAR) return "char"; 
        if (this->m_dType == cle::Object::DataType::UINT) return "uint"; 
        if (this->m_dType == cle::Object::DataType::UCHAR) return "uchar"; 
        if (this->m_dType == cle::Object::DataType::SHORT) return "short"; 
        if (this->m_dType == cle::Object::DataType::USHORT) return "ushort"; 
        return "";
    };

    /**
     * Compare object data type (float, int, char, etc.).
     * @param t_dtype data type to compare with
     * @return true if same data type, false otherwise.
     */
    const bool IsDataType(const char* t_dtype) const
    {
        size_t size = strlen(t_dtype);
        if (this->m_dType == DataType::FLOAT && strncmp("float", t_dtype, size) == 0) return true; 
        if (this->m_dType == DataType::DOUBLE && strncmp("double", t_dtype, size) == 0) return true; 
        if (this->m_dType == DataType::INT && strncmp("int", t_dtype, size) == 0) return true; 
        if (this->m_dType == DataType::CHAR && strncmp("char", t_dtype, size) == 0) return true;
        if (this->m_dType == DataType::UINT && strncmp("uint", t_dtype, size) == 0) return true; 
        if (this->m_dType == DataType::UCHAR && strncmp("uchar", t_dtype, size) == 0) return true; 
        if (this->m_dType == DataType::SHORT && strncmp("short", t_dtype, size) == 0) return true; 
        if (this->m_dType == DataType::USHORT && strncmp("ushort", t_dtype, size) == 0) return true; 
        return false;
    };

    /**
     * Get object type (Buffer, Image, etc.).
     * @return data type as string.
     */ 
    const char* GetObjectType() const { return m_oType; };

    /**
     * Compare object type (Buffer, Image, etc.).
     * @param t_otype object type to compare with
     * @return true if same object type, false otherwise.
     */
    const bool IsObjectType(const char* t_otype) const { return strncmp(this->m_oType, t_otype, strlen(t_otype)) == 0; };

protected:

    /// data type holder
    DataType m_dType = DataType::FLOAT;
    /// object type holder
    const char* m_oType = "object";
    /// objet dimension
    int m_ndim = 1;
    /// object shape
    std::array<size_t,3> m_Shape {1, 1, 1};
    /// object origin
    std::array<size_t,3> m_Origin {0, 0, 0};
    /// object region
    std::array<size_t,3> m_Region {1, 1, 1};
};

} // namespace cle

#endif //__cleObject_hpp