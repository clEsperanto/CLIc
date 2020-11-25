
set(source_path ${CMAKE_CURRENT_SOURCE_DIR})

# set source files
set(SOURCES
    # Gateway
    ${source_path}/CLE.cpp
    
    #Core
    ${source_path}/cleGPU.cpp
    ${source_path}/clePlatformManager.cpp
    ${source_path}/cleCommandQueueManager.cpp
    ${source_path}/cleDeviceManager.cpp
    ${source_path}/cleContextManager.cpp

    # Datatype
    ${source_path}/cleLightObject.cpp
    ${source_path}/cleBuffer.cpp
    ${source_path}/cleFloat.cpp
    ${source_path}/cleInt.cpp


    # Kernels
    ${source_path}/cleKernel.cpp
    ${source_path}/cleAbsoluteKernel.cpp
    ${source_path}/cleAddImageAndScalarKernel.cpp
    ${source_path}/cleAddImagesWeightedKernel.cpp
    ${source_path}/cleDilateSphereKernel.cpp
    ${source_path}/cleErodeSphereKernel.cpp
    ${source_path}/cleGreaterConstantKernel.cpp
    ${source_path}/cleMaximumZProjectionKernel.cpp
    ${source_path}/cleMaximumYProjectionKernel.cpp
    ${source_path}/cleMaximumXProjectionKernel.cpp
    ${source_path}/cleMaximumOfAllPixelsKernel.cpp
    ${source_path}/cleMean2DSphereKernel.cpp
    ${source_path}/cleNonzeroMinimumBoxKernel.cpp
    ${source_path}/cleSmallerOrEqualConstantKernel.cpp
    ${source_path}/cleSobelKernel.cpp
    ${source_path}/cleSetKernel.cpp
    ${source_path}/cleSetNonzeroPixelsToPixelindexKernel.cpp

    # utilities
    ${source_path}/tiffreader.cpp
    ${source_path}/tiffwriter.cpp
    ${source_path}/utils.cpp
    PARENT_SCOPE)

