
#include "cleGPU.hpp"
#include "cleKernelList.hpp"

template<class type>
bool IsDifferent(std::vector<type>& arr_1, std::vector<type>& arr_2)
{
    float difference = 0;
    for (auto it1 = arr_1.begin(), it2 = arr_2.begin(); 
              (it1 != arr_1.end()) && (it2 != arr_2.end()); ++it1, ++it2)
    {
        difference += std::abs(static_cast<float>(*it1) - static_cast<float>(*it2));
    }
    if (difference > std::numeric_limits<float>::epsilon() )
    {
        std::cerr << "[FAILED] : difference = " << difference << std::endl;
        return false;
    }
    else
    {
        return true;
    }
}

template<class T>
bool run_kernel_with_buffer(std::array<size_t, 3> dims)
{
    auto gpu = std::make_shared<cle::GPU>();
    gpu->SetWaitForKernelToFinish(true);

    std::vector<T> data (dims[0]*dims[1]*dims[2]);
    std::vector<T> valid (dims[0]*dims[1]*dims[2]);
    std::fill (data.begin(),data.end(), static_cast<T>(10.0f));
    std::fill (valid.begin(),valid.end(), static_cast<T>(100.0f));

    auto buff_A = gpu->Push<T>(data, dims);
    auto buff_B = gpu->Create<T>(dims);

    cle::AddImageAndScalarKernel kernel(gpu);
    kernel.SetInput(buff_A);
    kernel.SetOutput(buff_B);
    kernel.SetScalar(90.0f);
    kernel.Execute();

    auto res = gpu->Pull<T>(buff_B);
    
    return IsDifferent<T>(res, valid);
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

    auto buff_A = gpu->Push<T>(data, dims, "image");
    auto buff_B = gpu->Create<T>(dims, "image");

    cle::AddImageAndScalarKernel kernel(gpu);
    kernel.SetInput(buff_A);
    kernel.SetOutput(buff_B);
    kernel.SetScalar(90.0f);
    kernel.Execute();

    auto res = gpu->Pull<T>(buff_B);
    
    return IsDifferent<T>(res, valid);
}



int main(int argc, char **argv)
{
    bool flag = true;

{ 
    std::array<size_t, 3> dims = {10,1,1};
    if( ! run_kernel_with_buffer<float>(dims) ) { std::cerr << "test buffer float ("<<dims[0]<<","<<dims[1]<<","<<dims[2]<<") ... FAILED! \n"; flag = false; }
    if( ! run_kernel_with_image<float>(dims) ) {  std::cerr << "test image float ("<<dims[0]<<","<<dims[1]<<","<<dims[2]<<") ... FAILED! \n"; flag = false; }
    if( ! run_kernel_with_buffer<int>(dims) ) { std::cerr << "test buffer int ("<<dims[0]<<","<<dims[1]<<","<<dims[2]<<") ... FAILED! \n"; flag = false; }
    // if( ! run_kernel_with_image<int>(dims) ) {  std::cerr << "test image int ("<<dims[0]<<","<<dims[1]<<","<<dims[2]<<") ... FAILED! \n"; flag = false; }
    if( ! run_kernel_with_buffer<unsigned int>(dims) ) { std::cerr << "test buffer uint ("<<dims[0]<<","<<dims[1]<<","<<dims[2]<<") ... FAILED! \n"; flag = false; }
    // if( ! run_kernel_with_image<unsigned int>(dims) ) {  std::cerr << "test image uint ("<<dims[0]<<","<<dims[1]<<","<<dims[2]<<") ... FAILED! \n"; flag = false; }
    if( ! run_kernel_with_buffer<char>(dims) ) { std::cerr << "test buffer char ("<<dims[0]<<","<<dims[1]<<","<<dims[2]<<") ... FAILED! \n"; flag = false; }
    if( ! run_kernel_with_buffer<unsigned char>(dims) ) { std::cerr << "test buffer uchar ("<<dims[0]<<","<<dims[1]<<","<<dims[2]<<") ... FAILED! \n"; flag = false; }
    if( ! run_kernel_with_buffer<short>(dims) ) { std::cerr << "test buffer short ("<<dims[0]<<","<<dims[1]<<","<<dims[2]<<") ... FAILED! \n"; flag = false; }
    if( ! run_kernel_with_buffer<unsigned short>(dims) ) { std::cerr << "test buffer ushort ("<<dims[0]<<","<<dims[1]<<","<<dims[2]<<") ... FAILED! \n"; flag = false; }
}
{
    std::array<size_t, 3> dims = {10,5,1};
    if( ! run_kernel_with_buffer<float>(dims) ) { std::cerr << "test buffer float ("<<dims[0]<<","<<dims[1]<<","<<dims[2]<<") ... FAILED! \n"; flag = false; }
    if( ! run_kernel_with_image<float>(dims) ) {  std::cerr << "test image float ("<<dims[0]<<","<<dims[1]<<","<<dims[2]<<") ... FAILED! \n"; flag = false; }
    if( ! run_kernel_with_buffer<int>(dims) ) { std::cerr << "test buffer int ("<<dims[0]<<","<<dims[1]<<","<<dims[2]<<") ... FAILED! \n"; flag = false; }
    // if( ! run_kernel_with_image<int>(dims) ) {  std::cerr << "test image int ("<<dims[0]<<","<<dims[1]<<","<<dims[2]<<") ... FAILED! \n"; flag = false; }
    if( ! run_kernel_with_buffer<unsigned int>(dims) ) { std::cerr << "test buffer uint ("<<dims[0]<<","<<dims[1]<<","<<dims[2]<<") ... FAILED! \n"; flag = false; }
    // if( ! run_kernel_with_image<unsigned int>(dims) ) {  std::cerr << "test image uint ("<<dims[0]<<","<<dims[1]<<","<<dims[2]<<") ... FAILED! \n"; flag = false; }
    if( ! run_kernel_with_buffer<char>(dims) ) { std::cerr << "test buffer char ("<<dims[0]<<","<<dims[1]<<","<<dims[2]<<") ... FAILED! \n"; flag = false; }
    if( ! run_kernel_with_buffer<unsigned char>(dims) ) { std::cerr << "test buffer uchar ("<<dims[0]<<","<<dims[1]<<","<<dims[2]<<") ... FAILED! \n"; flag = false; }
    if( ! run_kernel_with_buffer<short>(dims) ) { std::cerr << "test buffer short ("<<dims[0]<<","<<dims[1]<<","<<dims[2]<<") ... FAILED! \n"; flag = false; }
    if( ! run_kernel_with_buffer<unsigned short>(dims) ) { std::cerr << "test buffer ushort ("<<dims[0]<<","<<dims[1]<<","<<dims[2]<<") ... FAILED! \n"; flag = false; }
}
{
    std::array<size_t, 3> dims = {10,5,2};
    if( ! run_kernel_with_buffer<float>(dims) ) { std::cerr << "test buffer float ("<<dims[0]<<","<<dims[1]<<","<<dims[2]<<") ... FAILED! \n"; flag = false; }
    if( ! run_kernel_with_image<float>(dims) ) {  std::cerr << "test image float ("<<dims[0]<<","<<dims[1]<<","<<dims[2]<<") ... FAILED! \n"; flag = false; }
    if( ! run_kernel_with_buffer<int>(dims) ) { std::cerr << "test buffer int ("<<dims[0]<<","<<dims[1]<<","<<dims[2]<<") ... FAILED! \n"; flag = false; }
    // if( ! run_kernel_with_image<int>(dims) ) {  std::cerr << "test image int ("<<dims[0]<<","<<dims[1]<<","<<dims[2]<<") ... FAILED! \n"; flag = false; }
    if( ! run_kernel_with_buffer<unsigned int>(dims) ) { std::cerr << "test buffer uint ("<<dims[0]<<","<<dims[1]<<","<<dims[2]<<") ... FAILED! \n"; flag = false; }
    // if( ! run_kernel_with_image<unsigned int>(dims) ) {  std::cerr << "test image uint ("<<dims[0]<<","<<dims[1]<<","<<dims[2]<<") ... FAILED! \n"; flag = false; }
    if( ! run_kernel_with_buffer<char>(dims) ) { std::cerr << "test buffer char ("<<dims[0]<<","<<dims[1]<<","<<dims[2]<<") ... FAILED! \n"; flag = false; }
    if( ! run_kernel_with_buffer<unsigned char>(dims) ) { std::cerr << "test buffer uchar ("<<dims[0]<<","<<dims[1]<<","<<dims[2]<<") ... FAILED! \n"; flag = false; }
    if( ! run_kernel_with_buffer<short>(dims) ) { std::cerr << "test buffer short ("<<dims[0]<<","<<dims[1]<<","<<dims[2]<<") ... FAILED! \n"; flag = false; }
    if( ! run_kernel_with_buffer<unsigned short>(dims) ) { std::cerr << "test buffer ushort ("<<dims[0]<<","<<dims[1]<<","<<dims[2]<<") ... FAILED! \n"; flag = false; }
}
{
    std::array<size_t, 3> dims = {1,5,2};
    if( ! run_kernel_with_buffer<float>(dims) ) { std::cerr << "test buffer float ("<<dims[0]<<","<<dims[1]<<","<<dims[2]<<") ... FAILED! \n"; flag = false; }
    if( ! run_kernel_with_image<float>(dims) ) {  std::cerr << "test image float ("<<dims[0]<<","<<dims[1]<<","<<dims[2]<<") ... FAILED! \n"; flag = false; }
    if( ! run_kernel_with_buffer<int>(dims) ) { std::cerr << "test buffer int ("<<dims[0]<<","<<dims[1]<<","<<dims[2]<<") ... FAILED! \n"; flag = false; }
    // if( ! run_kernel_with_image<int>(dims) ) {  std::cerr << "test image int ("<<dims[0]<<","<<dims[1]<<","<<dims[2]<<") ... FAILED! \n"; flag = false; }
    if( ! run_kernel_with_buffer<unsigned int>(dims) ) { std::cerr << "test buffer uint ("<<dims[0]<<","<<dims[1]<<","<<dims[2]<<") ... FAILED! \n"; flag = false; }
    // if( ! run_kernel_with_image<unsigned int>(dims) ) {  std::cerr << "test image uint ("<<dims[0]<<","<<dims[1]<<","<<dims[2]<<") ... FAILED! \n"; flag = false; }
    if( ! run_kernel_with_buffer<char>(dims) ) { std::cerr << "test buffer char ("<<dims[0]<<","<<dims[1]<<","<<dims[2]<<") ... FAILED! \n"; flag = false; }
    if( ! run_kernel_with_buffer<unsigned char>(dims) ) { std::cerr << "test buffer uchar ("<<dims[0]<<","<<dims[1]<<","<<dims[2]<<") ... FAILED! \n"; flag = false; }
    if( ! run_kernel_with_buffer<short>(dims) ) { std::cerr << "test buffer short ("<<dims[0]<<","<<dims[1]<<","<<dims[2]<<") ... FAILED! \n"; flag = false; }
    if( ! run_kernel_with_buffer<unsigned short>(dims) ) { std::cerr << "test buffer ushort ("<<dims[0]<<","<<dims[1]<<","<<dims[2]<<") ... FAILED! \n"; flag = false; }
}
{
    std::array<size_t, 3> dims = {1,1,2};
    if( ! run_kernel_with_buffer<float>(dims) ) { std::cerr << "test buffer float ("<<dims[0]<<","<<dims[1]<<","<<dims[2]<<") ... FAILED! \n"; flag = false; }
    if( ! run_kernel_with_image<float>(dims) ) {  std::cerr << "test image float ("<<dims[0]<<","<<dims[1]<<","<<dims[2]<<") ... FAILED! \n"; flag = false; }
    if( ! run_kernel_with_buffer<int>(dims) ) { std::cerr << "test buffer int ("<<dims[0]<<","<<dims[1]<<","<<dims[2]<<") ... FAILED! \n"; flag = false; }
    // if( ! run_kernel_with_image<int>(dims) ) {  std::cerr << "test image int ("<<dims[0]<<","<<dims[1]<<","<<dims[2]<<") ... FAILED! \n"; flag = false; }
    if( ! run_kernel_with_buffer<unsigned int>(dims) ) { std::cerr << "test buffer uint ("<<dims[0]<<","<<dims[1]<<","<<dims[2]<<") ... FAILED! \n"; flag = false; }
    // if( ! run_kernel_with_image<unsigned int>(dims) ) {  std::cerr << "test image uint ("<<dims[0]<<","<<dims[1]<<","<<dims[2]<<") ... FAILED! \n"; flag = false; }
    if( ! run_kernel_with_buffer<char>(dims) ) { std::cerr << "test buffer char ("<<dims[0]<<","<<dims[1]<<","<<dims[2]<<") ... FAILED! \n"; flag = false; }
    if( ! run_kernel_with_buffer<unsigned char>(dims) ) { std::cerr << "test buffer uchar ("<<dims[0]<<","<<dims[1]<<","<<dims[2]<<") ... FAILED! \n"; flag = false; }
    if( ! run_kernel_with_buffer<short>(dims) ) { std::cerr << "test buffer short ("<<dims[0]<<","<<dims[1]<<","<<dims[2]<<") ... FAILED! \n"; flag = false; }
    if( ! run_kernel_with_buffer<unsigned short>(dims) ) { std::cerr << "test buffer ushort ("<<dims[0]<<","<<dims[1]<<","<<dims[2]<<") ... FAILED! \n"; flag = false; }
}

    if (flag)
        return EXIT_SUCCESS;
    else
        return EXIT_FAILURE;
}