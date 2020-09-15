
# set headers
set(header_path ${Clic_SOURCE_DIR}/include)
set(header 
${header_path}/cleGPU.h
${header_path}/cleKernel.h
${header_path}/cleAddImageAndScalar.h
${header_path}/cleSmallerOrEqualConstant.h
${header_path}/cleMaximumZProjection.h
${header_path}/cleBuffer.h
${header_path}/tiffreader.h
${header_path}/tiffwriter.h
${header_path}/utils.h
)

# set source files
set(src
cleGPU.cpp
cleKernel.cpp
cleAddImageAndScalar.cpp
cleSmallerOrEqualConstant.cpp
cleMaximumZProjection.cpp
cleBuffer.cpp
tiffreader.cpp
tiffwriter.cpp
utils.cpp
)

# configure library
configure_file("cleConfig.h.in" ${CMAKE_CURRENT_BINARY_DIR}/cleConfig.h)

# define library target
add_library(${PROJECT_NAME} ${header} ${src})
target_link_libraries(${PROJECT_NAME} OpenCL::OpenCL TIFF::TIFF)
target_include_directories(${PROJECT_NAME} PUBLIC
		$<BUILD_INTERFACE:${Clic_SOURCE_DIR}/include>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}> 
		$<INSTALL_INTERFACE:${include_dest}> 
		$<INSTALL_INTERFACE:${lib_dest}> 
		)

install(TARGETS ${PROJECT_NAME} EXPORT ${PROJECT_NAME} DESTINATION ${lib_dest})
install(FILES ${header} DESTINATION ${include_dest})
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/cleConfig.h DESTINATION ${lib_dest})
