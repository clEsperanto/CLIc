
set(source_path ${CMAKE_CURRENT_SOURCE_DIR})

# set source files
set(SOURCES
    # API functionality
	${source_path}/CLE.cpp
	${source_path}/cleGPU.cpp
	${source_path}/cleKernel.cpp
	
	${source_path}/cleLightObject.cpp
	${source_path}/cleBuffer.cpp

	# ${source_path}/cleFloat.cpp
	# ${source_path}/cleInt.cpp

	# API utilities
	${source_path}/tiffreader.cpp
	${source_path}/tiffwriter.cpp
	${source_path}/utils.cpp

    # Kernels
	${source_path}/cleAddImageAndScalarKernel.cpp
	${source_path}/cleSmallerOrEqualConstantKernel.cpp
	${source_path}/cleMaximumZProjectionKernel.cpp
	${source_path}/cleMaximumYProjectionKernel.cpp
	${source_path}/cleMaximumXProjectionKernel.cpp
	${source_path}/cleMaximumOfAllPixelsKernel.cpp
	${source_path}/cleMean2DSphereKernel.cpp
	${source_path}/cleAbsoluteKernel.cpp


	PARENT_SCOPE)

