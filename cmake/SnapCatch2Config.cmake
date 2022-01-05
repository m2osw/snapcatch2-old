# - Try to find SnapCatch2
#
# Once done this will define
#
# SNAPCATCH2_FOUND        - System has SnapCatch2
# SNAPCATCH2_INCLUDE_DIRS - The SnapCatch2 include directories
#
# License:
#
# Copyright (c) 2013-2022  Made to Order Software Corp.  All Rights Reserved
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

find_path(
    SNAPCATCH2_INCLUDE_DIR
        catch2/snapcatch2.hpp

    PATHS
        ENV SNAPCATCH2_INCLUDE_DIR
)

mark_as_advanced(
    SNAPCATCH2_INCLUDE_DIR
)

set(SNAPCATCH2_INCLUDE_DIRS ${SNAPCATCH2_INCLUDE_DIR})

include(FindPackageHandleStandardArgs)

# handle the QUIETLY and REQUIRED arguments and set SnapCatch2_FOUND to
# TRUE if all listed variables are TRUE
find_package_handle_standard_args(
    SnapCatch2
    DEFAULT_MSG
    SNAPCATCH2_INCLUDE_DIR
)

# vim: ts=4 sw=4 et
