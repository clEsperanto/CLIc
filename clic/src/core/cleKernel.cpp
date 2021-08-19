
#include "cleKernel.hpp"

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
            if(object->GetShape()[2] > 1)
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
        // if (itr->second->IsObjectType(LightObject::cleBuffer))
        if (itr->second->IsObjectType("buffer"))
        {              
            // get object information
            std::shared_ptr<cle::Buffer> object = std::dynamic_pointer_cast<cle::Buffer>(itr->second);
            std::string tagObject = itr->first;
            std::string objectType = object->GetObjectType();
            std::string dataType = object->GetDataType();
            std::string abbrType = TypeAbbr(dataType.c_str());
            // image type handling
            defines = defines + "\n#define CONVERT_" + tagObject + "_PIXEL_TYPE clij_convert_" + dataType + "_sat";
            defines = defines + "\n#define IMAGE_" + tagObject + "_TYPE __global " + dataType + "*";
            defines = defines + "\n#define IMAGE_" + tagObject + "_PIXEL_TYPE " + dataType;
            // image size handling
            if (object->GetShape()[2] > 1)
            {
                defines = defines + "\n#define IMAGE_SIZE_" + tagObject + "_WIDTH " + std::to_string(object->GetShape()[0]);
                defines = defines + "\n#define IMAGE_SIZE_" + tagObject + "_HEIGHT " + std::to_string(object->GetShape()[1]);
                defines = defines + "\n#define IMAGE_SIZE_" + tagObject + "_DEPTH " + std::to_string(object->GetShape()[2]);
            }
            else
            {
                if (object->GetShape()[1] > 1)
                {
                    defines = defines + "\n#define IMAGE_SIZE_" + tagObject + "_WIDTH " + std::to_string(object->GetShape()[0]);
                    defines = defines + "\n#define IMAGE_SIZE_" + tagObject + "_HEIGHT " + std::to_string(object->GetShape()[1]);
                }
                else
                {
                    defines = defines + "\n#define IMAGE_SIZE_" + tagObject + "_WIDTH " + std::to_string(object->GetShape()[0]);
                    defines = defines + "\n#define IMAGE_SIZE_" + tagObject + "_HEIGHT 1";
                }
                defines = defines + "\n#define IMAGE_SIZE_" + tagObject + "_DEPTH 1";
            }
            // position (dimensionality) handling
            if (object->GetShape()[2] == 1)
            {
                defines = defines + "\n#define POS_" + tagObject + "_TYPE int2";
                if (object->GetShape()[1] == 1) // 1D
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
                defines = defines + "\n#define POS_" + tagObject + "_INSTANCE(pos0,pos1,pos2,pos3) (int4)(pos0, pos1, pos2, 0)";
            }
            // read/write images
            std::string sdim = (object->GetShape()[2] == 1) ? "2" : "3";
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

std::string Kernel::TypeAbbr(const char* str) const
{
    size_t size = strlen(str);
    if (strncmp("double", str, size) == 0) return "d";
    if (strncmp("float", str, size) == 0)  return "f";
    if (strncmp("char", str, size) == 0)   return "c";
    if (strncmp("uchar", str, size) == 0)  return "uc";
    if (strncmp("int", str, size) == 0)    return "i";
    if (strncmp("uint", str, size) == 0)   return "ui";
    return "?"; 
}

void Kernel::SetArguments()
{
    for(auto it = m_TagList.begin(); it != m_TagList.end(); it++ )
    {
        size_t index = it - m_TagList.begin();
        if(m_ParameterList.find(it->c_str()) != m_ParameterList.end())
        {
            std::string tag = it->c_str();
            if (m_ParameterList.at(tag)->IsObjectType("buffer"))
            {    
                std::shared_ptr<cle::Buffer> object = std::dynamic_pointer_cast<cle::Buffer>(m_ParameterList.at(tag));
                this->m_Kernel.setArg(index, object->GetObject());
                for (size_t i = 0; i < 3; i++)
                {
                    size_t tempDim = size_t(object->GetShape()[i]);
                    m_GlobalRange[i] = std::max(m_GlobalRange[i], tempDim);
                }
            }
            else if (m_ParameterList.at(tag)->IsObjectType("scalar"))
            {    
                if (m_ParameterList.at(tag)->IsDataType("float")) {
                    std::shared_ptr< cle::Scalar<float> > object = std::dynamic_pointer_cast< cle::Scalar<float> >(m_ParameterList.at(tag));
                    this->m_Kernel.setArg(index, object->GetObject()); 
                }
                else if (m_ParameterList.at(tag)->IsDataType("double")) {
                    std::shared_ptr< cle::Scalar<double> > object = std::dynamic_pointer_cast< cle::Scalar<double> >(m_ParameterList.at(tag));
                    this->m_Kernel.setArg(index, object->GetObject()); 
                }
                else if (m_ParameterList.at(tag)->IsDataType("int")) {
                    std::shared_ptr< cle::Scalar<int> > object = std::dynamic_pointer_cast< cle::Scalar<int> >(m_ParameterList.at(tag));
                    this->m_Kernel.setArg(index, object->GetObject()); 
                }
                else if (m_ParameterList.at(tag)->IsDataType("char")) {
                    std::shared_ptr< cle::Scalar<char> > object = std::dynamic_pointer_cast< cle::Scalar<char> >(m_ParameterList.at(tag));
                    this->m_Kernel.setArg(index, object->GetObject()); 
                }
                else if (m_ParameterList.at(tag)->IsDataType("unsigned int")) {
                    std::shared_ptr< cle::Scalar<unsigned int> > object = std::dynamic_pointer_cast< cle::Scalar<unsigned int> >(m_ParameterList.at(tag));
                    this->m_Kernel.setArg(index, object->GetObject()); 
                }
                else if (m_ParameterList.at(tag)->IsDataType("unsigned char")) {
                    std::shared_ptr< cle::Scalar<unsigned char> > object = std::dynamic_pointer_cast< cle::Scalar<unsigned char> >(m_ParameterList.at(tag));
                    this->m_Kernel.setArg(index, object->GetObject());
                }

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
            m_ParameterList.insert(std::make_pair(t.c_str(), std::make_shared<Buffer>(o)));
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
        cle::Scalar<int> x(o);
        auto it = m_ParameterList.find(t.c_str()); 
        if (it != m_ParameterList.end())
        {
            it->second = std::make_shared< cle::Scalar<int> >(x);
        }
        else    
        {
            m_ParameterList.insert(std::make_pair(t.c_str(), std::make_shared< cle::Scalar<int> >(x)));
        }
    }
    else
    {
        std::cerr << "Error: Invalid Scalar parameter tag" << std::endl;
    }
}

void Kernel::AddObject(float o, std::string t)
{
    if(std::find(m_TagList.begin(), m_TagList.end(), t.c_str()) != m_TagList.end())
    {
        cle::Scalar<float> x(o);
        auto it = m_ParameterList.find(t.c_str()); 
        if (it != m_ParameterList.end())
        {
            it->second = std::make_shared< cle::Scalar<float> >(x);
        }
        else    
        {
            m_ParameterList.insert(std::make_pair(t.c_str(), std::make_shared< Scalar<float> >(x)));
        }
    }
    else
    {
        std::cerr << "Error: Invalid Scalar parameter tag" << std::endl;
    }
}

void Kernel::BuildProgramKernel()
{
    std::string sources = GenerateSources();
    std::hash<std::string> hasher;
    size_t source_hash = hasher(sources);
    if(m_CurrentHash != source_hash)  
    {
        if(this->m_gpu->FindProgram(source_hash))
        {
            this->m_Program = this->m_gpu->GetProgram(source_hash);
            this->m_CurrentHash = source_hash;
        }
        else
        {
            try
            {
                this->m_Program = cl::Program(this->m_gpu->GetContextManager().GetContext(), sources);
            }
            catch(cl::Error& e)
            {
                std::cerr << "Fail to create program ..." << std::endl;
                std::cerr << "\tException caught! " << e.what() << " error code " << e.err() << std::endl;
                std::cerr << sources << std::endl;
            }
            try
            {
                this->m_Program.build({this->m_gpu->GetDeviceManager().GetDevice()});
            }
            catch(cl::Error& e)
            {
                std::cerr << "Fail to build program from source ..." << std::endl;
                std::cerr << "\tException caught! " << e.what() << " error code " << e.err() << std::endl;
                std::string build_log = this->m_Program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(this->m_gpu->GetDeviceManager().GetDevice());
                std::cerr << "\tbuild log:" << std::endl;
                std::cerr << build_log << std::endl;
            }
            m_CurrentHash = source_hash;
            this->m_gpu->AddProgram(this->m_Program, m_CurrentHash);
        }
        std::string fullName = this->m_KernelName + this->m_DimensionTag;
        try
        {
            this->m_Kernel = cl::Kernel(this->m_Program, fullName.c_str());
        }
        catch(cl::Error& e)
        {
            std::cerr << "Fail to create kernel: " << fullName.c_str() << std::endl;
            std::cerr << "\tException caught! " << e.what() << " error code " << e.err() << std::endl;
        }
    }
}

void Kernel::EnqueueKernel()
{
    cl::NDRange globalND(this->m_GlobalRange[0], this->m_GlobalRange[1], this->m_GlobalRange[2]);  
    try
    {
        this->m_gpu->GetCommandQueueManager().GetCommandQueue().enqueueNDRangeKernel(
            this->m_Kernel, cl::NullRange, globalND, cl::NullRange
        );
    }
    catch(const cl::Error& e)
    {
        std::cerr << "Fail to enqueue kernel ..." << std::endl;
        std::cerr << "\tException caught! " << e.what() << " -> " << e.err() << '\n';
    }
    this->m_gpu->GetCommandQueueManager().GetCommandQueue().finish();
}

Kernel::Kernel(std::shared_ptr<GPU> gpu, std::string kernel, std::vector<std::string> tags) : m_gpu(gpu)
{
    this->m_KernelName = kernel;
    this->m_TagList = tags;
}

Kernel::~Kernel()
{}

} // namespace clic
