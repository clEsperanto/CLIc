#include "translator.hpp"

#include "cle_affine_transform.h"
#include "cle_binary_erosion.h"
#include "cle_clahe.h"
#include "cle_depth_projection.h"
#include "cle_draw_sphere.h"
#include "cle_generate_touching_area_matrix.h"
#include "cle_hessian_eigenvalues.h"
#include "cle_histogram.h"
#include "cle_maximum_distance_touching_neighbors.h"
#include "cle_mean_distance_n_nearest_neighbors.h"
#include "cle_sobel.h"

#include <gtest/gtest.h>
#include <string>
#include <utility>
#include <vector>

// The translators are pure text transformations, compiled in every build: these tests need no GPU and run
// whatever the backend. They check the translated text, not that it compiles (the CUDA / Metal test suites do).

namespace
{
auto
contains(const std::string & code, const std::string & token) -> ::testing::AssertionResult
{
  if (code.find(token) != std::string::npos)
  {
    return ::testing::AssertionSuccess();
  }
  return ::testing::AssertionFailure() << "'" << token << "' not found in:\n" << code;
}

auto
lacks(const std::string & code, const std::string & token) -> ::testing::AssertionResult
{
  if (code.find(token) == std::string::npos)
  {
    return ::testing::AssertionSuccess();
  }
  return ::testing::AssertionFailure() << "'" << token << "' unexpectedly found in:\n" << code;
}

auto
count(const std::string & code, const std::string & token) -> size_t
{
  size_t n = 0;
  for (size_t pos = code.find(token); pos != std::string::npos; pos = code.find(token, pos + token.size()))
  {
    ++n;
  }
  return n;
}

auto
toCUDA(const std::string & code) -> std::string
{
  return OpenCLToCUDATranslator().translate(code);
}

auto
toMetal(const std::string & code) -> std::string
{
  return OpenCLToMetalTranslator().translate(code);
}

const std::vector<std::pair<std::string, std::string>> real_kernels = {
  { "affine_transform", kernel::affine_transform },
  { "binary_erosion", kernel::binary_erosion },
  { "clahe", kernel::clahe },
  { "depth_projection", kernel::depth_projection },
  { "draw_sphere", kernel::draw_sphere },
  { "generate_touching_area_matrix", kernel::generate_touching_area_matrix },
  { "hessian_eigenvalues", kernel::hessian_eigenvalues },
  { "histogram", kernel::histogram },
  { "maximum_distance_touching_neighbors", kernel::maximum_distance_touching_neighbors },
  { "mean_distance_n_nearest_neighbors", kernel::mean_distance_n_nearest_neighbors },
  { "sobel", kernel::sobel },
};
} // namespace

// ═══════════════════════════════════════════════════════════════════════════
//  OpenCL → CUDA
// ═══════════════════════════════════════════════════════════════════════════

TEST(TestCUDATranslator, kernelSignatureAndGlobalSize)
{
  const std::string code = toCUDA("__kernel void k(__global float* dst, const int n)\n"
                                  "{\n"
                                  "  const int x = get_global_id(0);\n"
                                  "  dst[x] = get_global_size(0) + get_global_size(2);\n"
                                  "}\n");
  EXPECT_TRUE(contains(code, "extern \"C\" __global__ void k("));
  EXPECT_TRUE(
    contains(code, "float* dst, const int n, const int cle_global_size_0, const int cle_global_size_1, const int cle_global_size_2)"));
  EXPECT_TRUE(lacks(code, "__global float"));
  // requested global size, and out-of-range threads return
  EXPECT_TRUE(contains(code, "dst[x] = cle_global_size_0 + cle_global_size_2;"));
  EXPECT_TRUE(contains(code, "if (((int)(blockDim.x * blockIdx.x + threadIdx.x)) >= cle_global_size_0"));
  EXPECT_TRUE(contains(code, ") return;"));
}

TEST(TestCUDATranslator, kernelWithoutParameters)
{
  for (const std::string params : { "", "void" })
  {
    const std::string code = toCUDA("__kernel void k(" + params + ")\n{\n}\n");
    EXPECT_TRUE(contains(code, "void k(const int cle_global_size_0, const int cle_global_size_1, const int cle_global_size_2)"));
  }
}

TEST(TestCUDATranslator, prototypeIsLeftUnchanged)
{
  const std::string code = toCUDA("__kernel void k(int a);\n"
                                  "__kernel void k(int a)\n{\n}\n");
  EXPECT_EQ(count(code, "const int cle_global_size_0"), 1);
  EXPECT_TRUE(contains(code, "void k(int a);"));
}

TEST(TestCUDATranslator, barrierKernelHasNoRangeGuard)
{
  const std::string code = toCUDA("__kernel void k(__global float* dst)\n"
                                  "{\n"
                                  "  __local float tile[16];\n"
                                  "  tile[get_local_id(0)] = get_group_id(1) + get_local_size(0) + get_num_groups(2);\n"
                                  "  barrier(CLK_LOCAL_MEM_FENCE);\n"
                                  "  dst[get_global_id(1)] = tile[0];\n"
                                  "}\n");
  EXPECT_TRUE(lacks(code, ") return;"));
  EXPECT_TRUE(contains(code, "__shared__ float tile[16];"));
  EXPECT_TRUE(contains(code, "tile[((int)threadIdx.x)] = ((int)blockIdx.y) + ((int)blockDim.x) + ((int)gridDim.z);"));
  EXPECT_TRUE(contains(code, "__syncthreads();"));
  EXPECT_TRUE(contains(code, "dst[((int)(blockDim.y * blockIdx.y + threadIdx.y))]"));
}

TEST(TestCUDATranslator, workItemFunctionsOutsideKernel)
{
  // outside a kernel body, get_global_size is the (rounded) CUDA grid size
  const std::string code = toCUDA("int f() { return get_global_size(1) + get_global_id(2) + get_local_id(1) + get_local_id(2) +"
                                  " get_group_id(0) + get_group_id(2) + get_local_size(1) + get_local_size(2) + get_num_groups(0) +"
                                  " get_num_groups(1) + get_global_size(0) + get_global_size(2); }");
  EXPECT_TRUE(contains(code, "((int)(gridDim.y * blockDim.y))"));
  EXPECT_TRUE(contains(code, "((int)(blockDim.z * blockIdx.z + threadIdx.z))"));
  EXPECT_TRUE(contains(code, "((int)threadIdx.y)"));
  EXPECT_TRUE(contains(code, "((int)threadIdx.z)"));
  EXPECT_TRUE(contains(code, "((int)blockIdx.x)"));
  EXPECT_TRUE(contains(code, "((int)blockIdx.z)"));
  EXPECT_TRUE(contains(code, "((int)blockDim.y)"));
  EXPECT_TRUE(contains(code, "((int)blockDim.z)"));
  EXPECT_TRUE(contains(code, "((int)gridDim.x)"));
  EXPECT_TRUE(contains(code, "((int)gridDim.y)"));
  EXPECT_TRUE(contains(code, "((int)(gridDim.x * blockDim.x))"));
  EXPECT_TRUE(contains(code, "((int)(gridDim.z * blockDim.z))"));
}

TEST(TestCUDATranslator, synchronization)
{
  const std::string code = toCUDA("barrier(CLK_GLOBAL_MEM_FENCE);\n"
                                  "barrier(CLK_LOCAL_MEM_FENCE | CLK_GLOBAL_MEM_FENCE);\n"
                                  "barrier(CLK_GLOBAL_MEM_FENCE | CLK_LOCAL_MEM_FENCE);\n"
                                  "mem_fence(CLK_GLOBAL_MEM_FENCE);\n"
                                  "mem_fence(CLK_LOCAL_MEM_FENCE);\n"
                                  "read_mem_fence(CLK_GLOBAL_MEM_FENCE);\n"
                                  "write_mem_fence(CLK_GLOBAL_MEM_FENCE);\n");
  EXPECT_EQ(count(code, "__syncthreads();"), 3);
  EXPECT_EQ(count(code, "__threadfence();"), 3);
  EXPECT_EQ(count(code, "__threadfence_block();"), 1);
  EXPECT_TRUE(lacks(code, "read_"));
  EXPECT_TRUE(lacks(code, "write_"));
  EXPECT_TRUE(lacks(code, "mem_fence"));
  EXPECT_TRUE(lacks(code, "CLK_"));
}

TEST(TestCUDATranslator, qualifiersAndAddressSpaces)
{
  const std::string code = toCUDA("inline float f(float x) { return x; }\n"
                                  "__device__ inline float g(float x) { return x; }\n"
                                  "__constant float c[2] = { 1, 2 };\n"
                                  "kernel void k(__global float* dst) __attribute__((reqd_work_group_size(16, 1, 1)))\n"
                                  "{\n"
                                  "  __private int a = 0;\n"
                                  "  __private\n"
                                  "  int b = 0;\n"
                                  "}\n");
  EXPECT_TRUE(contains(code, "__device__ inline float f("));
  EXPECT_TRUE(contains(code, "__device__ inline float g("));
  EXPECT_TRUE(lacks(code, "__device__ __device__"));
  EXPECT_TRUE(contains(code, "__constant__ float c[2]"));
  EXPECT_TRUE(contains(code, "extern \"C\" __global__ void k("));
  EXPECT_TRUE(contains(code, "// __attribute__((reqd_work_group_size("));
  EXPECT_TRUE(lacks(code, "__private"));
  EXPECT_TRUE(contains(code, "int a = 0;"));
}

TEST(TestCUDATranslator, pragmasAndSamplers)
{
  const std::string code =
    toCUDA("#pragma OPENCL EXTENSION cl_khr_fp64 : enable\n"
           "  #pragma unroll\n"
           "#define CL_VERSION_1_2 120\n"
           "__constant sampler_t sampler = CLK_NORMALIZED_COORDS_FALSE | CLK_ADDRESS_CLAMP_TO_EDGE | CLK_FILTER_NEAREST;\n"
           "const sampler_t s2 = CLK_NORMALIZED_COORDS_TRUE | CLK_ADDRESS_CLAMP | CLK_FILTER_LINEAR;\n"
           "sampler_t s3 = CLK_ADDRESS_REPEAT | CLK_ADDRESS_NONE;\n");
  EXPECT_TRUE(contains(code, "// #pragma OPENCL EXTENSION cl_khr_fp64"));
  EXPECT_TRUE(lacks(code, "// // #pragma"));
  EXPECT_TRUE(contains(code, "// #pragma unroll"));
  EXPECT_TRUE(contains(code, "// #define CL_VERSION_1_2"));
  EXPECT_TRUE(contains(code, "__device__ int sampler = 0 | 0 | 0;"));
  EXPECT_TRUE(contains(code, "__device__ int s2 = 1 | 0 | 0;"));
  EXPECT_TRUE(contains(code, "int s3 = 0 | 0;"));
}

TEST(TestCUDATranslator, vectorConstructorsAndAccess)
{
  const std::string code = toCUDA("int4 a = (int4){x, (y + 1), z, 0};\n"
                                  "float2 b = (float2)(u, v);\n"
                                  "float c = b.s0 + b.s1 + a.s2 + a.s3;\n"
                                  "int d = f2(int2)(1, 2);\n");
  EXPECT_TRUE(contains(code, "int4 a = make_int4(x, (y + 1), z, 0);"));
  EXPECT_TRUE(contains(code, "float2 b = make_float2(u, v);"));
  EXPECT_TRUE(contains(code, "float c = b.x + b.y + a.z + a.w;"));
  // preceded by an identifier: not a cast
  EXPECT_TRUE(contains(code, "f2(int2)(1, 2)"));
}

TEST(TestCUDATranslator, atomics)
{
  const std::string code = toCUDA("atomic_add(&p[0], 1); atomic_sub(&p[0], 1); atomic_xchg(&p[0], 1); atomic_inc(&p[0]);"
                                  " atomic_dec(&p[0]); atomic_min(&p[0], 1); atomic_max(&p[0], 1); atomic_and(&p[0], 1);"
                                  " atomic_or(&p[0], 1); atomic_xor(&p[0], 1); atomic_fetch_add(&p[0], 1);"
                                  " atomic_fetch_sub(&p[0], 1); atomic_fetch_min(&p[0], 1); atomic_fetch_max(&p[0], 1);"
                                  " atomic_fetch_and(&p[0], 1); atomic_fetch_or(&p[0], 1); atomic_fetch_xor(&p[0], 1);");
  EXPECT_EQ(count(code, "atomicAdd("), 3);
  EXPECT_EQ(count(code, "atomicSub("), 3);
  EXPECT_EQ(count(code, "atomicExch("), 1);
  EXPECT_EQ(count(code, "atomicMin("), 2);
  EXPECT_EQ(count(code, "atomicMax("), 2);
  EXPECT_EQ(count(code, "atomicAnd("), 2);
  EXPECT_EQ(count(code, "atomicOr("), 2);
  EXPECT_EQ(count(code, "atomicXor("), 2);
  EXPECT_TRUE(lacks(code, "atomic_"));
  EXPECT_TRUE(lacks(code, "cle_atomic_cmpxchg"));
}

TEST(TestCUDATranslator, atomicCompareExchangeAddsHelper)
{
  const std::string code = toCUDA("old = atomic_cmpxchg(ptr, cmp, val);");
  EXPECT_TRUE(contains(code, "old = cle_atomic_cmpxchg(ptr, cmp, val);"));
  EXPECT_TRUE(contains(code, "__device__ inline T cle_atomic_cmpxchg(volatile T * ptr, C cmp, V val)"));
  EXPECT_LT(code.find("template <typename T"), code.find("old = "));
}

TEST(TestCUDATranslator, typeConversions)
{
  const std::string code = toCUDA("a = convert_int(x); b = convert_int_sat_rte(x); c = convert_uchar_sat(x); d = convert_uint_rtz(x);"
                                  " e = convert_float_rtp(x); f = convert_short_rtn(x); g = convert_char_sat_rtz(x);"
                                  " h = convert_long_sat_rtp(x); i = convert_ushort_sat_rtn(x); j = convert_int_rte(x);"
                                  " k = convert_float4(v); l = as_float(i); m = as_int(f); n = as_uint(f);");
  EXPECT_TRUE(contains(code, "a = (int)(x);"));
  EXPECT_TRUE(contains(code, "b = (int)(x);"));
  EXPECT_TRUE(contains(code, "c = (unsigned char)(x);"));
  EXPECT_TRUE(contains(code, "d = (unsigned int)(x);"));
  EXPECT_TRUE(contains(code, "e = (float)(x);"));
  EXPECT_TRUE(contains(code, "f = (short)(x);"));
  EXPECT_TRUE(contains(code, "g = (char)(x);"));
  EXPECT_TRUE(contains(code, "h = (long)(x);"));
  EXPECT_TRUE(contains(code, "i = (unsigned short)(x);"));
  EXPECT_TRUE(contains(code, "j = (int)(x);"));
  EXPECT_TRUE(contains(code, "k = make_float4(v);"));
  EXPECT_TRUE(contains(code, "l = __int_as_float(i);"));
  EXPECT_TRUE(contains(code, "m = __float_as_int(f);"));
  EXPECT_TRUE(contains(code, "n = __float_as_uint(f);"));
  EXPECT_TRUE(lacks(code, "convert_"));
}

TEST(TestCUDATranslator, mathFunctions)
{
  const std::string code = toCUDA("native_sin(x); native_cos(x); native_tan(x); native_exp(x); native_exp2(x); native_exp10(x);"
                                  " native_log(x); native_log2(x); native_log10(x); native_sqrt(x); native_rsqrt(x);"
                                  " native_powr(x, y); native_recip(x); native_divide(x, y); half_sin(x); half_cos(x);"
                                  " half_tan(x); half_exp(x); half_exp2(x); half_exp10(x); half_log(x); half_log2(x);"
                                  " half_log10(x); half_sqrt(x); half_rsqrt(x); half_powr(x, y); half_recip(x);"
                                  " half_divide(x, y); mad(a, b, c);");
  EXPECT_EQ(count(code, "__sinf("), 2);
  EXPECT_EQ(count(code, "__cosf("), 2);
  EXPECT_EQ(count(code, "__tanf("), 2);
  EXPECT_EQ(count(code, "__expf("), 2);
  EXPECT_EQ(count(code, "exp2f("), 2);
  EXPECT_EQ(count(code, "__exp10f("), 2);
  EXPECT_EQ(count(code, "__logf("), 2);
  EXPECT_EQ(count(code, "__log2f("), 2);
  EXPECT_EQ(count(code, "__log10f("), 2);
  EXPECT_EQ(count(code, "__fsqrt_rn("), 2);
  EXPECT_EQ(count(code, "rsqrtf("), 2);
  EXPECT_EQ(count(code, "__powf("), 2);
  EXPECT_EQ(count(code, "__frcp_rn("), 2);
  EXPECT_EQ(count(code, "__fdividef("), 2);
  EXPECT_TRUE(contains(code, "fma(a, b, c);"));
  EXPECT_TRUE(lacks(code, "native_"));
  EXPECT_TRUE(lacks(code, "half_"));
}

TEST(TestCUDATranslator, imageOperations)
{
  const std::string code = toCUDA("void f(__read_only image3d_t a, __write_only image3d_t b, __read_only image2d_t c,"
                                  " __write_only image2d_t d, read_only image3d_t e, write_only image3d_t f,"
                                  " read_only image2d_t g, write_only image2d_t h, image2d_t i, image3d_t j)\n"
                                  "{ read_imagef(a, s, p); read_imagei(a, s, p); read_imageui(a, s, p);"
                                  " get_image_width(a); get_image_height(a); get_image_depth(a); }");
  EXPECT_EQ(count(code, "cudaTextureObject_t"), 6);
  EXPECT_EQ(count(code, "cudaSurfaceObject_t"), 4);
  EXPECT_TRUE(contains(code, "tex2D<float4>(a, s, p);"));
  EXPECT_TRUE(contains(code, "tex2D<int4>(a, s, p);"));
  EXPECT_TRUE(contains(code, "tex2D<uint4>(a, s, p);"));
  EXPECT_TRUE(contains(code, "/* get_image_width */ (a);"));
  EXPECT_TRUE(contains(code, "/* get_image_height */ (a);"));
  EXPECT_TRUE(contains(code, "/* get_image_depth */ (a);"));
}

TEST(TestCUDATranslator, typeAliasesAndClamp)
{
  const std::string code = toCUDA("uint a; uint3 b; ushort c; uchar4 d; ulong e; uchar f;\n"
                                  "x = clamp(i, 0, (n - 1)); y = my_clamp(i, 0, n); z = clamp (j, f(1, 2), 3); w = clamp(i, 0);\n"
                                  "v = clamp\n;");
  EXPECT_TRUE(contains(code, "unsigned int a;"));
  EXPECT_TRUE(contains(code, "uint3 b;"));
  EXPECT_TRUE(contains(code, "unsigned short c;"));
  EXPECT_TRUE(contains(code, "uchar4 d;"));
  EXPECT_TRUE(contains(code, "unsigned long e;"));
  EXPECT_TRUE(contains(code, "unsigned char f;"));
  EXPECT_TRUE(contains(code, "x = min(max(i, 0), (n - 1));"));
  EXPECT_TRUE(contains(code, "y = my_clamp(i, 0, n);"));
  EXPECT_TRUE(contains(code, "z = min(max(j, f(1, 2)), 3);"));
  // not a 3-argument call: left unchanged
  EXPECT_TRUE(contains(code, "w = clamp(i, 0);"));
  EXPECT_TRUE(contains(code, "v = clamp\n;"));
}

TEST(TestCUDATranslator, cleanupWhitespace)
{
  const std::string code = toCUDA("int  a;\n\n\n\n\nint    b;\n");
  EXPECT_EQ(code, "int a;\n\nint b;\n");
}

TEST(TestCUDATranslator, translateInPlaceMatchesTranslate)
{
  std::string code = kernel::sobel;
  OpenCLToCUDATranslator().translateInPlace(code);
  EXPECT_EQ(code, toCUDA(kernel::sobel));
}

TEST(TestCUDATranslator, realKernels)
{
  for (const auto & [name, source] : real_kernels)
  {
    SCOPED_TRACE(name);
    const std::string code = toCUDA(source);
    EXPECT_TRUE(contains(code, "extern \"C\" __global__ void " + name));
    EXPECT_TRUE(contains(code, "const int cle_global_size_2)"));
    for (const char * opencl : { "__kernel",
                                 "__global ",
                                 "__local ",
                                 "__constant ",
                                 "get_global_id(",
                                 "get_local_id(",
                                 "get_global_size(",
                                 "barrier(",
                                 "sampler_t",
                                 "CLK_",
                                 "atomic_",
                                 "convert_" })
    {
      EXPECT_TRUE(lacks(code, opencl));
    }
  }
}

// ═══════════════════════════════════════════════════════════════════════════
//  OpenCL → Metal
// ═══════════════════════════════════════════════════════════════════════════

TEST(TestMetalTranslator, kernelSignature)
{
  const std::string code = toMetal("__kernel void k(__global float* dst, const int n, float scale, const unsigned int m)\n"
                                   "{\n"
                                   "  const int x = get_global_id(0);\n"
                                   "  dst[x] = scale * get_global_size(0) + n + m;\n"
                                   "}\n");
  EXPECT_TRUE(contains(code, "kernel void k("));
  EXPECT_TRUE(lacks(code, "__kernel"));
  EXPECT_TRUE(contains(code, "device float* dst"));
  EXPECT_TRUE(contains(code, "constant int& n"));
  EXPECT_TRUE(contains(code, "constant unsigned int& m"));
  // non-const scalars are bound by constant reference and copied into a mutable local
  EXPECT_TRUE(contains(code, "constant float& __cle_arg_scale"));
  EXPECT_TRUE(contains(code, "float scale = __cle_arg_scale;"));
  EXPECT_TRUE(contains(code, "uint3 __cle_gid [[thread_position_in_grid]]"));
  EXPECT_TRUE(contains(code, "uint3 __cle_tpg [[threads_per_grid]])"));
  EXPECT_TRUE(contains(code, "const int x = ((int)__cle_gid.x);"));
  EXPECT_TRUE(contains(code, "scale * ((int)__cle_tpg.x)"));
}

TEST(TestMetalTranslator, kernelParameterEdgeCases)
{
  const std::string code = toMetal("__kernel void a()\n{\n}\n"
                                   "__kernel void b(const int n, // comment, with a comma\n"
                                   "                unsigned float bad, signed int s, unsigned short us, unsigned char uc,"
                                   " unsigned long ul, const half h, int 2bad, const, unsigned)\n{\n}\n");
  EXPECT_TRUE(contains(code, "kernel void a(uint3 __cle_gid [[thread_position_in_grid]]"));
  EXPECT_TRUE(contains(code, "constant int& n"));
  EXPECT_TRUE(lacks(code, "comment"));
  EXPECT_TRUE(contains(code, "unsigned float bad"));
  EXPECT_TRUE(contains(code, "constant signed int& __cle_arg_s"));
  EXPECT_TRUE(contains(code, "constant unsigned short& __cle_arg_us"));
  EXPECT_TRUE(contains(code, "constant unsigned char& __cle_arg_uc"));
  EXPECT_TRUE(contains(code, "constant unsigned long& __cle_arg_ul"));
  EXPECT_TRUE(contains(code, "constant half& h"));
  EXPECT_TRUE(contains(code, "int 2bad"));
}

TEST(TestMetalTranslator, translationIsIdempotentForBuiltins)
{
  std::string                   code = "__kernel void k(__global float* dst)\n{\n  dst[get_global_id(0)] = 0;\n}\n";
  const OpenCLToMetalTranslator translator;
  translator.translateInPlace(code);
  translator.translateInPlace(code);
  EXPECT_EQ(count(code, "[[thread_position_in_grid]]"), 1);
}

TEST(TestMetalTranslator, addressSpaces)
{
  const std::string code = toMetal("#define IMAGE_src_TYPE float*\n"
                                   "#define IMAGE_dst_TYPE device float*\n"
                                   "void f( global float* g, local float* l, __local float* l2, private int p)\n"
                                   "{\n"
                                   "  __local float tile[16];\n"
                                   "  __constant float c = 1;\n"
                                   "  __private int a = 0;\n"
                                   "  __global __global float* d;\n"
                                   "}\n"
                                   "void h(IMAGE_src_PIXEL_TYPE * arr, IMAGE_src_PIXEL_TYPE v, thread IMAGE_dst_PIXEL_TYPE * done,"
                                   " MY_src_PIXEL_TYPE * other);\n");
  EXPECT_TRUE(contains(code, "#define IMAGE_src_TYPE device float*"));
  EXPECT_TRUE(lacks(code, "device device"));
  EXPECT_TRUE(contains(code, " device float* g"));
  EXPECT_TRUE(contains(code, " threadgroup float* l"));
  EXPECT_TRUE(contains(code, "threadgroup float* l2"));
  EXPECT_TRUE(contains(code, "threadgroup float tile[16];"));
  EXPECT_TRUE(contains(code, "constant float c = 1;"));
  EXPECT_TRUE(lacks(code, "private"));
  EXPECT_TRUE(contains(code, "device float* d;"));
  EXPECT_TRUE(contains(code, "thread IMAGE_src_PIXEL_TYPE * arr"));
  EXPECT_TRUE(contains(code, " IMAGE_src_PIXEL_TYPE v"));
  EXPECT_EQ(count(code, "thread IMAGE_dst_PIXEL_TYPE"), 1);
  EXPECT_TRUE(lacks(code, "thread MY_src_PIXEL_TYPE"));
}

TEST(TestMetalTranslator, pragmasSamplersQualifiers)
{
  const std::string code = toMetal("#pragma OPENCL EXTENSION cl_khr_fp64 : enable\n"
                                   "__constant sampler_t sampler = CLK_FILTER_NEAREST;\n"
                                   "const sampler_t s2 = CLK_FILTER_NEAREST;\n"
                                   "__kernel void k() __attribute__((reqd_work_group_size(16, 1, 1)))\n{\n}\n");
  EXPECT_TRUE(contains(code, "// #pragma OPENCL EXTENSION"));
  EXPECT_TRUE(contains(code, "constant sampler_t __cle_sampler = CLK_FILTER_NEAREST;"));
  EXPECT_TRUE(contains(code, "constant sampler_t s2"));
  EXPECT_TRUE(contains(code, "// __attribute__((reqd_work_group_size("));
}

TEST(TestMetalTranslator, vectorConstructors)
{
  const std::string code = toMetal("int4 a = (int4)(1, 2, 3, 4); half2 b = (half2)(x, y); uchar3 c = (uchar3)(0, 0, 0);");
  EXPECT_TRUE(contains(code, "int4 a = int4(1, 2, 3, 4);"));
  EXPECT_TRUE(contains(code, "half2 b = half2(x, y);"));
  EXPECT_TRUE(contains(code, "uchar3 c = uchar3(0, 0, 0);"));
}

TEST(TestMetalTranslator, workItemFunctions)
{
  const std::string code = toMetal("get_global_id(1); get_global_id(2); get_local_id(0); get_local_id(1); get_local_id(2);"
                                   " get_group_id(0); get_group_id(1); get_group_id(2); get_local_size(0); get_local_size(1);"
                                   " get_local_size(2); get_num_groups(0); get_num_groups(1); get_num_groups(2);"
                                   " get_global_size(1); get_global_size(2);");
  for (const char * token : { "__cle_gid.y",
                              "__cle_gid.z",
                              "__cle_tid.x",
                              "__cle_tid.y",
                              "__cle_tid.z",
                              "__cle_tgid.x",
                              "__cle_tgid.y",
                              "__cle_tgid.z",
                              "__cle_tptg.x",
                              "__cle_tptg.y",
                              "__cle_tptg.z",
                              "__cle_tgpg.x",
                              "__cle_tgpg.y",
                              "__cle_tgpg.z",
                              "__cle_tpg.y",
                              "__cle_tpg.z" })
  {
    EXPECT_TRUE(contains(code, std::string("((int)") + token + ")"));
  }
  EXPECT_TRUE(lacks(code, "get_"));
}

TEST(TestMetalTranslator, synchronization)
{
  const std::string code = toMetal("barrier(CLK_LOCAL_MEM_FENCE | CLK_GLOBAL_MEM_FENCE);\n"
                                   "barrier(CLK_GLOBAL_MEM_FENCE | CLK_LOCAL_MEM_FENCE);\n"
                                   "barrier(CLK_LOCAL_MEM_FENCE);\n"
                                   "barrier(CLK_GLOBAL_MEM_FENCE);\n"
                                   "mem_fence(CLK_LOCAL_MEM_FENCE);\n"
                                   "mem_fence(CLK_GLOBAL_MEM_FENCE);\n"
                                   "read_mem_fence(CLK_LOCAL_MEM_FENCE);\n"
                                   "read_mem_fence(CLK_GLOBAL_MEM_FENCE);\n"
                                   "write_mem_fence(CLK_LOCAL_MEM_FENCE);\n"
                                   "write_mem_fence(CLK_GLOBAL_MEM_FENCE);\n");
  EXPECT_EQ(count(code, "threadgroup_barrier(mem_flags::mem_threadgroup);"), 6);
  EXPECT_EQ(count(code, "threadgroup_barrier(mem_flags::mem_device);"), 4);
  EXPECT_EQ(count(code, "\nthreadgroup_barrier("), 9); // every statement on its own line, no prefix left
  EXPECT_TRUE(lacks(code, "fence"));
}

TEST(TestMetalTranslator, atomics)
{
  const std::string code = toMetal("atomic_add(&count[f(i, j)], 1); atomic_sub(&p[0], 1); atomic_min(&p[0], 1);"
                                   " atomic_max(&p[0], 1); atomic_and(&p[0], 1); atomic_or(&p[0], 1); atomic_xor(&p[0], 1);"
                                   " atomic_xchg(&p[0], 1); my_atomic_add(&p[0], 1); atomic_add(p); atomic_add(&p[0], 1");
  EXPECT_TRUE(contains(code, "atomic_fetch_add_explicit((volatile device atomic_int*)&count[f(i, j)], 1, memory_order_relaxed);"));
  for (const char * op : { "sub", "min", "max", "and", "or", "xor" })
  {
    EXPECT_TRUE(
      contains(code, std::string("atomic_fetch_") + op + "_explicit((volatile device atomic_int*)&p[0], 1, memory_order_relaxed);"));
  }
  EXPECT_TRUE(contains(code, "atomic_exchange_explicit((volatile device atomic_int*)&p[0], 1, memory_order_relaxed);"));
  // not an OpenCL atomic call, single argument, unbalanced parenthesis: left unchanged
  EXPECT_TRUE(contains(code, "my_atomic_add(&p[0], 1);"));
  EXPECT_TRUE(contains(code, "atomic_add(p);"));
  EXPECT_TRUE(contains(code, "atomic_add(&p[0], 1"));
}

TEST(TestMetalTranslator, atomicCompareExchange)
{
  const std::string code = toMetal("{\n  old = atomic_cmpxchg(ptr, cmp, as_uint(val));\n}\n"
                                   "if (atomic_cmpxchg(ptr, a, b) == a) {}\n"
                                   "x == atomic_cmpxchg(ptr, a, b);\n"
                                   "y = atomic_cmpxchg(ptr, a);\n"
                                   "z = atomic_cmpxchg(ptr, a, b) + 1;\n"
                                   "w = my_atomic_cmpxchg(ptr, a, b);\n"
                                   "v = atomic_cmpxchg(ptr, a, b");
  EXPECT_EQ(count(code, "atomic_compare_exchange_weak_explicit("), 1);
  EXPECT_TRUE(contains(code, "{ auto __cle_cmpxchg_expected_"));
  EXPECT_TRUE(contains(code, "atomic_compare_exchange_weak_explicit((volatile device atomic_uint*)ptr, &__cle_cmpxchg_expected_"));
  EXPECT_TRUE(contains(code, ", as_type<uint>(val), memory_order_relaxed, memory_order_relaxed); old = __cle_cmpxchg_expected_"));
  // not a plain "lhs = atomic_cmpxchg(a, b, c);" statement: left unchanged
  EXPECT_TRUE(contains(code, "if (atomic_cmpxchg(ptr, a, b) == a)"));
  EXPECT_TRUE(contains(code, "x == atomic_cmpxchg(ptr, a, b);"));
  EXPECT_TRUE(contains(code, "y = atomic_cmpxchg(ptr, a);"));
  EXPECT_TRUE(contains(code, "z = atomic_cmpxchg(ptr, a, b) + 1;"));
  EXPECT_TRUE(contains(code, "w = my_atomic_cmpxchg(ptr, a, b);"));
  EXPECT_TRUE(contains(code, "v = atomic_cmpxchg(ptr, a, b"));
}

TEST(TestMetalTranslator, bitcastAndPrintf)
{
  const std::string code = toMetal("a = as_float(u); b = as_int(f); c = has_float(x);\n"
                                   "printf(\"x=%d (%d)\\n\", x, f(y));\n"
                                   "printf(\"no semicolon\")\n"
                                   "snprintf(buf, 4, \"x\");\n"
                                   "printf(\"unbalanced\"\n");
  EXPECT_TRUE(contains(code, "a = as_type<float>(u);"));
  EXPECT_TRUE(contains(code, "b = as_type<int>(f);"));
  EXPECT_TRUE(contains(code, "c = has_float(x);"));
  EXPECT_TRUE(lacks(code, "x=%d"));
  EXPECT_TRUE(lacks(code, "no semicolon"));
  EXPECT_TRUE(contains(code, "snprintf(buf, 4, \"x\");"));
  EXPECT_TRUE(contains(code, "printf(\"unbalanced\""));
}

TEST(TestMetalTranslator, mathFunctions)
{
  const std::string code =
    toMetal("a = cbrt(x) + pow(x, 2) + sqrt(x) + powr(x, 2) + native_sqrt(x) + half_cos(x);\n"
            "if (x2 >= 0 && y2 >= 0 && z2 >= 0 && x2 < Nx && y2 < Ny && z2 < Nz) { v = POS_src_INSTANCE(x2, y2, z2, 0); }\n");
  EXPECT_TRUE(contains(code, "a = __cle_cbrt(x) + __cle_pow(x, 2) + metal::sqrt(x) + powr(x, 2) + fast::sqrt(x) + fast::cos(x);"));
  EXPECT_TRUE(contains(code, "if (AFFINE_BOUNDS_CHECK(x2, y2, z2, Nx, Ny, Nz))"));
  EXPECT_TRUE(contains(code, "POS_src_INSTANCE(x2 + 1e-5f, y2 + 1e-5f, z2 + 1e-5f, 0)"));
}

TEST(TestMetalTranslator, cleanupWhitespace)
{
  const std::string code = toMetal("int  a;\n\n\n\n\nint    b;\n");
  EXPECT_EQ(code, "int a;\n\nint b;\n");
}

TEST(TestMetalTranslator, realKernels)
{
  for (const auto & [name, source] : real_kernels)
  {
    SCOPED_TRACE(name);
    const std::string code = toMetal(source);
    EXPECT_TRUE(contains(code, "kernel void " + name));
    EXPECT_TRUE(contains(code, "uint3 __cle_gid [[thread_position_in_grid]]"));
    for (const char * opencl : { "__kernel",
                                 "__global ",
                                 "__local ",
                                 "__constant ",
                                 "get_global_id(",
                                 "get_local_id(",
                                 "get_global_size(",
                                 "barrier(CLK",
                                 "printf(",
                                 "atomic_add(",
                                 "atomic_inc(" })
    {
      EXPECT_TRUE(lacks(code, opencl));
    }
  }
}
