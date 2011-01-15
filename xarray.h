/**
 * @file   xarray.h
 *
 * Definition of the @ref XARRAY_DECLARE macro.
 *
 * @author  riccardo.murri@gmail.com
 * @version 1.0
 *
 * @mainpage
 *
 * The XArray module consists of the sole @ref xarray.h header file,
 * which defines a single C preprocessor macro @ref XARRAY_DECLARE,
 * for declaring variable-sized arrays and defining functions to
 * operate on them.
 */
/*
 * Copyright (c) 2010, 2011 riccardo.murri@gmail.com.  All rights reserved.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation; either version 3 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Suite 500, Boston, MA
 * 02110-1335 USA
 *
 */

#ifndef XARRAY_H
#define XARRAY_H

/* The "xalloc.h" header should define functions xmalloc(),
 * xrealloc(), xfree() with the same signature as malloc(), realloc()
 * and free() from the C standard library, except that they never
 * return in case allocation fails, i.e., we don't have to check for a
 * non-zero result of xmalloc() or xrealloc().  The GPL'ed GNUlib
 * implementation is bundled with the `xarray` sources in the
 * directory `gnulib/` and used for the test program.
 */
#include <xalloc.h>

#include <assert.h>
#include <string.h> // memmove
#include <stdlib.h> // malloc, realloc, free


#ifndef _inline
/** Define to your compiler's equivalent of C99's @c static @c inline;
    make the definition empty if the compiler does not support inlining. */
#define _inline static inline
#endif 


/** @def XARRAY_DECLARE(aname, elt_t, extra)
 *
 * Declare an xarray type @c aname_t and functions to operate on it.
 *
 * An xarray is a @c struct where the last element is a variable-size
 * array, holding instances of elements of type @a elt_t.  An xarray
 * is always stored in a (@c malloc'd) contiguous block of memory.
 * As a special case, when no @a extra members have been declared in
 * the xarray, the xarray reduces to a homogeneous variable-size
 * array.
 *
 * Invocation of XARRAY_DECLARE requires the three arguments @p aname, 
 * @p elt_t, and @p extra.
 * @param aname a valid C identifier; XARRAY_DECLARE declares a
 *     @c struct @c aname_t and defines a number of functions
 *     (see below) to operate on it, all beginning with the prefix 
 *     @a aname;
 * @param elt_t type of the elements in the variable-size array
 * @param extra declarations of other fields in the @c aname_t
 *     structure; any statement that is legal in a C @c struct declaration
 *     is permitted here.  If @a extra is void, then @c aname_t
 *     is just a variable-size array stored in a contiguous block of memory.
 *
 * XARRAY_DECLARE defines functions to allocate a new xarray (@c
 * aname_alloc), append elements at the end of the array,
 * automatically relocating it if needed (@c aname_extend, @c
 * aname_extend1), access/insert/remove elements at specific positions
 * (@c aname_at, @c aname_insert, @c aname_erase, @c aname_shorten, @c
 * aname_shorten1), wipe off all elements from an array (@c
 * aname_clear) or dispose of it and return the storage to the system
 * (@c aname_free).
 *
 * The XARRAY_DECLARE macro defines the following functions.  Note
 * that functions that modify the variable-sized array take a
 * parameter of type @code aname_t** xa @endcode (i.e., the address to a pointer
 * to an xarray): since an xarray is always stored in a contiguous
 * block of memory, resizing the array may require moving the block to
 * a different memory location, hence the pointer to the xarray must
 * be modified.
 *
 * @li @code aname_t aname_alloc(const size_t nmemb) @endcode
 *  Return a newly-allocated xarray, or @c NULL if the allocation       
 *  failed. The xarray is initially sized to contain @c nmemb elements 
 *  without the need to resize.
 *
 * @li @code aname_t aname_alloc_placed(void* xa, const size_t size) @endcode
 *  Initialize a new xarray placed at address @a ptr and fitting within 
 *  @a size bytes. Return pointer to the start of the xarray, or       
 *  @c NULL if there was some error (e.g., @a size is insufficient). 
 *
 * @li @code size_t aname_size(const aname_t* const xa) @endcode
 *  Return a pointer to the element in the xarray at position @c pos.   
 *  Xarray positions follow the usual C convention, ranging from 0 to @c  
 *  xa->count-1. 
 *
 * @li @code void* aname_lb(const aname_t* const xa) @endcode
 *   Return a pointer to the first byte in the xarray, i.e., the address
 *   of the first used memory location in a @c aname_t instance. 
 *   (Which is not the first byte in the variable-size array, unless
 *   the @a extra parameter to XARRAY_DECLARE was empty.)
 *
 * @li @code void* aname_ub(const aname_t* const xa) @endcode
 *   Return a pointer past the last used byte in the xarray, i.e.,
 *   an @c aname_t instance @c xa uses memory locations @c p such that
 *   @code aname_lb(xa) <= p < aname_ub(xa) @endcode
 *
 * @li @code elt_t* aname_at(aname_t* const xa, size_t pos) @endcode
 *   Return a pointer to the element in the xarray at position @c pos.   
 *   Xarray positions follow the usual C convention, ranging from 0 to @c  
 *   xa->count-1.
 *
 * @li @code void aname_reserve(aname_t** xa, const size_t nmemb) @endcode
 *  Ensure that xarray @c xa can be extended by appending @c nmemb      
 *  elements at the end of the array without incurring in any          
 *  relocation; return a pointer to the first newly-added place, or @c 
 *  NULL on failure. Note that the newly-added positions are not       
 *  initialized.
 *
 * @li @code elt_t* aname_extend(aname_t** xa, size_t nmemb) @endcode
 *  Extend xarray @c xa by adding @c nmemb elements at the end of the  
 *  array; return a pointer to the first newly-added place, or @c      
 *  NULL on failure. Note that the newly-added positions are not       
 *  initialized.
 *
 * @li @code elt_t* aname_extend1(aname_t** xa) @endcode
 *  Extend xarray @c xa by adding 1 element at the end of the array,
 *  and return a pointer to the newly-added place, or @c NULL on    
 *  failure.
 *
 * @li @code void aname_shorten(aname_t** const xa, const size_t nmemb) @endcode
 *   Shorten @c xa by removing @a nmemb elements from the end of the array.
 *
 * @li @code void aname_shorten1(aname_t** const xa) @endcode
 *   Shorten @c xa by removing one element from the end of the array.
 *
 * @li @code elt_t* aname_insert(aname_t** const za, const size_t pos) @endcode
 * Insert one element in the array at position @c pos and return       
 * pointer to the newly-added place, or @c NULL if some error occurred.  
 * XArray positions follow the usual C convention, ranging from 0 to @c   
 * xa->count-1. Note that the new location is not initialized.
 *
 * @li @code aname_erase(aname_t** const xa, const size_t pos) @endcode
 * Remove the element at position @c pos from the xarray. XArray
 * positions follow the usual C convention, ranging from 0 to @c
 * xa->count-1.
 *
 * @li @code aname_clear(aname_t** const xa) @endcode
 * Forget all the contents and return @c xa to 0 size. The memory
 * allocated to the array is @e not freed.
 *
 * @li @code aname_free(aname_t *xa) @endcode
 * Free array @c xa and return allocated memory to the system.
 *
 */
#define XARRAY_DECLARE(aname, elt_t, extra)                            \
                                                                       \
  typedef struct {                                                     \
    /**  user defined extra info that is fitted into the xarray */     \
    extra                                                              \
    /** number of elements that can be stored in the xarray before     \
     *  realloc() must take place. */                                  \
    size_t allocated;                                                  \
    /** number of elements stored in the xarray */                     \
    size_t count;                                                      \
    /** memory space where xarray elements are stored */               \
    elt_t  storage[];                                                  \
  } aname##_t;                                                         \
                                                                       \
                                                                       \
_inline aname##_t* aname##_alloc(const size_t nmemb) {                 \
  aname##_t* xa = (aname##_t*)xmalloc(sizeof(aname##_t)                \
                                      + sizeof(elt_t)*nmemb);          \
  if (NULL == xa)                                                      \
    return NULL;                                                       \
                                                                       \
  xa->allocated = nmemb;                                               \
  xa->count = 0;                                                       \
                                                                       \
  assert(NULL != xa);                                                  \
  assert(xa->count <= xa->allocated);                                  \
  return xa;                                                           \
};                                                                     \
                                                                       \
                                                                       \
_inline aname##_t* aname##_alloc_placed(void* xa, const size_t size) { \
  assert(NULL != xa);                                                  \
  assert(size > sizeof(aname##_t));                                    \
  if(size < sizeof(aname##_t))                                         \
    return NULL;                                                       \
                                                                       \
  aname##_t* xa_ = (aname##_t*)xa;                                     \
  xa_->allocated = (size - sizeof(aname##_t)) / sizeof(elt_t);         \
  xa_->count = 0;                                                      \
                                                                       \
  assert(xa_->count <= xa_->allocated);                                \
  return xa_;                                                          \
};                                                                     \
                                                                       \
                                                                       \
_inline size_t aname##_size(const aname##_t* const xa) {               \
  assert(NULL != xa);                                                  \
  return xa->count;                                                    \
};                                                                     \
                                                                       \
                                                                       \
_inline void* aname##_lb(const aname##_t* const xa) {                  \
  assert(NULL != xa);                                                  \
  return (void*)xa;                                                    \
};                                                                     \
                                                                       \
                                                                       \
_inline void* aname##_ub(const aname##_t* const xa) {                  \
  assert(NULL != xa);                                                  \
  return (void*) &(xa->storage[xa->count]);                            \
};                                                                     \
                                                                       \
                                                                       \
_inline elt_t* aname##_at(aname##_t* const xa, size_t pos) {           \
  assert(NULL != xa);                                                  \
  assert(pos < xa->count);                                             \
  return &(xa->storage[pos]);                                          \
};                                                                     \
                                                                       \
                                                                       \
_inline void aname##_reserve(aname##_t** xa, const size_t nmemb) {     \
  assert(NULL != xa);                                                  \
  assert(NULL != *xa);                                                 \
  assert((*xa)->count <= (*xa)->allocated);                            \
                                                                       \
  const long more = nmemb - ((*xa)->allocated - (*xa)->count);         \
  if (more > 0) {                                                      \
    /* resize xarray->storage */                                       \
    aname##_t* new_xa = xrealloc((*xa), sizeof(aname##_t) +            \
                                 sizeof(elt_t) * (more + (*xa)->allocated)); \
    if (NULL == new_xa)                                                \
      return;                                                          \
    (*xa) = new_xa;                                                    \
    (*xa)->allocated += more;                                          \
  }                                                                    \
  assert((*xa)->count <= (*xa)->allocated);                            \
};                                                                     \
                                                                       \
                                                                       \
_inline elt_t*  aname##_extend(aname##_t** xa, size_t nmemb) {         \
  assert(NULL != xa);                                                  \
  assert(NULL != *xa);                                                 \
  assert(0 < nmemb);                                                   \
                                                                       \
  aname##_reserve(xa, nmemb);                                          \
  (*xa)->count += nmemb;                                               \
                                                                       \
  assert((*xa)->count <= (*xa)->allocated);                            \
  return aname##_at(*xa, (*xa)->count - nmemb);                        \
};                                                                     \
                                                                       \
                                                                       \
_inline elt_t*  aname##_extend1(aname##_t** xa) {                      \
  return aname##_extend(xa, 1);                                        \
};                                                                     \
                                                                       \
                                                                       \
_inline void aname##_shorten(aname##_t** const xa, const size_t nmemb){\
  assert(NULL != xa);                                                  \
  assert(NULL != *xa);                                                 \
                                                                       \
  (*xa)->count -= nmemb;                                               \
  if((*xa)->count < 0)                                                 \
    (*xa)->count = 0;                                                  \
};                                                                     \
                                                                       \
                                                                       \
_inline void aname##_shorten1(aname##_t** const xa) {                  \
  assert(NULL != xa);                                                  \
  assert(NULL != *xa);                                                 \
                                                                       \
  if((*xa)->count > 0)                                                 \
    (*xa)->count--;                                                    \
};                                                                     \
                                                                       \
                                                                       \
_inline elt_t* aname##_insert(aname##_t** const xa, const size_t pos) {\
  assert(NULL != xa);                                                  \
  assert(NULL != *xa);                                                 \
  assert(pos < (*xa)->count);                                          \
                                                                       \
  elt_t* p = aname##_extend1(xa);                                      \
  if(NULL == p)                                                        \
    return NULL;                                                       \
                                                                       \
  elt_t* src = aname##_at(*xa, pos);                                   \
  elt_t* dst = aname##_at(*xa, pos+1);                                 \
  memmove(dst, src, sizeof(elt_t) * ((*xa)->count - pos));             \
  return src;                                                          \
};                                                                     \
                                                                       \
                                                                       \
_inline void aname##_erase(aname##_t** const xa, const size_t pos) {   \
  assert(NULL != xa);                                                  \
  assert(NULL != *xa);                                                 \
  assert(pos < (*xa)->count);                                          \
                                                                       \
  if (pos == (*xa)->count - 1) {                                       \
    /* remove last element */                                          \
    aname##_shorten1(xa);                                              \
  }                                                                    \
  else {                                                               \
    void* src = aname##_at(*xa, pos+1);                                \
    void* dst = aname##_at(*xa, pos);                                  \
    memmove(dst, src, sizeof(elt_t) * ((*xa)->count - pos - 1));       \
    (*xa)->count -= 1;                                                 \
  }                                                                    \
};                                                                     \
                                                                       \
                                                                       \
_inline void aname##_clear(aname##_t** const xa) {                     \
  assert(NULL != xa);                                                  \
  assert(NULL != *xa);                                                 \
  (*xa)->count = 0;                                                    \
};                                                                     \
                                                                       \
                                                                       \
_inline void aname##_free(aname##_t *xa) {                             \
  free(xa);                                                            \
};                                                                     \


#endif // XARRAY_H
