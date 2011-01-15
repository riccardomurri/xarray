Introduction
------------
The XArray module consists of the sole `xarray.h`_ header file,
which defines a single C preprocessor macro XARRAY_DECLARE,
for declaring variable-sized arrays and defining functions to
operate on them.


Usage
-----
The macro invocation::

    XARRAY_DECLARE(aname, elt_t, extra)

declares an xarray type ``aname_t`` and functions to operate on it.

An xarray is a ``struct`` where the last element is a variable-size
array, holding instances of elements of type *elt_t*.  An xarray
is always stored in a (``malloc``'d) contiguous block of memory.
As a special case, when no *extra* members have been declared in
the xarray, the xarray reduces to a homogeneous variable-size
array.

Invocation of XARRAY_DECLARE requires the three arguments *aname*, 
*elt_t*, and *extra*.

``aname`` 
  a valid C identifier; XARRAY_DECLARE declares a ``struct aname_t``
  and defines a number of functions (see below) to operate on it,
  all beginning with the prefix *aname*;

``elt_t`` 
  type of the elements in the variable-size array

``extra`` 
  declarations of other fields in the ``aname_t`` structure; any
  statement that is legal in a C ``struct`` declaration is permitted
  here.  If *extra* is void, then ``aname_t`` is just a
  variable-size array stored in a contiguous block of memory.

XARRAY_DECLARE defines functions to allocate a new xarray
(``aname_alloc``), append elements at the end of the array,
automatically relocating it if needed (``aname_extend``,
``aname_extend1``), access/insert/remove elements at specific
positions (``aname_at``, ``aname_insert``, ``aname_erase``,
``aname_shorten``, ``aname_shorten1``), wipe off all elements from an
array (``aname_clear``) or dispose of it and return the storage to the
system (``aname_free``).

The XARRAY_DECLARE macro defines the following functions.  Note that
functions that modify the variable-sized array take a parameter of
type ``aname_t** xa`` (i.e., the address to a pointer to an xarray):
since an xarray is always stored in a contiguous block of memory,
resizing the array may require moving the block to a different memory
location, hence the pointer to the xarray must be modified.

``aname_t aname_alloc(const size_t nmemb)``
  Return a newly-allocated xarray, or ``NULL`` if the allocation
  failed. The xarray is initially sized to contain ``nmemb``
  elements without the need to resize.

``aname_t aname_alloc_placed(void* xa, const size_t size)``
  Initialize a new xarray placed at address *ptr* and fitting within
  *size* bytes. Return pointer to the start of the xarray, or
  ``NULL`` if there was some error (e.g., *size* is insufficient).

``size_t aname_size(const aname_t* const xa)``
  Return a pointer to the element in the xarray at position ``pos``.
  Xarray positions follow the usual C convention, ranging from 0 to
  ``xa->count-1``.

``void* aname_lb(const aname_t* const xa)``
  Return a pointer to the first byte in the xarray, i.e., the
  address of the first used memory location in a ``aname_t`` instance.
  (Which is not the first byte in the variable-size array, unless
  the *extra* parameter to XARRAY_DECLARE was empty.)

``void* aname_ub(const aname_t* const xa)``
  Return a pointer past the last used byte in the xarray, i.e.,
  an ``aname_t`` instance ``xa`` uses memory locations ``p`` such that
  ``aname_lb(xa) <= p < aname_ub(xa)``

``elt_t* aname_at(aname_t* const xa, size_t pos)``
  Return a pointer to the element in the xarray at position ``pos``.
  Xarray positions follow the usual C convention, ranging from 0 to
  ``xa``->count-1.

``void aname_reserve(aname_t** xa, const size_t nmemb)``
  Ensure that xarray ``xa`` can be extended by appending ``nmemb``
  elements at the end of the array without incurring in any
  relocation; return a pointer to the first newly-added place, or
  ``NULL`` on failure. Note that the newly-added positions are not
  initialized.

``elt_t* aname_extend(aname_t** xa, size_t nmemb)``
  Extend xarray ``xa`` by adding ``nmemb`` elements at the end of the
  array; return a pointer to the first newly-added place, or ``NULL``
  on failure. Note that the newly-added positions are not
  initialized.

``elt_t* aname_extend1(aname_t** xa)``
  Extend xarray ``xa`` by adding 1 element at the end of the array,
  and return a pointer to the newly-added place, or ``NULL`` on    
  failure.

``void aname_shorten(aname_t** const xa, const size_t nmemb)``
  Shorten ``xa`` by removing *nmemb* elements from the end of the array.

``void aname_shorten1(aname_t** const xa)``
  Shorten ``xa`` by removing one element from the end of the array.

``elt_t* aname_insert(aname_t** const za, const size_t pos)``
  Insert one element in the array at position ``pos`` and return
  pointer to the newly-added place, or ``NULL`` if some error
  occurred.  XArray positions follow the usual C convention, ranging
  from 0 to ``xa->count-1``. Note that the new location is not
  initialized.

``aname_erase(aname_t** const xa, const size_t pos)``
  Remove the element at position ``pos`` from the xarray. XArray
  positions follow the usual C convention, ranging from 0 to ``xa``->count-1.

``aname_clear(aname_t** const xa)``
  Forget all the contents and return ``xa`` to 0 size. The memory
  allocated to the array is *not* freed.

``aname_free(aname_t *xa)``
  Free array ``xa`` and return allocated memory to the system.


Examples
--------

The provided file ``test-xarray.c`` provides example usage of XArray.
It simulates the use of XArray for implementing a sparse matrix row.


Dependencies
------------

The ``xarray.h`` header file includes in turn an ``xalloc.h`` header.
The ``xalloc.h`` header should define functions xmalloc(), xrealloc(),
xfree() with the same signature as malloc(), realloc() and free() from
the C standard library, except that they never return in case
allocation fails, i.e., we don't have to check for a non-zero result
of xmalloc() or xrealloc().  The GPL'ed GNUlib implementation is
bundled with the ``xarray`` sources in the directory ``gnulib/`` and
used for the test program.

Additionally, ``xarray.h`` depends on a macro ``_inline``; if it's not
defined at the time of inclusion, it will be defined as ``static
inline``.  If your C compiler does not support C99's ``static
inline``, you might want to redefine macro ``_inline`` to something
different, or make its definition empty if the compiler does not
support inlining at all, *before* including ``xarray.h``.


Copyright and license
---------------------

Copyright (c) 2010, 2011 riccardo.murri@gmail.com.  All rights reserved.

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public License
as published by the Free Software Foundation; either version 3 of
the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Suite 500, Boston, MA
02110-1335 USA


.. References

.. _`xarray.h`: http://github.com/riccardomurri/xarray/blob/master/xarray.h
.. _`test-xarray.c`: http://github.com/riccardomurri/xarray/blob/master/test-xarray.c
