# Copyright (c) 2006-2019  Made to Order Software Corp.  All Rights Reserved.
#
# https://snapwebsites.org/
# contact@m2osw.com
# 
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.

cmake_minimum_required(VERSION 2.8.4)

project(snapcatch2)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})

find_package( SnapCMakeModules REQUIRED )

SnapGetVersion( SNAPCATCH2 ${CMAKE_CURRENT_SOURCE_DIR} )

set(SNAPCATCH2_VERSION ${SNAPCATCH2_VERSION_MAJOR}.${SNAPCATCH2_VERSION_MINOR}.${SNAPCATCH2_VERSION_PATCH})
set(OUTPUT_PATH ${CMAKE_BINARY_DIR}/Catch2-${SNAPCATCH2_VERSION})

message("CMAKE_INSTALL_LIBDIR = ${CMAKE_INSTALL_LIBDIR}")
add_custom_command(
    OUTPUT ${OUTPUT_PATH}/out/include/catch2/catch.hpp
    COMMAND echo "------------ sub-cmake call in `pwd` -- ${CMAKE_INSTALL_PREFIX}"
    COMMAND rm -rf ${OUTPUT_PATH}
    COMMAND tar xf ${CMAKE_SOURCE_DIR}/catch2-${SNAPCATCH2_VERSION}.tar.gz
    COMMAND if test -f ${CMAKE_SOURCE_DIR}/Catch2-${SNAPCATCH2_VERSION}.patch\; then patch ${OUTPUT_PATH}/projects/CMakeLists.txt <${CMAKE_SOURCE_DIR}/Catch2-${SNAPCATCH2_VERSION}.patch\; fi
    COMMAND mkdir -p ${OUTPUT_PATH}/out
    COMMAND mkdir -p ${OUTPUT_PATH}/projects/Generated
    COMMAND cd ${OUTPUT_PATH}/projects/Generated && ${CMAKE_COMMAND} -DCATCH_BUILD_TESTING=off -DCATCH_INSTALL_HELPERS=on -DCATCH_INSTALL_DOCS=on -DCMAKE_INSTALL_PREFIX=${OUTPUT_PATH}/out ../..
    COMMAND make -C ${OUTPUT_PATH}/projects/Generated install
    COMMAND if test -d ${OUTPUT_PATH}/out/lib/*-linux-gnu\; then mv ${OUTPUT_PATH}/out/lib ${OUTPUT_PATH}/out/ignore\; mv ${OUTPUT_PATH}/out/ignore/*-linux-gnu ${OUTPUT_PATH}/out/lib\; rmdir ${OUTPUT_PATH}/out/ignore\; fi
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

message( "Dependency: ${OUTPUT_PATH}" )
add_custom_target( run ALL
    DEPENDS ${OUTPUT_PATH}/out/include/catch2/catch.hpp
    COMMENT "Generating API documentation with Doxygen" VERBATIM
)

install(
    DIRECTORY
        ${OUTPUT_PATH}/out/include/

    DESTINATION
        include
)

install(
    DIRECTORY
        ${OUTPUT_PATH}/out/lib/

    DESTINATION
        lib

    FILES_MATCHING PATTERN
        lib*

    PATTERN cmake EXCLUDE
)

# The current setup places the cmake directory under
#     /usr/lib/cmake
# On our end we want it under
#     /usr/share/cmake
#
install(
    DIRECTORY
        ${OUTPUT_PATH}/out/lib/cmake/

    DESTINATION
        share/cmake
)

install(
    DIRECTORY
        ${OUTPUT_PATH}/out/share/

    DESTINATION
        share
)

install(
    FILES
        snapcatch2.hpp

    DESTINATION
        include/catch2
)

# vim: ts=4 sw=4 et nocindent
