## Process this file with automake to produce Makefile.in

# `xarray` is a include-only library
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
