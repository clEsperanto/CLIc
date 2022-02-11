#ifndef __cleObject_hpp
#define __cleObject_hpp

#include "clic.hpp"
#include "cleLightObject.hpp"

namespace cle
{

/**
 * generic class defining object properties
 *
 */
class Object : public LightObject
{
public: 

    /**
     * @brief enum for DataType.
     */
    enum DataType { FLOAT, DOUBLE, INT, UINT, CHAR, UCHAR, SHORT, USHORT };

    /**
     * @brief Default constructor.
     */
    Object() = default; 
    
    /**
     * @brief constructor.
     * 
     * @param t_object memory object in device.
     * @param t_shape data shape as an array of 3.
     * @param t_dtype data type of the image.
     */
    Object(const cl::Memory& t_object, const std::array<size_t,3>& t_shape, const DataType t_dtype);

    /**
     * @brief Default destructor.
     */
    ~Object() =default;

    void Reset()
    {
        if(this->m_Ocl.get() != nullptr)
        {
            this->m_Ocl = cl::Memory();
        }
        this->m_dType = DataType::FLOAT;
        this->m_oType = "empty";
        this->m_ndim = 0;
        this->m_Shape = {1, 1, 1};
        this->m_Origin = {0, 0, 0};
        this->m_Region = {1, 1, 1};
    }

    /**
     * @brief Get object memory.
     * 
     * @return memory object pointing to data in device.
     */
    const cl::Memory& Data() const;

    /**
     * @brief Get object memory type.
     * 
     * @return memory type (CL_MEM_OBJECT_BUFFER, CL_MEM_OBJECT_IMAGE1D, etc.).
     */
    const cl_mem_object_type MemoryType() const;

    /**
     * @brief Get object memory size.
     * 
     * @return memory size (in bits).
     */
    const cl::size_type MemorySize() const;

    /**
     * @brief Get object dimension.
     * 
     * @return dimensionality.
     */
    const int nDim() const;

    /**
     * @brief Get object size as number of elements.
     * 
     * @return number of elements.
     */
    const int Size() const;
    
    /**
     * @brief Get object shape (width, height, depth).
     * 
     * @return shape array of size 3.
     */
    const std::array<size_t,3> Shape() const;

    /**
     * @brief Get object origin coordinate (x, y, z).
     * 
     * @return coordinate array of size 3.
     */
    const std::array<size_t,3> Origin() const;

    /**
     * @brief Get object region shape (width, height, depth).
     * 
     * @return shape array of size 3.
     */
    const std::array<size_t,3> Region() const;

    /**
     * @brief Get object data type.
     * 
     * @return data type as Object::DataType.
     */
    const DataType Type() const;

    /**
     * @brief Get object data type.
     * 
     * @return data type as string.
     */
    const char* GetDataType() const;

    /**
     * @brief Compare object data type (float, int, char, etc.).
     * 
     * @param t_dtype data type to compare with
     * @return true if same data type, false otherwise.
     */
    const bool IsDataType(const char* t_dtype) const;

    /**
     * @brief Get object type (Buffer, Image, etc.).
     * 
     * @return data type as string.
     */ 
    const char* GetObjectType() const;

    /**
     * @brief Compare object type (Buffer, Image, etc.).
     * 
     * @param t_otype object type to compare with
     * @return true if same object type, false otherwise.
     */
    const bool IsObjectType(const char* t_otype) const;

    /**
     * @brief Compare Memory type.
     * 
     * @param t_type object type to compare with
     * @return true if same object type, false otherwise.
     */
    const bool IsMemoryType(const cl_mem_object_type t_type) const;

    /**
     * @brief Print out operator
     * 
     * @param os output stream
     * @param obj object to print out
     * @return output stream reference
     */
    // friend std::ostream& operator<<(std::ostream& os, const Object& obj);

protected:

    /// OpenCL memory object (buffer, image, etc.)
    cl::Memory m_Ocl;
    /// data type holder
    DataType m_dType = DataType::FLOAT;
    /// object type holder
    const char* m_oType = "empty";
    /// objet dimension
    int m_ndim = 0;
    /// object shape
    std::array<size_t,3> m_Shape {1, 1, 1};
    /// object origin
    std::array<size_t,3> m_Origin {0, 0, 0};
    /// object region
    std::array<size_t,3> m_Region {1, 1, 1};
};

} // namespace cle

#endif //__cleObject_hpp