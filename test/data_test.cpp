
#include "CLE.h"

#include "cleLightObject.h"
#include "cleFloat.h"
#include "cleInt.h"
#include "cleBuffer.h"


#include <map>
#include <random>

#include "cleAddImageAndScalarKernel.h"


using namespace cle;

int main(int argc, char **argv)
{
    std::cout << "Initialisation of the gpu and data structure" << std::endl;
    cle::GPU gpu;
    cle::CLE cle(gpu);
    std::map<std::string, LightObject*> paramList;

    std::cout << "Initialisation of cleFloat" << std::endl;
    float x = 1.28;
    Float x_value(x);
    
    std::cout << "Initialisation of cleInt" << std::endl;
    int y = 100;
    Int y_value(y);
    
    std::cout << "Initialisation of cleBuffer" << std::endl;
    unsigned int width (10), height (5), depth (13);
    float* input_data = new float[width*height*depth];
    std::default_random_engine generator;
    std::normal_distribution<float> distribution(5.0,2.0);
    for (size_t i = 0; i < width*height*depth; i++)
    {
        input_data[i] = distribution(generator);
    }
    Image<float> input_img (input_data, width, height, depth, "float");   
    Buffer b_value = cle.Push<float>(input_img);
    Buffer c_value = cle.Create<float>(b_value);

    std::cout << "Fill data structure parameter" << std::endl;
    paramList.insert(std::make_pair("x_value", &x_value));
    paramList.insert(std::make_pair("y_value", &y_value));
    paramList.insert(std::make_pair("b_value", &b_value));
    paramList.insert(std::make_pair("c_value", &c_value));
    std::cout << "Structure number of elem = " << paramList.size() << std::endl;

    for (auto itr = paramList.begin(); itr != paramList.end(); ++itr)
    {
        std::cout << itr->first;
        std::cout << " is a " << itr->second->GetObjectType();
        if (itr->second->IsObject("cleFloat"))
        {
            Float* new_x_value = dynamic_cast<Float*>(itr->second);
            std::cout << " = " << new_x_value->GetData() << std::endl;
        }
        else if (itr->second->IsObject("cleInt"))
        {
            Int* new_y_value = dynamic_cast<Int*>(itr->second);
            std::cout << " = " << new_y_value->GetData() << std::endl;
        }
        else if (itr->second->IsObject("cleBuffer"))
        {
            Buffer* new_b_value = dynamic_cast<Buffer*>(itr->second);
            std::cout << " = " << new_b_value->GetData() << " of type " << new_b_value->GetDataType();
            std::cout << ", dim(" << new_b_value->GetDimensions()[0] <<","<< new_b_value->GetDimensions()[1] <<","<< new_b_value->GetDimensions()[2]; 
            std::cout << ")" << std::endl;
        }
        else
        {
            std::cout << " = (unknow object type)" << std::endl;
        }
    }

    return EXIT_SUCCESS;
}