
#include "cleKernel.hpp"

namespace cle
{

void Kernel::ManageDimensions(std::string tag)
{
    if(m_ParameterList.size() > 1 && m_Sources.size() > 1)
    {
        auto it = m_ParameterList.find(tag);
        if (it != m_ParameterList.end())
        {
            if(m_ParameterList.at(tag)->GetDimension() == 3)
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
            std::cerr << "Kernel : Error in managing data dimensions, could not find \"dst\" tag in Parameters list." << std::endl;
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
    std::string defines = "\n";  

    // image size independent from kernel compilation
    defines = defines + "\n#define GET_IMAGE_WIDTH(image_key) IMAGE_SIZE_ ## image_key ## _WIDTH";
    defines = defines + "\n#define GET_IMAGE_HEIGHT(image_key) IMAGE_SIZE_ ## image_key ## _HEIGHT";
    defines = defines + "\n#define GET_IMAGE_DEPTH(image_key) IMAGE_SIZE_ ## image_key ## _DEPTH";
    defines = defines + "\n";   

    // loop on parameters
    for (auto itr = m_ParameterList.begin(); itr != m_ParameterList.end(); ++itr)
    {
        // only non-scalar argument are relevant
        if (!itr->second->IsObjectType("scalar"))  
        {     

            // compute defines adapted to current argument (type, size, shape, etc.)
            std::string tagObject = itr->first;                      // get argument tag (e.g. dst, destination, output)
            std::string objectType = itr->second->GetObjectType();   // buffer or image
            std::string dataType = itr->second->GetDataType();       // float, double, char, int, etc.
            std::string abbrType = TypeAbbr(dataType.c_str());       // shorten type f->float, i->int, d->double, etc.
            int ndim = itr->second->GetDimension();
            std::string img_type_name;
            if(tagObject.compare("dst") == 0 || tagObject.compare("destination") == 0 || tagObject.compare("output") == 0)
            {
                img_type_name = "__write_only image" + std::to_string(ndim) + "d_t";
            }
            else
            {
                img_type_name = "__read_only image" + std::to_string(ndim) + "d_t";
            }
            std::string  pos_type, pos, img_dim; 
            if( ndim == 1 )
            {
                img_dim = "2";
                pos_type = "int2";
                pos = "(pos0, 0)";
            }
            else if (ndim == 2)
            { 
                img_dim = "2";
                pos_type = "int2";
                pos = "(pos0, pos1)";
            } 
            else if (ndim == 3)
            { 
                img_dim = "3";
                pos_type = "int4";
                pos = "(pos0, pos1, pos2, 0)";
            } 
            std::string size_parameters = ""; // todo: image size independent kernel 

            std::string common_header = "\n";
            common_header = common_header + "\n#define CONVERT_" + tagObject + "_PIXEL_TYPE clij_convert_" + dataType + "_sat";
            common_header = common_header + "\n#define IMAGE_" + tagObject + "_PIXEL_TYPE " + dataType + "";
            common_header = common_header + "\n#define POS_" + tagObject + "_TYPE " + pos_type;
            common_header = common_header + "\n#define POS_" + tagObject + "_INSTANCE(pos0,pos1,pos2,pos3) (" + pos_type + ")" + pos;

            std::string size_header = "\n";
            size_header = size_header + "\n#define IMAGE_SIZE_" + tagObject + "_WIDTH " + std::to_string(itr->second->GetWidth());
            size_header = size_header + "\n#define IMAGE_SIZE_" + tagObject + "_HEIGHT " + std::to_string(itr->second->GetHeight());
            size_header = size_header + "\n#define IMAGE_SIZE_" + tagObject + "_DEPTH " + std::to_string(itr->second->GetDepth());

            defines = defines + size_header;
            if (itr->second->IsObjectType("buffer"))
            {
                std::string array_header = common_header + "\n";
                array_header = array_header + "\n#define IMAGE_" + tagObject + "_TYPE " + size_parameters + " __global " + dataType + "*";
                array_header = array_header + "\n#define READ_" + tagObject + "_IMAGE(a,b,c) read_buffer" + img_dim + "d" + abbrType + "(GET_IMAGE_WIDTH(a),GET_IMAGE_HEIGHT(a),GET_IMAGE_DEPTH(a),a,b,c)";
                array_header = array_header + "\n#define WRITE_" + tagObject + "_IMAGE(a,b,c) write_buffer" + img_dim + "d" + abbrType + "(GET_IMAGE_WIDTH(a),GET_IMAGE_HEIGHT(a),GET_IMAGE_DEPTH(a),a,b,c)";
                defines = defines + array_header;
            }   
            else
            {
                std::string image_header = common_header + "\n";
                image_header = image_header + "\n#define IMAGE_" + tagObject + "_TYPE " + size_parameters + " " + img_type_name;
                image_header = image_header + "\n#define READ_" + tagObject + "_IMAGE(a,b,c) read_image" + abbrType + "(a,b,c)";
                image_header = image_header + "\n#define WRITE_" + tagObject + "_IMAGE(a,b,c) write_image" + abbrType + "(a,b,c)";
                defines = defines + image_header;
            }
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
    // if (strncmp("double", str, size) == 0) return "d";    // Missing definition type in preamble
    if (strncmp("float", str, size) == 0)  return "f";
    if (strncmp("char", str, size) == 0)   return "c";
    if (strncmp("uchar", str, size) == 0)  return "uc";
    // if (strncmp("int", str, size) == 0)    return "i";    // Missing definition type in preamble
    // if (strncmp("uint", str, size) == 0)   return "ui";   // Missing definition type in preamble
    // if (strncmp("short", str, size) == 0)    return "s";  // Missing definition type in preamble
    // if (strncmp("ushort", str, size) == 0)   return "us"; // Missing definition type in preamble
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
                std::string mem_type = (object->GetObject().getInfo<CL_MEM_TYPE>()  ==  CL_MEM_OBJECT_BUFFER) ? "true" :  "false";
                try
                {
                    this->m_Kernel.setArg(index, object->GetObject());
                }
                catch(cl::Error& e)
                {
                    std::cerr << "Kernel : Fail to set argument to kernel." << std::endl;
                    std::cerr << "\tException caught! " << e.what() << " error code " << e.err() << std::endl;
                }
                for (size_t i = 0; i < 3; i++)
                {
                    size_t tempDim = size_t(object->GetShape()[i]);
                    m_GlobalRange[i] = std::max(m_GlobalRange[i], tempDim);
                }
            }
            else if (m_ParameterList.at(tag)->IsObjectType("image"))
            {    
                std::shared_ptr<cle::Image2D> object = std::dynamic_pointer_cast<cle::Image2D>(m_ParameterList.at(tag));
                std::string mem_type = (object->GetObject().getInfo<CL_MEM_TYPE>()  ==  CL_MEM_OBJECT_IMAGE2D) ? "true" :  "false";                
                try
                {
                    this->m_Kernel.setArg(index, object->GetObject());
                }
                catch(cl::Error& e)
                {
                    std::cerr << "Kernel : Fail to set argument to kernel." << std::endl;
                    std::cerr << "\tException caught! " << e.what() << " error code " << e.err() << std::endl;
                }
                for (size_t i = 0; i < 3; i++)
                {
                    size_t tempDim = size_t(object->GetShape()[i]);
                    m_GlobalRange[i] = std::max(m_GlobalRange[i], tempDim);
                }
            }
            else if (m_ParameterList.at(tag)->IsObjectType("scalar"))  // only float and int used in kernel for now
            {    
                if (m_ParameterList.at(tag)->IsDataType("float")) {
                    std::shared_ptr< cle::Scalar<float> > object = std::dynamic_pointer_cast< cle::Scalar<float> >(m_ParameterList.at(tag));
                    this->m_Kernel.setArg(index, object->GetObject()); 
                }
                else if (m_ParameterList.at(tag)->IsDataType("int")) {
                    std::shared_ptr< cle::Scalar<int> > object = std::dynamic_pointer_cast< cle::Scalar<int> >(m_ParameterList.at(tag));
                    this->m_Kernel.setArg(index, object->GetObject()); 
                }
                // else if (m_ParameterList.at(tag)->IsDataType("double")) {
                //     std::shared_ptr< cle::Scalar<double> > object = std::dynamic_pointer_cast< cle::Scalar<double> >(m_ParameterList.at(tag));
                //     this->m_Kernel.setArg(index, object->GetObject()); 
                // }
                // else if (m_ParameterList.at(tag)->IsDataType("char")) {
                //     std::shared_ptr< cle::Scalar<char> > object = std::dynamic_pointer_cast< cle::Scalar<char> >(m_ParameterList.at(tag));
                //     this->m_Kernel.setArg(index, object->GetObject()); 
                // }
                // else if (m_ParameterList.at(tag)->IsDataType("short")) {
                //     std::shared_ptr< cle::Scalar<short> > object = std::dynamic_pointer_cast< cle::Scalar<short> >(m_ParameterList.at(tag));
                //     this->m_Kernel.setArg(index, object->GetObject()); 
                // }
                // else if (m_ParameterList.at(tag)->IsDataType("unsigned int")) {
                //     std::shared_ptr< cle::Scalar<unsigned int> > object = std::dynamic_pointer_cast< cle::Scalar<unsigned int> >(m_ParameterList.at(tag));
                //     this->m_Kernel.setArg(index, object->GetObject()); 
                // }
                // else if (m_ParameterList.at(tag)->IsDataType("unsigned char")) {
                //     std::shared_ptr< cle::Scalar<unsigned char> > object = std::dynamic_pointer_cast< cle::Scalar<unsigned char> >(m_ParameterList.at(tag));
                //     this->m_Kernel.setArg(index, object->GetObject());
                // }
                // else if (m_ParameterList.at(tag)->IsDataType("unsigned short")) {
                //     std::shared_ptr< cle::Scalar<unsigned short> > object = std::dynamic_pointer_cast< cle::Scalar<unsigned short> >(m_ParameterList.at(tag));
                //     this->m_Kernel.setArg(index, object->GetObject()); 
                // }
            }
        }
    }
}

void Kernel::AddObject(Buffer &o, std::string t)
{
    if(std::find(m_TagList.begin(), m_TagList.end(), t.c_str()) != m_TagList.end())
    {
        auto it = m_ParameterList.find(t.c_str()); 
        if (it != m_ParameterList.end())
        {
            it->second = std::make_shared< cle::Buffer >(o);
        }
        else    
        {
            m_ParameterList.insert(std::make_pair(t.c_str(), std::make_shared< cle::Buffer >(o)));
        }
    }
    else
    {
        std::cerr << "Kernel : Invalid tag '" << t << "' used to add buffer." << std::endl;
    }
}

void Kernel::AddObject(Image2D &o, std::string t)
{
    if(std::find(m_TagList.begin(), m_TagList.end(), t.c_str()) != m_TagList.end())
    {
        auto it = m_ParameterList.find(t.c_str()); 
        if (it != m_ParameterList.end())
        {
            it->second = std::make_shared< cle::Image2D >(o);
        }
        else    
        {
            m_ParameterList.insert(std::make_pair(t.c_str(), std::make_shared< cle::Image2D >(o)));
        }
    }
    else
    {
        std::cerr << "Kernel : Invalid tag '" << t << "' used to add buffer." << std::endl;
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
        std::cerr << "Kernel : Invalid tag '" << t << "' used to add int scalar." << std::endl;
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
        std::cerr << "Kernel : Invalid tag '" << t << "' used to add float scalar." << std::endl;
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
                std::cerr << "Kernel : Fail to create program." << std::endl;
                std::cerr << "\tException caught! " << e.what() << " error code " << e.err() << std::endl;
                std::cerr << sources << std::endl;
            }
            try
            {
                this->m_Program.build({this->m_gpu->GetDeviceManager().GetDevice()});
            }
            catch(cl::Error& e)
            {
                std::cerr << "Kernel : Fail to build program from source." << std::endl;
                std::cerr << "\tException caught! " << e.what() << " error code " << e.err() << std::endl;
                std::cerr << "build log:" << std::endl;
                std::cerr << this->m_Program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(this->m_gpu->GetDeviceManager().GetDevice()) << std::endl;
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
            std::cerr << "Kernel : Fail to create kernel. " << fullName.c_str() << std::endl;
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
        std::cerr << "Kernel : Fail to enqueue kernel." << std::endl;
        std::cerr << "\tException caught! " << e.what() << " error code " << e.err() << '\n';
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
