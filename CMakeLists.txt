
cmake_minimum_required (VERSION 2.6)
project (libxml-libbz2)
add_executable(dumper src/dumper.c)

find_package (BZip2)
include_directories(${BZIP_INCLUDE_DIRS})
target_link_libraries (dumper ${BZIP2_LIBRARIES})

