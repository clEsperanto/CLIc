cmake_minimum_required(VERSION 3.0)

project(CLIc)

find_package(OpenCL REQUIRED)
find_package(TIFF REQUIRED)

set(source 
src/main.cpp
src/tiffreader.cpp
src/tiffwriter.cpp
)

add_executable(CLIc ${source})

target_link_libraries(CLIc ${OpenCL_LIBRARIES} ${TIFF_LIBRARIES})
set_target_properties(CLIc PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

set(CLI_Path ${CMAKE_CURRENT_SOURCE_DIR}/cli-opencl-kernels/src/main/java/net/haesleinhuepf/clij/kernels CACHE PATH "Path to CLI kernels folder" FORCE )
set(CLP_Path ${CMAKE_CURRENT_SOURCE_DIR}/ressources CACHE PATH "Path to CLI preamble folder" FORCE )

target_compile_definitions(CLIc PRIVATE -DCLI_PATH=\"${CLI_Path}\")
target_compile_definitions(CLIc PRIVATE -DCLP_PATH=\"${CLP_Path}\")
