# Process this file with automake to produce Makefile.in
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

# `xarray` is an include-only library
include_HEADERS = xarray.h


# required by GNUlib
SUBDIRS = gnulib/src
ACLOCAL_AMFLAGS = -I gnulib/m4
EXTRA_DIST = gnulib/m4/gnulib-cache.m4


# tests
check_PROGRAMS = test-xarray
test_xarray_SOURCES = test-xarray.c
test_xarray_CPPFLAGS = -I gnulib/src
test_xarray_LDADD = ./gnulib/src/libgnu.a

TESTS = test-xarray
