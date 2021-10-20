# Copyright (c) 2006-2021  Made to Order Software Corp.  All Rights Reserved.
#
# https://snapwebsites.org/project/snapcatch2
# contact@m2osw.com
# 
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License along
# with this program; if not, write to the Free Software Foundation, Inc.,
# 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

cmake_minimum_required(VERSION 2.8.4)

project(snapcatch2)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})

find_package(SnapCMakeModules REQUIRED)

SnapGetVersion(SNAPCATCH2 ${CMAKE_CURRENT_SOURCE_DIR})

set(SNAPCATCH2_VERSION ${SNAPCATCH2_VERSION_MAJOR}.${SNAPCATCH2_VERSION_MINOR}.${SNAPCATCH2_VERSION_PATCH})
set(OUTPUT_PATH ${CMAKE_BINARY_DIR}/Catch2-${SNAPCATCH2_VERSION})

message("CMAKE_INSTALL_LIBDIR = ${CMAKE_INSTALL_LIBDIR}")

# The copy of the cmake folder is for developers; as far as the package is
# concerned, the "normal" location (as choosen by Catch2/CMake/LinuxGNU) is
# used as expected (and it changed between drastically between 16.04 & 18.04)
# If that becomes an issue in the future, we may instead have to add another
# search path to look for cmake files under dist/lib/cmake
#    COMMAND if test -d ${OUTPUT_PATH}/out/lib/*-linux-gnu/cmake\; then cp -r ${OUTPUT_PATH}/out/lib/*-linux-gnu/cmake ${OUTPUT_PATH}/out/share/.\; fi
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
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

message("Dependency: ${OUTPUT_PATH}")
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

add_subdirectory(cmake)

# vim: ts=4 sw=4 et nocindent
