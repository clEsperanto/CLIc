
#include <random>
#include <iostream>
#include <vector>

#include "cleGPU.hpp"
#include "cleBuffer.hpp"


/**
 * Main test function
 *
 */

int main(int argc, char **argv)
{
    
    {
        using type = float;
        auto gpu = std::make_shared<cle::GPU>();

        std::array<size_t,3> dims = {10, 1, 1};
        std::vector<type> A (dims[0]*dims[1]*dims[2]); 
        std::fill (A.begin(),A.end(), 10.0f);

        cle::Buffer buff_A = gpu->CreateBuffer<type>(dims);
        cle::Buffer buff_B = gpu->PushBuffer<type>(A, dims);
        std::vector<type> C = gpu->Pull<type>(buff_B);
        
        float difference = 0;
        for( auto it1 = A.begin(), it2 = C.begin(); 
            it1 != A.end() && it2 != C.end(); ++it1, ++it2)
        {
            difference += std::abs(*it1 - *it2);
        }
        if (difference > std::numeric_limits<type>::epsilon())
        {
            std::cout << "test cleImage {Create, Push, Pull} fail - diff = " << difference << std::endl;
            return EXIT_FAILURE;
        }
        else
        {
            std::cout << "test cleImage {Create, Push, Pull} pass - diff = " << difference << std::endl;
        }
    }

    {
        using type = float;
        auto gpu = std::make_shared<cle::GPU>();

        std::array<size_t,3> dims = {10, 5, 1};
        std::vector<type> A (dims[0]*dims[1]*dims[2]); 
        std::fill (A.begin(),A.end(), 10.0f);

        cle::Buffer buff_A = gpu->CreateBuffer<type>(dims);
        cle::Buffer buff_B = gpu->PushBuffer<type>(A, dims);
        std::vector<type> C = gpu->Pull<type>(buff_B);
        
        float diff = 0;
        for (int i =0; i< A.size(); i++)
        {
            diff += A[i] - C[i];
        }
        if (diff > 0)
        {
            return EXIT_FAILURE;
        }
        }

    {
        using type = float;
        auto gpu = std::make_shared<cle::GPU>();

        std::array<size_t,3> dims = {10, 5, 2};
        std::vector<type> A (dims[0]*dims[1]*dims[2]); 
        std::fill (A.begin(),A.end(), 10.0f);

        cle::Buffer buff_A = gpu->CreateBuffer<type>(dims);
        cle::Buffer buff_B = gpu->PushBuffer<type>(A, dims);
        std::vector<type> C = gpu->Pull<type>(buff_B);

        float difference = 0;
        for( auto it1 = A.begin(), it2 = C.begin(); 
            it1 != A.end() && it2 != C.end(); ++it1, ++it2)
        {
            difference += std::abs(*it1 - *it2);
        }
        if (difference > std::numeric_limits<type>::epsilon())
        {
            std::cout << "test cleImage {Create, Push, Pull} fail - diff = " << difference << std::endl;
            return EXIT_FAILURE;
        }
        else
        {
            std::cout << "test cleImage {Create, Push, Pull} pass - diff = " << difference << std::endl;
        }
    }

    {
        using type = double;
        auto gpu = std::make_shared<cle::GPU>();

        std::array<size_t,3> dims = {10, 5, 2};
        std::vector<type> A (dims[0]*dims[1]*dims[2]); 
        std::fill (A.begin(),A.end(), 10.0f);

        cle::Buffer buff_A = gpu->CreateBuffer<type>(dims);
        cle::Buffer buff_B = gpu->PushBuffer<type>(A, dims);
        std::vector<type> C = gpu->Pull<type>(buff_B);
        
        float difference = 0;
        for( auto it1 = A.begin(), it2 = C.begin(); 
            it1 != A.end() && it2 != C.end(); ++it1, ++it2)
        {
            difference += std::abs(*it1 - *it2);
        }
        if (difference > std::numeric_limits<type>::epsilon())
        {
            std::cout << "test cleImage {Create, Push, Pull} fail - diff = " << difference << std::endl;
            return EXIT_FAILURE;
        }
        else
        {
            std::cout << "test cleImage {Create, Push, Pull} pass - diff = " << difference << std::endl;
        }
    }

    {
        using type = int;
        auto gpu = std::make_shared<cle::GPU>();

        std::array<size_t,3> dims = {10, 5, 2};
        std::vector<type> A (dims[0]*dims[1]*dims[2]); 
        std::fill (A.begin(),A.end(), 10.0f);

        cle::Buffer buff_A = gpu->CreateBuffer<type>(dims);
        cle::Buffer buff_B = gpu->PushBuffer<type>(A, dims);
        std::vector<type> C = gpu->Pull<type>(buff_B);
        
        float difference = 0;
        for( auto it1 = A.begin(), it2 = C.begin(); 
            it1 != A.end() && it2 != C.end(); ++it1, ++it2)
        {
            difference += std::abs(*it1 - *it2);
        }
        if (difference > std::numeric_limits<type>::epsilon())
        {
            std::cout << "test cleImage {Create, Push, Pull} fail - diff = " << difference << std::endl;
            return EXIT_FAILURE;
        }
        else
        {
            std::cout << "test cleImage {Create, Push, Pull} pass - diff = " << difference << std::endl;
        }
    }

    {
        using type = unsigned int;
        auto gpu = std::make_shared<cle::GPU>();

        std::array<size_t,3> dims = {10, 5, 2};
        std::vector<type> A (dims[0]*dims[1]*dims[2]); 
        std::fill (A.begin(),A.end(), 10.0f);

        cle::Buffer buff_A = gpu->CreateBuffer<type>(dims);
        cle::Buffer buff_B = gpu->PushBuffer<type>(A, dims);
        std::vector<type> C = gpu->Pull<type>(buff_B);
        
        float difference = 0;
        for( auto it1 = A.begin(), it2 = C.begin(); 
            it1 != A.end() && it2 != C.end(); ++it1, ++it2)
        {
            difference += (*it1 - *it2);
        }
        if (difference > std::numeric_limits<type>::epsilon())
        {
            std::cout << "test cleImage {Create, Push, Pull} fail - diff = " << difference << std::endl;
            return EXIT_FAILURE;
        }
        else
        {
            std::cout << "test cleImage {Create, Push, Pull} pass - diff = " << difference << std::endl;
        }
    }

    {
        using type = char;
        auto gpu = std::make_shared<cle::GPU>();

        std::array<size_t,3> dims = {10, 5, 2};
        std::vector<type> A (dims[0]*dims[1]*dims[2]); 
        std::fill (A.begin(),A.end(), 10.0f);

        cle::Buffer buff_A = gpu->CreateBuffer<type>(dims);
        cle::Buffer buff_B = gpu->PushBuffer<type>(A, dims);
        std::vector<type> C = gpu->Pull<type>(buff_B);
        
        float difference = 0;
        for( auto it1 = A.begin(), it2 = C.begin(); 
            it1 != A.end() && it2 != C.end(); ++it1, ++it2)
        {
            difference += std::abs(*it1 - *it2);
        }
        if (difference > std::numeric_limits<type>::epsilon())
        {
            std::cout << "test cleImage {Create, Push, Pull} fail - diff = " << difference << std::endl;
            return EXIT_FAILURE;
        }
        else
        {
            std::cout << "test cleImage {Create, Push, Pull} pass - diff = " << difference << std::endl;
        }
    }

    {
        using type = unsigned char;
        auto gpu = std::make_shared<cle::GPU>();

        std::array<size_t,3> dims = {10, 5, 2};
        std::vector<type> A (dims[0]*dims[1]*dims[2]); 
        std::fill (A.begin(),A.end(), 10.0f);

        cle::Buffer buff_A = gpu->CreateBuffer<type>(dims);
        cle::Buffer buff_B = gpu->PushBuffer<type>(A, dims);
        std::vector<type> C = gpu->Pull<type>(buff_B);
        
        float difference = 0;
        for( auto it1 = A.begin(), it2 = C.begin(); 
            it1 != A.end() && it2 != C.end(); ++it1, ++it2)
        {
            difference += (*it1 - *it2);
        }
        if (difference > std::numeric_limits<type>::epsilon())
        {
            std::cout << "test cleImage {Create, Push, Pull} fail - diff = " << difference << std::endl;
            return EXIT_FAILURE;
        }
        else
        {
            std::cout << "test cleImage {Create, Push, Pull} pass - diff = " << difference << std::endl;
        }
    }

    {
        using type = short;
        auto gpu = std::make_shared<cle::GPU>();

        std::array<size_t,3> dims = {10, 5, 2};
        std::vector<type> A (dims[0]*dims[1]*dims[2]); 
        std::fill (A.begin(),A.end(), 10.0f);

        cle::Buffer buff_A = gpu->CreateBuffer<type>(dims);
        cle::Buffer buff_B = gpu->PushBuffer<type>(A, dims);
        std::vector<type> C = gpu->Pull<type>(buff_B);
        
        float difference = 0;
        for( auto it1 = A.begin(), it2 = C.begin(); 
            it1 != A.end() && it2 != C.end(); ++it1, ++it2)
        {
            difference += std::abs(*it1 - *it2);
        }
        if (difference > std::numeric_limits<type>::epsilon())
        {
            std::cout << "test cleImage {Create, Push, Pull} fail - diff = " << difference << std::endl;
            return EXIT_FAILURE;
        }
        else
        {
            std::cout << "test cleImage {Create, Push, Pull} pass - diff = " << difference << std::endl;
        }
    }

    {
        using type = unsigned short;
        auto gpu = std::make_shared<cle::GPU>();

        std::array<size_t,3> dims = {10, 5, 2};
        std::vector<type> A (dims[0]*dims[1]*dims[2]); 
        std::fill (A.begin(),A.end(), 10.0f);

        cle::Buffer buff_A = gpu->CreateBuffer<type>(dims);
        cle::Buffer buff_B = gpu->PushBuffer<type>(A, dims);
        std::vector<type> C = gpu->Pull<type>(buff_B);
        
        float difference = 0;
        for( auto it1 = A.begin(), it2 = C.begin(); 
            it1 != A.end() && it2 != C.end(); ++it1, ++it2)
        {
            difference += (*it1 - *it2);
        }
        if (difference > std::numeric_limits<type>::epsilon())
        {
            std::cout << "test cleImage {Create, Push, Pull} fail - diff = " << difference << std::endl;
            return EXIT_FAILURE;
        }
        else
        {
            std::cout << "test cleImage {Create, Push, Pull} pass - diff = " << difference << std::endl;
        }
    }

    {
        using type = float;
        auto gpu = std::make_shared<cle::GPU>();

        std::array<size_t,3> dims = {10, 5, 2};
        std::vector<type> A (dims[0]*dims[1]*dims[2]); 
        std::fill (A.begin(),A.end(), 10.0f);

        cle::Buffer buff_A = gpu->CreateBuffer<type>(dims);
        cle::Buffer buff_B = gpu->PushBuffer<type>(A, dims);
        buff_A = buff_B;
        std::vector<type> C = gpu->Pull<type>(buff_A);
        
        float diff = 0;
        for (int i =0; i< A.size(); i++)
        {
            diff += A[i] - C[i];
        }
        if (diff > 0)
        {
            return EXIT_FAILURE;
        }
    }

    {
        using type = double;
        auto gpu = std::make_shared<cle::GPU>();

        std::array<size_t,3> dims = {10, 5, 2};
        std::vector<type> A (dims[0]*dims[1]*dims[2]); 
        std::fill (A.begin(),A.end(), 10.0f);

        cle::Buffer buff_A = gpu->CreateBuffer<type>(dims);
        cle::Buffer buff_B = gpu->PushBuffer<type>(A, dims);
        buff_A = buff_B;
        std::vector<type> C = gpu->Pull<type>(buff_A);
        
        float diff = 0;
        for (int i =0; i< A.size(); i++)
        {
            diff += A[i] - C[i];
        }
        if (diff > 0)
        {
            return EXIT_FAILURE;
        }
    }
    

    return EXIT_SUCCESS;
}