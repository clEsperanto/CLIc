
#ifndef __cleKernel_hpp
#define __cleKernel_hpp

#include "clic.hpp"
#include "cleLightObject.hpp"
#include "cleObject.hpp"
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
     * @brief compute part of the clEsperanto defines specific to Buffer. 
     * 
     * @param t_tag buffer tag (src, dst, etc.).
     * @param t_dtype buffer data type (float, int, char, etc.).
     * @param t_dim buffer dimensionality (1, 2, 3).
     * @return buffer specific defines as string.
     */
    const std::string BufferDefines(std::string& t_tag, std::string& t_dtype, std::string& t_dim) const;
    
    /**
     * @brief compute part of the clEsperanto defines specific to Image. 
     * 
     * @param t_tag image tag (src, dst, etc.).
     * @param t_dtype image data type (float, int, char, etc.).
     * @param t_dim image dimensionality (1, 2, 3).
     * @return image specific defines as string.
     */
    const std::string ImageDefines(std::string& t_tag, std::string& t_dtype, std::string& t_dim) const;

protected:
    /// Pointer to GPU ressources
    std::shared_ptr<GPU> m_gpu;

    /// Device global range
    std::array<size_t, 3> m_GlobalRange = {{0, 0, 0}};
    cl::NDRange m_globalND;

    /// kernel specifics parameters to be defined in subclass
    std::string m_KernelName ="";
    std::string m_nDimPrefix ="";
    std::vector<std::string> m_Tags;
    std::map<std::string, std::string> m_Sources;
    std::map<std::string, std::shared_ptr<cle::LightObject> > m_Parameters;
    std::map<std::string, std::string> m_Constants;

    /// Source code hash and build flag parameters
    size_t m_CurrentHash = 0;
    bool m_BuildProgram = true;

    /**
     * @brief Load clEsperanto `preamble.h`. 
     * 
     * @return preamble source code as string.
     */
    const std::string LoadPreamble() const;

    /**
     * @brief Load kernel operation source defined in daughter class. 
     * 
     * @return kernel source code as string.
     */
    const std::string LoadSources() const;

    /**
     * @brief Generate opencl define code based on kernel I/O parameters. 
     * 
     * @return defines source code as string.
     */
    const std::string LoadDefines() const;

    /**
     * @brief return short version of data type name. 
     * 
     * @param t_buffer flag, true if processing buffer, false if processing an image
     * @param t_str data type as string (float, int, etc.)
     * @return short name of data type (f, i, c, etc.).
     */
    std::string TypeAbbr(const char* t_str, const bool t_buffer =true) const;

    /**
     * @brief Add Object (Buffer or Image) to the parameter list with associated tag. 
     *        If {tag,object} pair already exist, replace. 
     * 
     * @param t_object Object type (Buffer or Image) to add to the paramters
     * @param t_tag paramter tag associated to the object
     * @throws std::exception::runtime_error throw if could not find `t_tag` in kernel tag list. 
     */
    void AddObject(Object& t_object, const char* t_tag);

    /**
     * @brief Add int to the parameter list with associated tag.
     *        If {tag,scalar} pair already exist, replace. 
     * 
     * @param t_scalar scalar type (int) to add to the paramters
     * @param t_tag parameter tag associated to the object
     * @throws std::exception::runtime_error throw if could not find `t_tag` in kernel tag list. 
     */
    void AddObject(int t_scalar, const char* t_tag);

    /**
     * @brief Add float to the parameter list with associated tag.
     *        If {tag,scalar} pair already exist, replace. 
     * 
     * @param t_scalar scalar type (float) to add to the paramters
     * @param t_tag parameter tag associated to the object
     * @throws std::exception::runtime_error throw if could not find `t_tag` in kernel tag list. 
     */
    void AddObject(float t_scalar, const char* t_tag);

    /**
     * @brief Add a constant value to be defined in the kernel source.
     * 
     * @param t_value constant value in string to be written in source
     * @param t_key constant variable name
     */
    void AddConstant(std::string t_value, std::string t_key);

    /**
     * @brief Method to detect I/O dimensionality and adapte kernel if needed. 
     */
    void ManageDimensions();

    /**
     * @brief Method to build program from source and store it in a kernel.
     */
    void BuildProgramKernel();

    /**
     * @brief Method to set each arguments from the parameter list to the kernel.
     */
    void SetArguments();

    /**
     * @brief Method to define global range base on argument shape. 
     * 
     * @param t_tag tag of parameter to use. 
     */
    bool SetGlobalNDRange(const char* t_tag);

    /**
     * @brief Method to define global range base shape. 
     * 
     * @param t_shape shape to use. 
     */
    bool SetGlobalNDRange(const std::array<size_t,3>& t_shape);

    /**
     * @brief Convert buffer or image shape into GlobalNDRange
     * 
     * @param t_shape 3d shape array
     * @return cl::NDRange 
     */
    cl::NDRange ComputeNDRange(const std::array<size_t,3>& t_shape);

    /**
     * @brief Enqueue Kernel in the device command queue for execution.
     * 
     * @throws std::exception::runtime_error throw if could not define a global ND range.
     * @throws std::exception::runtime_error throw if could not enqueue ND range in command queue.
     */
    void EnqueueKernel();

    /**
     * @brief Method to get parameter from parameter list using tag id.
     *  
     * @param t_tag tag of parameter to retrieve.
     * @return Pointer to object, null if not found.
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

    size_t GetConstant(const char* t_tag) const
    {
        if(this->m_Constants.find(t_tag) != this->m_Constants.end())
        {
            return std::stoi(this->m_Constants.at(t_tag));
        }
        else
        {
            return 0;
        }
    }

public:
    /**
     * @brief Constructor.
     * 
     * @param t_gpu gpu ressources to be used.
     * @param t_name kernel operation name.
     * @param t_tags tag list of parameters.
     */
    Kernel(std::shared_ptr<GPU> t_gpu, const char* t_name, const std::vector<std::string>& t_tags);

    /**
     * @brief Destructor.
     */
    ~Kernel();

    /**
     * @brief Virtual method execute, to be implemented in the subclass.
     *        The method is a placeholder for host code needed to run the kernel.
     */
    virtual void Execute() = 0;
};

} // namespace cle

#endif // __cleKernel_hpp