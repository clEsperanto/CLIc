#ifndef __UTILS_H
#define __UTILS_H


#include <vector>
#include <array>
#include <iostream>
#include <limits>

#include <cmath>

template<class type>
bool IsDifferent(std::vector<type>& output, std::vector<type>& valid)
{
    if (output.size() != valid.size())
    {
        std::cerr << "[FAILED] : output size does not match." << std::endl;
        return true;
    }
    float difference = 0;
    for (auto it_output = output.begin(), it_valid = valid.begin(); 
              it_output != output.end() && it_valid != valid.end(); ++it_output, ++it_valid)
    {
        difference += std::abs( roundf(static_cast<float>(*it_output) * 10000)/10000 - roundf(static_cast<float>(*it_valid) * 10000)/10000 );
    }
    if (difference > std::numeric_limits<float>::epsilon())
    {
        std::cerr << "[FAILED] : difference = " << difference << std::endl;
        return true;
    }
    else
    {
        return false;
    }
}

template<class type>
void PrintData(std::vector<type>& array, std::array<size_t,3> shape)
{
    std::cout.precision(std::numeric_limits<float>::max_digits10);
    for (auto it = array.begin(); it != array.end(); ++it)
    {
        if( (it-array.begin())%shape[0] ==0 && it!=array.begin())
        {
            std::cout << "\n";
        }
        if( (it-array.begin())%(shape[0]*shape[1]) ==0 && it!=array.begin())
        {
            std::cout << "\n";
        }
        std::cout << *it << " ";
    }
    std::cout << "\n";
}


#endif // __UTILS_H