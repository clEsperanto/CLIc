
#include "cleKernel.h"

namespace cle
{

void Kernel::ManageDimensions(std::string tag)
{
    if(m_ParameterList.size() > 1)
    {
        auto it = m_ParameterList.find(tag);
        if (it != m_ParameterList.end())
        {
            std::shared_ptr<cle::Buffer> object = std::dynamic_pointer_cast<cle::Buffer>(m_ParameterList.at(tag));
            if(object->GetDimensions()[2] > 1)
            {
                m_DimensionTag = "_3d";
            }
            else
            {
                m_DimensionTag = "_2d";
            }
        }
        else
        {
            std::cerr << "Error ManageDimensions() : Could not find \"dst\" in Parameters list." << std::endl;
        }
    }
}
    
std::string Kernel::LoadPreamble()
{
    std::string preamble = 
        #include "cle_preamble.h"
    ;
    return preamble;
}

std::string Kernel::LoadSources()
{  
    std::string kernel_source = "";
    if(!m_Sources.empty())
    {
        if(m_Sources.size() == 1)
        {
            kernel_source = m_Sources.begin()->second.c_str();
        }
        else
        {
            kernel_source = m_Sources[(m_KernelName + m_DimensionTag).c_str()];
        }
    }
    return kernel_source;
}

std::string Kernel::LoadDefines()
{
    std::string defines;
    defines = defines + "\n#define GET_IMAGE_WIDTH(image_key) IMAGE_SIZE_ ## image_key ## _WIDTH";
    defines = defines + "\n#define GET_IMAGE_HEIGHT(image_key) IMAGE_SIZE_ ## image_key ## _HEIGHT";
    defines = defines + "\n#define GET_IMAGE_DEPTH(image_key) IMAGE_SIZE_ ## image_key ## _DEPTH";
    defines = defines + "\n";   
    // loop on parameters
    for (auto itr = m_ParameterList.begin(); itr != m_ParameterList.end(); ++itr)
    {
        if (itr->second->IsObjectType(LightObject::cleBuffer))
        {                
            // get object information
            std::shared_ptr<cle::Buffer> object = std::dynamic_pointer_cast<cle::Buffer>(itr->second);
            std::string tagObject = itr->first;
            std::string objectType = object->GetObjectType();
            std::string dataType = object->GetDataType();
            std::string abbrType = TypeAbbr(dataType);
            // image type handling
            defines = defines + "\n#define CONVERT_" + tagObject + "_PIXEL_TYPE clij_convert_" + dataType + "_sat";
            defines = defines + "\n#define IMAGE_" + tagObject + "_TYPE __global " + dataType + "*";
            defines = defines + "\n#define IMAGE_" + tagObject + "_PIXEL_TYPE " + dataType;
            // image size handling
            if (object->GetDimensions()[2] > 1)
            {
                defines = defines + "\n#define IMAGE_SIZE_" + tagObject + "_WIDTH " + std::to_string(object->GetDimensions()[0]);
                defines = defines + "\n#define IMAGE_SIZE_" + tagObject + "_HEIGHT " + std::to_string(object->GetDimensions()[1]);
                defines = defines + "\n#define IMAGE_SIZE_" + tagObject + "_DEPTH " + std::to_string(object->GetDimensions()[2]);
            }
            else
            {
                if (object->GetDimensions()[1] > 1)
                {
                    defines = defines + "\n#define IMAGE_SIZE_" + tagObject + "_WIDTH " + std::to_string(object->GetDimensions()[0]);
                    defines = defines + "\n#define IMAGE_SIZE_" + tagObject + "_HEIGHT " + std::to_string(object->GetDimensions()[1]);
                }
                else
                {
                    defines = defines + "\n#define IMAGE_SIZE_" + tagObject + "_WIDTH " + std::to_string(object->GetDimensions()[0]);
                    defines = defines + "\n#define IMAGE_SIZE_" + tagObject + "_HEIGHT 1";
                }
                defines = defines + "\n#define IMAGE_SIZE_" + tagObject + "_DEPTH 1";
            }
            // position (dimensionality) handling
            if (object->GetDimensions()[2] == 1)
            {
                defines = defines + "\n#define POS_" + tagObject + "_TYPE int2";
                if (object->GetDimensions()[1] == 1) // 1D
                {
                    defines = defines + "\n#define POS_" + tagObject + "_INSTANCE(pos0,pos1,pos2,pos3) (int2)(pos0, 0)";
                }
                else // 2D
                {
                    defines = defines + "\n#define POS_" + tagObject + "_INSTANCE(pos0,pos1,pos2,pos3) (int2)(pos0, pos1)";
                }
            }
            else // 3/4D
            {
                defines = defines + "\n#define POS_" + tagObject + "_TYPE int4";
                defines =
                    defines + "\n#define POS_" + tagObject + "_INSTANCE(pos0,pos1,pos2,pos3) (int4)(pos0, pos1, pos2, 0)";
            }
            // read/write images
            std::string sdim = (object->GetDimensions()[2] == 1) ? "2" : "3";
            defines = defines + "\n#define READ_" + tagObject + "_IMAGE(a,b,c) read_buffer" + sdim + "d" + abbrType +
                    "(GET_IMAGE_WIDTH(a),GET_IMAGE_HEIGHT(a),GET_IMAGE_DEPTH(a),a,b,c)";
            defines = defines + "\n#define WRITE_" + tagObject + "_IMAGE(a,b,c) write_buffer" + sdim + "d" + abbrType +
                    "(GET_IMAGE_WIDTH(a),GET_IMAGE_HEIGHT(a),GET_IMAGE_DEPTH(a),a,b,c)";
            defines = defines + "\n";    
        } // end check if not Scalar Object
    } // end of for loop on hashmap
    return defines;
}

std::string Kernel::GenerateSources()
{
    std::string defines_src = LoadDefines();
    std::string kernel_src = LoadSources();
    std::string preambule_src = LoadPreamble();
    std::string source = defines_src + "\n" + preambule_src + "\n" + kernel_src;
    return source;
}

std::string Kernel::TypeAbbr(const std::string type) const
{
    if (type.compare("float") == 0)
    {
        return "f";
    }
    else if (type.compare("char") == 0)
    {
        return  "c";
    }
    else if (type.compare("uchar") == 0)
    {
        return  "uc";
    }
    else if (type.compare("int") == 0)
    {
        return  "i";
    }
    else if (type.compare("uint") == 0)
    {
        return  "ui";
    }
    return ""; 
}

void Kernel::SetArguments()
{
    cl_int clError = CL_SUCCESS;
    for(auto it = m_TagList.begin(); it != m_TagList.end(); it++ )
    {
        size_t index = it - m_TagList.begin();
        if(m_ParameterList.find(it->c_str()) != m_ParameterList.end())
        {
            std::string tag = it->c_str();
            if (m_ParameterList.at(tag)->IsObjectType(LightObject::cleBuffer))
            {    
                std::shared_ptr<cle::Buffer> object = std::dynamic_pointer_cast<cle::Buffer>(m_ParameterList.at(tag));
                this->m_Kernel.setArg(index, object->GetObject());
                for (size_t i = 0; i < 3; i++)
                {
                    size_t tempDim = size_t(object->GetDimensions()[i]);
                    m_GlobalRange[i] = std::max(m_GlobalRange[i], tempDim);
                }
            }
            else if (m_ParameterList.at(tag)->IsObjectType(LightObject::cleFloat))
            {    
                std::shared_ptr<cle::Float> object = std::dynamic_pointer_cast<cle::Float>(m_ParameterList.at(tag));
                this->m_Kernel.setArg(index, object->GetObject());
            }
            else if (m_ParameterList.at(tag)->IsObjectType(LightObject::cleInt))
            {   
                std::shared_ptr<cle::Int> object = std::dynamic_pointer_cast<cle::Int>(m_ParameterList.at(tag));
                this->m_Kernel.setArg(index, object->GetObject());
            }
        }
    }
}

void Kernel::AddObject(Buffer o, std::string t)
{
    if(std::find(m_TagList.begin(), m_TagList.end(), t.c_str()) != m_TagList.end())
    {
        auto it = m_ParameterList.find(t.c_str()); 
        if (it != m_ParameterList.end())
        {
            it->second = std::make_shared<Buffer>(o);
        }
        else    
        {
            m_ParameterList.insert(std::make_pair(t, std::make_shared<Buffer>(o)));
        }
    }
    else
    {
        std::cerr << "Error: Invalid buffer parameter tag" << std::endl;
    }
}

void Kernel::AddObject(int o, std::string t)
{
    if(std::find(m_TagList.begin(), m_TagList.end(), t.c_str()) != m_TagList.end())
    {
        Int x(o);
        auto it = m_ParameterList.find(t.c_str()); 
        if (it != m_ParameterList.end())
        {
            it->second = std::make_shared<Int>(x);
        }
        else    
        {
            m_ParameterList.insert(std::make_pair(t, std::make_shared<Int>(x)));
        }
    }
    else
    {
        std::cerr << "Error: Invalid Int parameter tag" << std::endl;
    }
}

void Kernel::AddObject(float o, std::string t)
{
    if(std::find(m_TagList.begin(), m_TagList.end(), t.c_str()) != m_TagList.end())
    {
        Float x(o);
        auto it = m_ParameterList.find(t.c_str()); 
        if (it != m_ParameterList.end())
        {
            it->second = std::make_shared<Float>(x);
        }
        else    
        {
            m_ParameterList.insert(std::make_pair(t, std::make_shared<Float>(x)));
        }
    }
    else
    {
        std::cerr << "Error: Invalid Float parameter tag" << std::endl;
    }
}

void Kernel::BuildProgramKernel()
{
    std::string sources = GenerateSources();
    std::hash<std::string> hasher;
    size_t source_hash = hasher(sources);
    if(m_CurrentHash != source_hash)  
    {
        if(m_gpu.FindProgram(source_hash))
        {
            this->m_Program = m_gpu.GetProgram(source_hash);
            this->m_CurrentHash = source_hash;
        }
        else
        {
            this->m_Program = cl::Program(this->m_gpu.GetContextManager().GetContext(), sources);
            if(this->m_Program.build({this->m_gpu.GetDeviceManager().GetDevice()}) != CL_SUCCESS)
            {
                std::cerr << "Kernel : Fail to create program from source." << std::endl;
                std::cerr << "\tbuild log:" << std::endl;
                std::cerr << this->m_Program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(
                                this->m_gpu.GetDeviceManager().GetDevice()) 
                                << std::endl;
            }
            m_CurrentHash = source_hash;
            this->m_gpu.AddProgram(this->m_Program, m_CurrentHash);
        }
        std::string fullName = this->m_KernelName + this->m_DimensionTag;
        this->m_Kernel = cl::Kernel(this->m_Program, fullName.c_str());
    }
}

void Kernel::EnqueueKernel()
{
    cl::NDRange globalND(this->m_GlobalRange[0], this->m_GlobalRange[1], this->m_GlobalRange[2]);    
    this->m_gpu.GetCommandQueueManager().GetCommandQueue().enqueueNDRangeKernel(
        this->m_Kernel, cl::NullRange, globalND, cl::NullRange
        );
    this->m_gpu.GetCommandQueueManager().GetCommandQueue().finish();
}

Kernel::Kernel(GPU& gpu, std::string kernel, std::vector<std::string> tags) : m_gpu(gpu)
{
    this->m_KernelName = kernel;
    this->m_TagList = tags;
}

Kernel::~Kernel()
{}

} // namespace cle
