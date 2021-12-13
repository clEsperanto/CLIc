
#include "cleGPU.hpp"
#include "cleBuffer.hpp"
#include "cleImage.hpp"
#include "cleKernelList.hpp"


template<class T>
bool run_kernel_with_buffer(std::array<size_t, 3> dims)
{
    auto gpu = std::make_shared<cle::GPU>();
    gpu->SetWaitForKernelToFinish(true);

    std::vector<T> data (dims[0]*dims[1]*dims[2]);
    std::vector<T> valid (dims[0]*dims[1]*dims[2]);
    std::fill (data.begin(),data.end(), static_cast<T>(10.0f));
    std::fill (valid.begin(),valid.end(), static_cast<T>(100.0f));

    auto buff_A = gpu->PushBuffer<T>(data, dims);
    auto buff_B = gpu->CreateBuffer<T>(dims);

    cle::AddImageAndScalarKernel kernel(gpu);
    kernel.SetInput(buff_A);
    kernel.SetOutput(buff_B);
    kernel.SetScalar(90.0f);
    kernel.Execute();

    std::vector<T> res = gpu->Pull<T>(buff_B);
    
    float difference = 0;
    for( auto it1 = valid.begin(), it2 = res.begin(); 
         it1 != valid.end() && it2 != res.end(); ++it1, ++it2)
    {
        difference += std::abs(static_cast<float>(*it1) - static_cast<float>(*it2));
    }
    if (difference > std::numeric_limits<T>::epsilon())
    {
        return false;
    }
    else
    {
        return true;
    }
}

template<class T>
bool run_kernel_with_image(std::array<size_t, 3> dims)
{
    auto gpu = std::make_shared<cle::GPU>();
    gpu->SetWaitForKernelToFinish(true);

    std::vector<T> data (dims[0]*dims[1]*dims[2]);
    std::vector<T> valid (dims[0]*dims[1]*dims[2]);
    std::fill (data.begin(),data.end(), static_cast<T>(10.0f));
    std::fill (valid.begin(),valid.end(), static_cast<T>(100.0f));

    auto buff_A = gpu->PushImage<T>(data, dims);
    auto buff_B = gpu->CreateImage<T>(dims);

    cle::AddImageAndScalarKernel kernel(gpu);
    kernel.SetInput(buff_A);
    kernel.SetOutput(buff_B);
    kernel.SetScalar(90.0f);
    kernel.Execute();

    std::vector<T> res = gpu->Pull<T>(buff_B);
    
    float difference = 0;
    for( auto it1 = valid.begin(), it2 = res.begin(); 
         it1 != valid.end() && it2 != res.end(); ++it1, ++it2)
    {
        difference += std::abs(static_cast<float>(*it1) - static_cast<float>(*it2));
    }
    if (difference > std::numeric_limits<T>::epsilon())
    {
        return false;
    }
    else
    {
        return true;
    }
}



int main(int argc, char **argv)
{
    bool flag = true;

{ 
    std::array<size_t, 3> dims = {10,1,1};
    // float
    if( ! run_kernel_with_buffer<float>(dims) ) { std::cout << "test buffer float ("<<dims[0]<<","<<dims[1]<<","<<dims[2]<<") fail.\n"; flag = false; }
    if( ! run_kernel_with_image<float>(dims) ) {  std::cout << "test image float ("<<dims[0]<<","<<dims[1]<<","<<dims[2]<<") fail.\n"; flag = false; }
    // int
    if( ! run_kernel_with_buffer<int>(dims) ) { std::cout << "test buffer int ("<<dims[0]<<","<<dims[1]<<","<<dims[2]<<") fail.\n"; flag = false; }
    if( ! run_kernel_with_image<int>(dims) ) {  std::cout << "test image int ("<<dims[0]<<","<<dims[1]<<","<<dims[2]<<") fail.\n"; flag = false; }
    // uint
    if( ! run_kernel_with_buffer<unsigned int>(dims) ) { std::cout << "test buffer uint ("<<dims[0]<<","<<dims[1]<<","<<dims[2]<<") fail.\n"; flag = false; }
    if( ! run_kernel_with_image<unsigned int>(dims) ) {  std::cout << "test image uint ("<<dims[0]<<","<<dims[1]<<","<<dims[2]<<") fail.\n"; flag = false; }
    // char
    if( ! run_kernel_with_buffer<char>(dims) ) { std::cout << "test buffer char ("<<dims[0]<<","<<dims[1]<<","<<dims[2]<<") fail.\n"; flag = false; }
    // uchar
    if( ! run_kernel_with_buffer<unsigned char>(dims) ) { std::cout << "test buffer uchar ("<<dims[0]<<","<<dims[1]<<","<<dims[2]<<") fail.\n"; flag = false; }
    // short
    if( ! run_kernel_with_buffer<short>(dims) ) { std::cout << "test buffer short ("<<dims[0]<<","<<dims[1]<<","<<dims[2]<<") fail.\n"; flag = false; }
    // ushort
    if( ! run_kernel_with_buffer<unsigned short>(dims) ) { std::cout << "test buffer ushort ("<<dims[0]<<","<<dims[1]<<","<<dims[2]<<") fail.\n"; flag = false; }
}
{
    std::array<size_t, 3> dims = {10,5,1};
    // float
    if( ! run_kernel_with_buffer<float>(dims) ) { std::cout << "test buffer float ("<<dims[0]<<","<<dims[1]<<","<<dims[2]<<") fail.\n"; flag = false; }
    if( ! run_kernel_with_image<float>(dims) ) {  std::cout << "test image float ("<<dims[0]<<","<<dims[1]<<","<<dims[2]<<") fail.\n"; flag = false; }
    // int
    if( ! run_kernel_with_buffer<int>(dims) ) { std::cout << "test buffer int ("<<dims[0]<<","<<dims[1]<<","<<dims[2]<<") fail.\n"; flag = false; }
    if( ! run_kernel_with_image<int>(dims) ) {  std::cout << "test image int ("<<dims[0]<<","<<dims[1]<<","<<dims[2]<<") fail.\n"; flag = false; }
    // uint
    if( ! run_kernel_with_buffer<unsigned int>(dims) ) { std::cout << "test buffer uint ("<<dims[0]<<","<<dims[1]<<","<<dims[2]<<") fail.\n"; flag = false; }
    if( ! run_kernel_with_image<unsigned int>(dims) ) {  std::cout << "test image uint ("<<dims[0]<<","<<dims[1]<<","<<dims[2]<<") fail.\n"; flag = false; }
    // char
    if( ! run_kernel_with_buffer<char>(dims) ) { std::cout << "test buffer char ("<<dims[0]<<","<<dims[1]<<","<<dims[2]<<") fail.\n"; flag = false; }
    // uchar
    if( ! run_kernel_with_buffer<unsigned char>(dims) ) { std::cout << "test buffer uchar ("<<dims[0]<<","<<dims[1]<<","<<dims[2]<<") fail.\n"; flag = false; }
    // short
    if( ! run_kernel_with_buffer<short>(dims) ) { std::cout << "test buffer short ("<<dims[0]<<","<<dims[1]<<","<<dims[2]<<") fail.\n"; flag = false; }
    // ushort
    if( ! run_kernel_with_buffer<unsigned short>(dims) ) { std::cout << "test buffer ushort ("<<dims[0]<<","<<dims[1]<<","<<dims[2]<<") fail.\n"; flag = false; }
}
{
    std::array<size_t, 3> dims = {10,5,2};
    // float
    if( ! run_kernel_with_buffer<float>(dims) ) { std::cout << "test buffer float ("<<dims[0]<<","<<dims[1]<<","<<dims[2]<<") fail.\n"; flag = false; }
    if( ! run_kernel_with_image<float>(dims) ) {  std::cout << "test image float ("<<dims[0]<<","<<dims[1]<<","<<dims[2]<<") fail.\n"; flag = false; }
    // int
    if( ! run_kernel_with_buffer<int>(dims) ) { std::cout << "test buffer int ("<<dims[0]<<","<<dims[1]<<","<<dims[2]<<") fail.\n"; flag = false; }
    if( ! run_kernel_with_image<int>(dims) ) {  std::cout << "test image int ("<<dims[0]<<","<<dims[1]<<","<<dims[2]<<") fail.\n"; flag = false; }
    // uint
    if( ! run_kernel_with_buffer<unsigned int>(dims) ) { std::cout << "test buffer uint ("<<dims[0]<<","<<dims[1]<<","<<dims[2]<<") fail.\n"; flag = false; }
    if( ! run_kernel_with_image<unsigned int>(dims) ) {  std::cout << "test image uint ("<<dims[0]<<","<<dims[1]<<","<<dims[2]<<") fail.\n"; flag = false; }
    // char
    if( ! run_kernel_with_buffer<char>(dims) ) { std::cout << "test buffer char ("<<dims[0]<<","<<dims[1]<<","<<dims[2]<<") fail.\n"; flag = false; }
    // uchar
    if( ! run_kernel_with_buffer<unsigned char>(dims) ) { std::cout << "test buffer uchar ("<<dims[0]<<","<<dims[1]<<","<<dims[2]<<") fail.\n"; flag = false; }
    // short
    if( ! run_kernel_with_buffer<short>(dims) ) { std::cout << "test buffer short ("<<dims[0]<<","<<dims[1]<<","<<dims[2]<<") fail.\n"; flag = false; }
    // ushort
    if( ! run_kernel_with_buffer<unsigned short>(dims) ) { std::cout << "test buffer ushort ("<<dims[0]<<","<<dims[1]<<","<<dims[2]<<") fail.\n"; flag = false; }
}
{
    std::array<size_t, 3> dims = {1,5,2};
    // float
    if( ! run_kernel_with_buffer<float>(dims) ) { std::cout << "test buffer float ("<<dims[0]<<","<<dims[1]<<","<<dims[2]<<") fail.\n"; flag = false; }
    if( ! run_kernel_with_image<float>(dims) ) {  std::cout << "test image float ("<<dims[0]<<","<<dims[1]<<","<<dims[2]<<") fail.\n"; flag = false; }
    // int
    if( ! run_kernel_with_buffer<int>(dims) ) { std::cout << "test buffer int ("<<dims[0]<<","<<dims[1]<<","<<dims[2]<<") fail.\n"; flag = false; }
    if( ! run_kernel_with_image<int>(dims) ) {  std::cout << "test image int ("<<dims[0]<<","<<dims[1]<<","<<dims[2]<<") fail.\n"; flag = false; }
    // uint
    if( ! run_kernel_with_buffer<unsigned int>(dims) ) { std::cout << "test buffer uint ("<<dims[0]<<","<<dims[1]<<","<<dims[2]<<") fail.\n"; flag = false; }
    if( ! run_kernel_with_image<unsigned int>(dims) ) {  std::cout << "test image uint ("<<dims[0]<<","<<dims[1]<<","<<dims[2]<<") fail.\n"; flag = false; }
    // char
    if( ! run_kernel_with_buffer<char>(dims) ) { std::cout << "test buffer char ("<<dims[0]<<","<<dims[1]<<","<<dims[2]<<") fail.\n"; flag = false; }
    // uchar
    if( ! run_kernel_with_buffer<unsigned char>(dims) ) { std::cout << "test buffer uchar ("<<dims[0]<<","<<dims[1]<<","<<dims[2]<<") fail.\n"; flag = false; }
    // short
    if( ! run_kernel_with_buffer<short>(dims) ) { std::cout << "test buffer short ("<<dims[0]<<","<<dims[1]<<","<<dims[2]<<") fail.\n"; flag = false; }
    // ushort
    if( ! run_kernel_with_buffer<unsigned short>(dims) ) { std::cout << "test buffer ushort ("<<dims[0]<<","<<dims[1]<<","<<dims[2]<<") fail.\n"; flag = false; }
}
{
    std::array<size_t, 3> dims = {1,1,2};
    // float
    if( ! run_kernel_with_buffer<float>(dims) ) { std::cout << "test buffer float ("<<dims[0]<<","<<dims[1]<<","<<dims[2]<<") fail.\n"; flag = false; }
    if( ! run_kernel_with_image<float>(dims) ) {  std::cout << "test image float ("<<dims[0]<<","<<dims[1]<<","<<dims[2]<<") fail.\n"; flag = false; }
    // int
    if( ! run_kernel_with_buffer<int>(dims) ) { std::cout << "test buffer int ("<<dims[0]<<","<<dims[1]<<","<<dims[2]<<") fail.\n"; flag = false; }
    if( ! run_kernel_with_image<int>(dims) ) {  std::cout << "test image int ("<<dims[0]<<","<<dims[1]<<","<<dims[2]<<") fail.\n"; flag = false; }
    // uint
    if( ! run_kernel_with_buffer<unsigned int>(dims) ) { std::cout << "test buffer uint ("<<dims[0]<<","<<dims[1]<<","<<dims[2]<<") fail.\n"; flag = false; }
    if( ! run_kernel_with_image<unsigned int>(dims) ) {  std::cout << "test image uint ("<<dims[0]<<","<<dims[1]<<","<<dims[2]<<") fail.\n"; flag = false; }
    // char
    if( ! run_kernel_with_buffer<char>(dims) ) { std::cout << "test buffer char ("<<dims[0]<<","<<dims[1]<<","<<dims[2]<<") fail.\n"; flag = false; }
    // uchar
    if( ! run_kernel_with_buffer<unsigned char>(dims) ) { std::cout << "test buffer uchar ("<<dims[0]<<","<<dims[1]<<","<<dims[2]<<") fail.\n"; flag = false; }
    // short
    if( ! run_kernel_with_buffer<short>(dims) ) { std::cout << "test buffer short ("<<dims[0]<<","<<dims[1]<<","<<dims[2]<<") fail.\n"; flag = false; }
    // ushort
    if( ! run_kernel_with_buffer<unsigned short>(dims) ) { std::cout << "test buffer ushort ("<<dims[0]<<","<<dims[1]<<","<<dims[2]<<") fail.\n"; flag = false; }
}
    return flag;
}