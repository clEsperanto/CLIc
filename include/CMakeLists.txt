
set(header_path ${CMAKE_CURRENT_SOURCE_DIR})

# set header files
set(HEADERS 
    # API functionality
    ${header_path}/CLE.h
    ${header_path}/cleGPU.h
    ${header_path}/cleKernel.h

    ${header_path}/cleLightObject.h
    ${header_path}/cleObject.h
    ${header_path}/cleScalar.h
    ${header_path}/cleBuffer.h
    # ${header_path}/cleFloat.h
    # ${header_path}/cleInt.h

    # API utilities
    ${header_path}/tiffreader.h
    ${header_path}/tiffwriter.h
    ${header_path}/image.h
    ${header_path}/utils.h
    
    # Kernels
    ${header_path}/cleKernelList.h
    ${header_path}/cleAddImageAndScalarKernel.h
    ${header_path}/cleSmallerOrEqualConstantKernel.h
    ${header_path}/cleMaximumZProjectionKernel.h
    ${header_path}/cleMaximumYProjectionKernel.h
    ${header_path}/cleMaximumXProjectionKernel.h
    ${header_path}/cleMaximumOfAllPixelsKernel.h
    ${header_path}/cleMean2DSphereKernel.h
    ${header_path}/cleAbsoluteKernel.h


    PARENT_SCOPE)
