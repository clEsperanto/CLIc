# Set SOURCES variable
# set(SOURCES
#   foo.cpp
# )
include_directories(src)
file(GLOB SOURCES src/*.cpp)

# Set HEADERS_PUBLIC variable (public headers, included in the library)
# set(HEADERS_PUBLIC
#   foo.h
# )
include_directories(include)
file(GLOB HEADERS_PUBLIC include/*.h)

# Set HEADERS_PRIVATE variable, if needed.
# (these private headers won't be included in the library)
# set(HEADERS_PRIVATE
#     foo_private.h
# )

include_directories(${CLI_KERNELS_DIR})
file(GLOB KERNELS ${CLI_KERNELS_DIR}/*.cl)

include(${PROJECT_SOURCE_DIR}/cmake/LibraryConfig.cmake)
