/**
 * @file   test-xarray.c
 *
 * Test cases for module @c xarray
 *
 * @author  riccardo.murri@gmail.com
 * @version 1.0
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

#include "xarray.h"

#include <stdio.h>
#include <assert.h>

typedef struct { int coord; double val; } entry_t;
XARRAY_DECLARE(row, entry_t, int a; float b; char c;)


/* check that `r` is an array of (n,n+1) pairs, sorted in ascending order. */
void check_row(row_t* r)
{
  int n = 0;
  for (n = 0; n < row_size(r); ++n) {
    assert(n == r->storage[n].coord);
    assert(n+1 == r->storage[n].val);
    assert(n == row_at(r, n)->coord);
    assert(n+1 == row_at(r, n)->val);
  };
};


int main(int argc, char** argv) 
{
  int n = 0;

  /* create new row */
  row_t* r = row_alloc(2);
  assert(row_size(r) == 0);
  assert(r->allocated == 2);

  /* add one element */
  entry_t* p = row_extend1(&r);
  assert(row_size(r) == 1);
  p->coord = 0;
  p->val = 1;
  /* check it back */
  p = row_at(r, 0);
  assert(0 == p->coord);
  assert(1.0 == p->val);

  /* request more storage space */
  row_reserve(&r, 3);
  assert(row_size(r) == 1);
  assert(r->allocated >= 4);

  /* add more items one at a time */
  for (n = 1; n < 10; ++n) {
    p = row_extend1(&r);
    assert(row_size(r) == n+1);
    p->coord = n;
    p->val = n+1;
  };
  check_row(r);

  row_shorten1(&r);
  assert(row_size(r) == 9);
  check_row(r);

  row_shorten(&r, 4);
  assert(row_size(r) == 5);
  check_row(r);

  row_clear(&r);
  assert(row_size(r) == 0);

  /* now try adding back 5 elements at a time */
  p = row_extend(&r, 5);
  assert(row_size(r) == 5);
  for (n = 0; n < 5; ++n) {
    p->coord = n;
    p->val = n+1;
    ++p;
  };
  check_row(r);

  /* replace them and then insert some more */
  p = r->storage;
  for (n = 0; n < 5; ++n) {
    p->coord = 2*n;
    p->val = 2*n+1;
    ++p;
  };
  for (n = 3; n >= 0; --n) {
    p = row_insert(&r, n+1);
    p->coord = 2*n+1;
    p->val = 2*n+2;
  }
  assert(row_size(r) == 9);
  check_row(r);

  /* erase elements in even position */
  for (n = 4; n >= 0; --n)
    row_erase(&r, 2*n);
  assert(row_size(r) == 4);
  for (n = 0; n < 4; ++n) {
    p = row_at(r, n);
    assert(2*n+1 == p->coord);
    assert(2*n+2 == p->val);
  }

  /* finally, free it */
  row_free(r);


  /* now, do the same checks, but allocated the buffer separately with malloc */
  void* buf = malloc(256);
  assert(NULL != buf);
  r = row_alloc_placed(buf, 256);
  assert(row_size(r) == 0);
  assert(r->allocated >= 0);

  /* add one element */
  p = row_extend1(&r);
  assert(row_size(r) == 1);
  p->coord = 0;
  p->val = 1;
  /* check it back */
  p = row_at(r, 0);
  assert(0 == p->coord);
  assert(1.0 == p->val);

  /* request more storage space */
  row_reserve(&r, 3);
  assert(row_size(r) == 1);
  assert(r->allocated >= 4);

  /* add more items one at a time */
  for (n = 1; n < 10; ++n) {
    p = row_extend1(&r);
    assert(row_size(r) == n+1);
    p->coord = n;
    p->val = n+1;
  };
  check_row(r);

  row_shorten1(&r);
  assert(row_size(r) == 9);
  check_row(r);

  row_shorten(&r, 4);
  assert(row_size(r) == 5);
  check_row(r);

  row_clear(&r);
  assert(row_size(r) == 0);

  /* now try adding back 5 elements at a time */
  p = row_extend(&r, 5);
  assert(row_size(r) == 5);
  for (n = 0; n < 5; ++n) {
    p->coord = n;
    p->val = n+1;
    ++p;
  };
  check_row(r);

  /* replace them and then insert some more */
  p = r->storage;
  for (n = 0; n < 5; ++n) {
    p->coord = 2*n;
    p->val = 2*n+1;
    ++p;
  };
  for (n = 3; n >= 0; --n) {
    p = row_insert(&r, n+1);
    p->coord = 2*n+1;
    p->val = 2*n+2;
  }
  assert(row_size(r) == 9);
  check_row(r);

  /* erase elements in even position */
  for (n = 4; n >= 0; --n)
    row_erase(&r, 2*n);
  assert(row_size(r) == 4);
  for (n = 0; n < 4; ++n) {
    p = row_at(r, n);
    assert(2*n+1 == p->coord);
    assert(2*n+2 == p->val);
  }

  /* finally, free it */
  row_free(r);

  return 0;
}
