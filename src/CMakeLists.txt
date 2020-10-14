
set(source_path ${CMAKE_CURRENT_BINARY_DIR})

# set source files
set(SOURCES
	CLE.cpp
	cleGPU.cpp
	cleKernel.cpp
	cleAddImageAndScalarKernel.cpp
	cleSmallerOrEqualConstantKernel.cpp
	cleMaximumZProjectionKernel.cpp
	cleMean2DSphereKernel.cpp
	cleAbsoluteKernel.cpp
	cleBuffer.cpp
	tiffreader.cpp
	tiffwriter.cpp
	utils.cpp
)

