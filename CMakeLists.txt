
cmake_minimum_required (VERSION 2.6)
project (libxml-libbz2)

find_package (LibXml2 REQUIRED)
include_directories (${LIBXML2_INCLUDE_DIR})

find_package (BZip2 REQUIRED)
include_directories(${BZIP_INCLUDE_DIRS})

add_executable(dumper src/dumper.c)
target_link_libraries (dumper ${BZIP2_LIBRARIES} ${LIBXML2_LIBRARIES})

