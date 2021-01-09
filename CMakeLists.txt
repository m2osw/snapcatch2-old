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

# The copy of the cmake folder is for developers; as far as the package is
# concerned, the "normal" location (as choosen by Catch2/CMake/LinuxGNU) is
# used as expected (and it changed between drastically between 16.04 & 18.04)
# If that becomes an issue in the future, we may instead have to add another
# search path to look for cmake files under dist/lib/cmake
#
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
    COMMAND if test -d ${OUTPUT_PATH}/out/lib/*-linux-gnu/cmake\; then cp -r ${OUTPUT_PATH}/out/lib/*-linux-gnu/cmake ${OUTPUT_PATH}/out/share/.\; fi
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
