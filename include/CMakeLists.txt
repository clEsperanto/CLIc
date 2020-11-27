
set(header_path ${CMAKE_CURRENT_SOURCE_DIR})

# set header files
set(HEADERS 
    # Gateway
    ${header_path}/CLE.h
    
    #Core
    ${header_path}/cleGPU.h
    ${header_path}/clePlatformManager.h
    ${header_path}/cleCommandQueueManager.h
    ${header_path}/cleDeviceManager.h
    ${header_path}/cleContextManager.h


    # Datatype
    ${header_path}/cleLightObject.h
    ${header_path}/cleObject.h
    ${header_path}/cleScalar.h
    ${header_path}/cleBuffer.h
    ${header_path}/cleFloat.h
    ${header_path}/cleInt.h

    # Kernels
    ${header_path}/cleKernel.h
    ${header_path}/cleKernelList.h
    ${header_path}/cleAbsoluteKernel.h
    ${header_path}/cleAddImageAndScalarKernel.h
    ${header_path}/cleAddImagesWeightedKernel.h
    ${header_path}/cleCopyKernel.h
    ${header_path}/cleDilateSphereKernel.h
    ${header_path}/cleErodeSphereKernel.h
    ${header_path}/cleEqualConstantKernel.h
    ${header_path}/cleGaussianBlurKernel.h
    ${header_path}/cleGreaterConstantKernel.h
    ${header_path}/cleGreaterOrEqualConstantKernel.h
    ${header_path}/cleMaximumZProjectionKernel.h
    ${header_path}/cleMaximumYProjectionKernel.h
    ${header_path}/cleMaximumXProjectionKernel.h
    ${header_path}/cleMaximumOfAllPixelsKernel.h
    ${header_path}/cleMinimumZProjectionKernel.h
    ${header_path}/cleMinimumYProjectionKernel.h
    ${header_path}/cleMinimumXProjectionKernel.h
    ${header_path}/cleMinimumOfAllPixelsKernel.h
    ${header_path}/cleMean2DSphereKernel.h
    ${header_path}/cleNonzeroMinimumBoxKernel.h
    ${header_path}/cleNotEqualConstantKernel.h
    ${header_path}/cleSmallerConstantKernel.h
    ${header_path}/cleSmallerOrEqualConstantKernel.h
    ${header_path}/cleSobelKernel.h
    ${header_path}/cleSetKernel.h
    ${header_path}/cleSetNonzeroPixelsToPixelindexKernel.h
    ${header_path}/cleSeparableKernel.h
    ${header_path}/cleExecuteSeparableKernel.h

    # utilities
    ${header_path}/tiffreader.h
    ${header_path}/tiffwriter.h
    ${header_path}/image.h
    ${header_path}/utils.h

    PARENT_SCOPE)
