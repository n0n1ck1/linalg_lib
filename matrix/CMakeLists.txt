get_filename_component(LIB_INCLUDE_PATH ".." ABSOLUTE)
get_filename_component(LIB_PATH "." ABSOLUTE)


# Find all headers and sources
file(GLOB_RECURSE LIB_CXX_SOURCES ${LIB_PATH}/*.cpp)
file(GLOB_RECURSE LIB_HEADERS ${LIB_PATH}/*.h)
# Create library
add_library(matrix STATIC ${LIB_CXX_SOURCES} ${LIB_HEADERS})
# Set include path for library
target_include_directories(matrix PUBLIC ${LIB_INCLUDE_PATH})
