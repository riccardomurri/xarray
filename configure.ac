#                                               -*- Autoconf -*-
#
# This file is part of `xarray`. See https://github.com/riccardomurri/xarray
# for more information.
#
# Copyright (c) 2010, 2011 riccardo.murri@gmail.com.  All rights reserved.
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public License
# as published by the Free Software Foundation; either version 3 of
# the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin St, Suite 500, Boston, MA
# 02110-1335 USA
#
#Process this file with autoconf to produce a configure script.

AC_PREREQ([2.65])
AC_INIT([xarray], [1.0], [riccardo.murri+xarray@gmail.com])
AC_CONFIG_SRCDIR([xarray.h])
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_MACRO_DIR([build-aux/m4])
AM_INIT_AUTOMAKE([1.10 no-define])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CC
AM_PROG_CC_C_O

# Checks for libraries.
gl_EARLY
gl_INIT

# Checks for header files.
AC_CHECK_HEADERS([stdlib.h string.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_CHECK_FUNCS([memmove])

AC_CONFIG_FILES([Makefile])
AC_CONFIG_FILES([gnulib/src/Makefile])
AC_OUTPUT
