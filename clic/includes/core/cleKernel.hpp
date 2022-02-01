

#ifndef __cleKernel_hpp
#define __cleKernel_hpp

#include "clic.hpp"

#include <string>
#include <fstream>
#include <iostream>
#include <map>
#include <vector>
#include <array>
#include <algorithm>
#include <memory>

#include "cleLightObject.hpp"
#include "cleObject.hpp"
#include "cleScalar.hpp"
#include "cleBuffer.hpp"
#include "cleImage.hpp"


#include "cleGPU.hpp"


namespace cle
{

/**
 * Abstract class defining Kernel operation build up and executions 
 *
 * To execute a kernel operation in opencl, one must build the program sources, encapsulate the program
 * in a kernel structure and set the arguments to the kernel before enqueuing the kernel for execution.
 * Those operations are depend of the type, dimensions, number, and order of arguments requested by the 
 * source code.
 * The Kernel class provide the operations and methods to read the arguments and sources and adapt the 
 * OpenCL #defines according to the clEsperanto language. All clEsperanto operation should inherit from 
 * this class. 
 */
class Kernel
{
private:
    /// Program and Kernel to be execute by Device
    cl::Program m_Program;
    cl::Kernel m_Kernel;

    /**
     * compute part of the clEsperanto defines specific to Buffer.
     * 
     * @param t_tag buffer tag (src, dst, etc.).
     * @param t_dtype buffer data type (float, int, char, etc.).
     * @param t_dim buffer dimensionality (1, 2, 3).
     * @return buffer specific defines as string.
     * @exceptsafe No-throw guarantee.
     */
    const std::string BufferDefines(std::string& t_tag, std::string& t_dtype, std::string& t_dim) const;
    
    /**
     * compute part of the clEsperanto defines specific to Image.
     * 
     * @param t_tag image tag (src, dst, etc.).
     * @param t_dtype image data type (float, int, char, etc.).
     * @param t_dim image dimensionality (1, 2, 3).
     * @return image specific defines as string.
     * @exceptsafe No-throw guarantee.
     */
    const std::string ImageDefines(std::string& t_tag, std::string& t_dtype, std::string& t_dim) const;

protected:
    /// Pointer to GPU ressources
    std::shared_ptr<GPU> m_gpu;

    /// Device global range
    std::array<size_t, 3> m_GlobalRange = {{0, 0, 0}};

    /// kernel specifics parameters to be defined in subclass
    std::string m_KernelName ="";
    std::string m_nDimPrefix ="";
    std::vector<std::string> m_Tags;
    std::map<std::string, std::string> m_Sources;
    std::map<std::string, std::shared_ptr<cle::LightObject> > m_Parameters;

    /// Source code hash and build flag parameters
    size_t m_CurrentHash = 0;
    bool m_BuildProgram = true;

    /**
     * Load clEsperanto `preamble.h`.
     * 
     * @return preamble source code as string.
     * @exceptsafe No-throw guarantee.
     */
    const std::string LoadPreamble() const;

    /**
     * Load kernel operation source defined in daughter class.
     * 
     * @return kernel source code as string.
     * @exceptsafe No-throw guarantee.
     */
    const std::string LoadSources() const;

    /**
     * Generate opencl define code based on kernel I/O parameters.
     * 
     * @return defines source code as string.
     * @exceptsafe No-throw guarantee.
     */
    const std::string LoadDefines() const;

    /**
     * return short version of data type name.
     * 
     * @param t_str data type as string (float, int, etc.)
     * @return short name of data type (f, i, c, etc.).
     * @exceptsafe No-throw guarantee.
     */
    std::string TypeAbbr(const char*) const;

    /**
     * Add Object (Buffer or Image) to the parameter list with associated tag.
     * If {tag,object} pair already exist, replace.
     * 
     * @param t_object Object type (Buffer or Image) to add to the paramters
     * @param t_tag paramter tag associated to the object
     */
    void AddObject(Object& t_object, const char* t_tag);

    /**
     * Add int to the parameter list with associated tag.
     * If {tag,scalar} pair already exist, replace.
     * 
     * @param t_scalar scalar type (int) to add to the paramters
     * @param t_tag parameter tag associated to the object
     */
    void AddObject(int t_scalar, const char* t_tag);

    /**
     * Add float to the parameter list with associated tag.
     * If {tag,scalar} pair already exist, replace.
     * 
     * @param t_scalar scalar type (float) to add to the paramters
     * @param t_tag parameter tag associated to the object
     */
    void AddObject(float t_scalar, const char* t_tag);

    /**
     * Method to detect I/O dimensionality and adapte kernel if needed.
     * 
     * @exceptsafe No-throw guarantee.
     */
    void ManageDimensions();

    /**
     * Method to build program from source and store it in a kernel.
     */
    void BuildProgramKernel();

    /**
     * Method to set each arguments from the parameter list to the kernel.
     */
    void SetArguments();

    /**
     * Method to define global range base on argument shape.
     * 
     * @param t_tag tag of parameter to use. 
     * @exceptsafe No-throw guarantee.
     */
    bool SetGlobalNDRange(const char* t_tag);

    /**
     * Method to define global range base shape.
     * 
     * @param t_shape shape to use. 
     * @exceptsafe No-throw guarantee.
     */
    bool SetGlobalNDRange(const std::array<size_t,3>& t_shape);

    /**
     * Enqueue Kernel in the device command queue for execution.
     */
    void EnqueueKernel();

    /**
     * Method to get parameter from parameter list using tag id.
     *  
     * @param t_tag tag of parameter to retrieve.
     * @return Pointer to object, null if not found.
     * @exceptsafe No-throw guarantee.
     */
    template<class T>
    std::shared_ptr<T> GetParameter(const char* t_tag) const
    {
        if(this->m_Parameters.find(t_tag) != this->m_Parameters.end())
        {
            return std::dynamic_pointer_cast<T>(this->m_Parameters.at(t_tag));
        }
        else
        {
            return nullptr;
        }
    }

public:
    /**
     * Constructor.
     *  
     * @param t_gpu gpu ressources to be used.
     * @param t_name kernel operation name.
     * @param t_tags tag list of parameters.
     */
    Kernel(std::shared_ptr<GPU> t_gpu, const char* t_name, const std::vector<std::string>& t_tags);

    /**
     * Destructor.
     */
    ~Kernel();

    /**
     * Virtual method execute, to be implemented in the subclass.
     * The method is a placeholder for host code needed to run the kernel.
     */
    virtual void Execute() = 0;
};

} // namespace cle

#endif // __cleKernel_hpp