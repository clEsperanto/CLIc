
set(header_path ${CMAKE_CURRENT_SOURCE_DIR})

# set header files
set(HEADERS 
    ${header_path}/CLE.h
    ${header_path}/cleKernelList.h
    ${header_path}/cleGPU.h
    ${header_path}/cleKernel.h
    ${header_path}/cleBuffer.h
    ${header_path}/cleAddImageAndScalarKernel.h
    ${header_path}/cleSmallerOrEqualConstantKernel.h
    ${header_path}/cleMaximumZProjectionKernel.h
    ${header_path}/cleMean2DSphereKernel.h
    ${header_path}/cleAbsoluteKernel.h
    ${header_path}/tiffreader.h
    ${header_path}/tiffwriter.h
    ${header_path}/image.h
    ${header_path}/utils.h

    PARENT_SCOPE)
