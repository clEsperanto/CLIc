
#include <random>
#include <iostream>
#include <vector>

#include "cleGPU.hpp"
#include "utils.hpp"


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

        auto buff_A = gpu->Create<type>(dims);
        auto buff_B = gpu->Push<type>(A, dims);
        auto C = gpu->Pull<type>(buff_B);
        
        if(IsDifferent(C, A))
        {
            return EXIT_FAILURE;
        }
    }

    {
        using type = float;
        auto gpu = std::make_shared<cle::GPU>();

        std::array<size_t,3> dims = {10, 5, 1};
        std::vector<type> A (dims[0]*dims[1]*dims[2]); 
        std::fill (A.begin(),A.end(), 10.0f);

        auto buff_A = gpu->Create<type>(dims);
        auto buff_B = gpu->Push<type>(A, dims);
        auto C = gpu->Pull<type>(buff_B);
        
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

        auto buff_A = gpu->Create<type>(dims);
        auto buff_B = gpu->Push<type>(A, dims);
        auto C = gpu->Pull<type>(buff_B);

        if(IsDifferent(C, A))
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

        auto buff_A = gpu->Create<type>(dims);
        auto buff_B = gpu->Push<type>(A, dims);
        auto C = gpu->Pull<type>(buff_B);
        
        if(IsDifferent(C, A))
        {
            return EXIT_FAILURE;
        }
    }

    {
        using type = int;
        auto gpu = std::make_shared<cle::GPU>();

        std::array<size_t,3> dims = {10, 5, 2};
        std::vector<type> A (dims[0]*dims[1]*dims[2]); 
        std::fill (A.begin(),A.end(), 10.0f);

        auto buff_A = gpu->Create<type>(dims);
        auto buff_B = gpu->Push<type>(A, dims);
        auto C = gpu->Pull<type>(buff_B);
        
        if(IsDifferent(C, A))
        {
            return EXIT_FAILURE;
        }
    }

    {
        using type = unsigned int;
        auto gpu = std::make_shared<cle::GPU>();

        std::array<size_t,3> dims = {10, 5, 2};
        std::vector<type> A (dims[0]*dims[1]*dims[2]); 
        std::fill (A.begin(),A.end(), 10.0f);

        auto buff_A = gpu->Create<type>(dims);
        auto buff_B = gpu->Push<type>(A, dims);
        auto C = gpu->Pull<type>(buff_B);
        
        if(IsDifferent(C, A))
        {
            return EXIT_FAILURE;
        }
    }

    {
        using type = char;
        auto gpu = std::make_shared<cle::GPU>();

        std::array<size_t,3> dims = {10, 5, 2};
        std::vector<type> A (dims[0]*dims[1]*dims[2]); 
        std::fill (A.begin(),A.end(), 10.0f);

        auto buff_A = gpu->Create<type>(dims);
        auto buff_B = gpu->Push<type>(A, dims);
        auto C = gpu->Pull<type>(buff_B);
        
        if(IsDifferent(C, A))
        {
            return EXIT_FAILURE;
        }
    }

    {
        using type = unsigned char;
        auto gpu = std::make_shared<cle::GPU>();

        std::array<size_t,3> dims = {10, 5, 2};
        std::vector<type> A (dims[0]*dims[1]*dims[2]); 
        std::fill (A.begin(),A.end(), 10.0f);

        auto buff_A = gpu->Create<type>(dims);
        auto buff_B = gpu->Push<type>(A, dims);
        auto C = gpu->Pull<type>(buff_B);
        
        if(IsDifferent(C, A))
        {
            return EXIT_FAILURE;
        }
    }

    {
        using type = short;
        auto gpu = std::make_shared<cle::GPU>();

        std::array<size_t,3> dims = {10, 5, 2};
        std::vector<type> A (dims[0]*dims[1]*dims[2]); 
        std::fill (A.begin(),A.end(), 10.0f);

        auto buff_A = gpu->Create<type>(dims);
        auto buff_B = gpu->Push<type>(A, dims);
        auto C = gpu->Pull<type>(buff_B);
        
        if(IsDifferent(C, A))
        {
            return EXIT_FAILURE;
        }
    }

    {
        using type = unsigned short;
        auto gpu = std::make_shared<cle::GPU>();

        std::array<size_t,3> dims = {10, 5, 2};
        std::vector<type> A (dims[0]*dims[1]*dims[2]); 
        std::fill (A.begin(),A.end(), 10.0f);

        auto buff_A = gpu->Create<type>(dims);
        auto buff_B = gpu->Push<type>(A, dims);
        auto C = gpu->Pull<type>(buff_B);
        
        if(IsDifferent(C, A))
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

        auto buff_A = gpu->Create<type>(dims);
        auto buff_B = gpu->Push<type>(A, dims);
        buff_A = buff_B;
        auto C = gpu->Pull<type>(buff_A);
        
        if(IsDifferent(C, A))
        {
            return EXIT_FAILURE;
        }
    }    

    return EXIT_SUCCESS;
}