#ifndef __cleScalar_hpp
#define __cleScalar_hpp

#include "clic.hpp"
#include <string>
#include <memory>
#include <algorithm>
#include <iterator>
#include <array>

#include "cleLightObject.hpp"

namespace cle
{

/**
 * template Scalar class 
 *
 * Inherit from LighObject, the class define a holder for float and int type.
 * This enable to process scalar arguments along buffer and image arguments.
 * The template define the scalar type.
 */
template<class T =float>
class Scalar : public LightObject
{
private:
    /// scalar value
    T m_obj = 0;

protected:

    /**     
     * @brief Convert template to string.
     * @return template type as string
     */
    const char* TemplateToString() const;

public: 

    /**     
     * @brief Default constructor.
     */
    Scalar() =default;

    /**     
     * @brief Constructor.
     * 
     * @param t_value value to allocate.
     */
    Scalar(const T t_value);

    /**     
     * @brief Default destructor.
     */
    ~Scalar() =default;

    /**     
     * @brief Get value.
     * 
     * @return value of templated type.
     */
    const T Data() const;

    /**     
     * @brief Get scalar dimension.
     * 
     * @return dimension equal to 1.
     */
    const int nDim() const;

    /**     
     * @brief Get scalar size.
     * 
     * @return size equal to 1.
     */
    const int Size() const;

    /**     
     * @brief Get scalar shape.
     * 
     * @return shape array equal to {1,1,1}.
     */
    const std::array<size_t,3> Shape() const;

    /**     
     * @brief Get scalar origin.
     * 
     * @return origin array equal to {0,0,0}.
     */
    const std::array<size_t,3> Origin() const;

    /**     
     * @brief Get scalar region.
     * 
     * @return region array equal to {1,1,1}.
     */
    const std::array<size_t,3> Region() const;

    /**
     * @brief Get object data type (float or int).
     * 
     * @return data type as string.
     */
    virtual const char* GetDataType() const;

    /**
     * @brief Compare object data type with template (float or int).
     * 
     * @param t_dtype data type to compare with
     * @return true if same data type, false otherwise.
     */
    virtual const bool IsDataType(const char* t_dtype) const; 

    /**
     * @brief Get object type (Buffer, Image, etc.).
     * 
     * @return Scalar as string.
     */   
    virtual const char* GetObjectType() const;

    /**
     * @brief Compare object type (Scalar).
     * 
     * @param t_otype object type to compare with
     * @return true if same object type, false otherwise.
     */
    virtual const bool IsObjectType(const char* t_otype) const;
};

template<class T>
Scalar<T>::Scalar(const T t_value) : LightObject(), m_obj(t_value)
{}

template<class T>
const T Scalar<T>::Data() const 
{ 
    return this->m_obj; 
}

template<class T>
const int Scalar<T>::nDim() const 
{ 
    return 1; 
}

template<class T>
const int Scalar<T>::Size() const 
{ 
    return 1; 
}

template<class T>
const std::array<size_t,3> Scalar<T>::Shape() const 
{ 
    return std::array<size_t,3>({1,1,1}); 
}

template<class T>
const std::array<size_t,3> Scalar<T>::Origin() const 
{ 
    return std::array<size_t,3>({0,0,0}); 
}

template<class T>
const std::array<size_t,3> Scalar<T>::Region() const 
{ 
    return std::array<size_t,3>({1,1,1}); 
}

template<class T>
const char* Scalar<T>::GetDataType() const 
{ 
    return TemplateToString(); 
}

template<class T>
const bool Scalar<T>::IsDataType(const char* t_str) const
{
    return strncmp(this->TemplateToString(), t_str, strlen(t_str)) == 0;
}

template<class T>
const char* Scalar<T>::GetObjectType() const 
{ 
    return "scalar"; 
}   

template<class T>
const bool Scalar<T>::IsObjectType(const char* t_str) const
{
    return strncmp("scalar", t_str, strlen(t_str)) == 0;
}

template<class T>
const char* Scalar<T>::TemplateToString() const
{
    if(std::is_same<T, float>::value)  return "float";
    if(std::is_same<T, int>::value) return "int";
    if(std::is_same<T, unsigned int>::value) return "uint";
    if(std::is_same<T, char>::value) return "char";
    if(std::is_same<T, unsigned char>::value) return "uchar";
    if(std::is_same<T, double>::value) return "double";
    if(std::is_same<T, short>::value) return "short";
    if(std::is_same<T, unsigned short>::value) return "ushort";
    return "unknown";
} 

} // namespace cle

#endif //__cleScalar_hpp