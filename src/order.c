/* gb: Gröbner Basis
 * Copyright (C) 2018 Christian Eder <ederc@mathematik.uni-kl.de>
 * This file is part of gb.
 * gbla is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 * gbla is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License
 * along with gbla . If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * \file order.c
 * \brief Order procedures
 *
 * \author Christian Eder <ederc@mathematik.uni-kl.de>
 */

#include "data.h"

/* sorting stuff for matrices */
static int columns_cmp(
    const void *a,
    const void *b
    )
{
  const val_t ca  = *((val_t *)a);
  const val_t cb  = *((val_t *)b);

  return (int)(ca - cb);
}

static int matrix_row_initial_input_cmp_drl(
    const void *a,
    const void *b
    )
{
  int32_t i;
  val_t va, vb;

  va  = ((val_t **)a)[0][2];
  vb  = ((val_t **)b)[0][2];

  const exp_t * const ea  = evl + va;
  const exp_t * const eb  = evl + vb;

  /* DRL */
  if (ea[HASH_DEG] < eb[HASH_DEG]) {
    return -1;
  } else {
    if (ea[HASH_DEG] != eb[HASH_DEG]) {
      return 1;
    }
  }

  /* note: reverse lexicographical */
  for (i = nvars-1; i >= 0; --i) {
    if (ea[i] < eb[i]) {
      return -1;
    } else {
      if (ea[i] != eb[i]) {
        return 1;
      }
    }
  }
  return 0;
}

static int matrix_row_cmp(
    const void *a,
    const void *b
    )
{
  val_t va, vb;
  /* compare pivot resp. column index */
  va  = ((val_t **)a)[0][2];
  vb  = ((val_t **)b)[0][2];
  if (va > vb) {
    return 1;
  }
  if (va < vb) {
    return -1;
  }
  /* same column index => compare density of row */
  va  = ((val_t **)a)[0][0];
  vb  = ((val_t **)b)[0][0];
  if (va > vb) {
    return 1;
  }
  if (va < vb) {
    return -1;
  }
  return 0;
}

static inline val_t **sort_matrix_rows(
    val_t **mat)
{
  qsort(mat, (unsigned long)nrows, sizeof(val_t *), &matrix_row_cmp);
  return mat;
}

/* comparison for monomials (in local hash table) */
static int monomial_cmp_pivots_drl(
    const len_t a,
    const len_t b
    )
{
  int32_t i;

  const exp_t * const ea  = evl + a;
  const exp_t * const eb  = evl + b;

  /* first known pivots vs. tail terms */
  if (ea[HASH_IND] < eb[HASH_IND]) {
    return 1;
  } else {
    if (ea[HASH_IND] != eb[HASH_IND]) {
      return -1;
    }
  }

  /* then DRL */
  if (ea[HASH_DEG] > eb[HASH_DEG]) {
    return -1;
  } else {
    if (ea[HASH_DEG] != eb[HASH_DEG]) {
      return 1;
    }
  }

  /* note: reverse lexicographical */
  for (i = nvars-1; i >= 0; --i) {
    if (ea[i] > eb[i]) {
      return 1;
    } else {
      if (ea[i] != eb[i]) {
        return -1;
      }
    }
  }
  return 0;
}

static int monomial_cmp_drl(
    const len_t a,
    const len_t b
    )
{
  int32_t i;

  const exp_t * const ea  = ev + a;
  const exp_t * const eb  = ev + b;

  if (ea[HASH_DEG] > eb[HASH_DEG]) {
    return 1;
  } else {
    if (ea[HASH_DEG] != eb[HASH_DEG]) {
      return -1;
    }
  }

  for (i = nvars-1; i >= 0; --i) {
    if (ea[i] < eb[i]) {
      return 1;
    } else {
      if (ea[i] != eb[i]) {
        return -1;
      }
    }
  }
  return 0;
}

/* comparison for hash-column-maps */
static int hcm_cmp_pivots_drl(
    const void *a,
    const void *b
    )
{
  const len_t ma  = ((len_t *)a)[0];
  const len_t mb  = ((len_t *)b)[0];

  return monomial_cmp_pivots_drl(ma, mb);
}

/* comparison for s-pairs */
static int spair_cmp(
    const void *a,
    const void *b
    )
{
  spair_t *sa = (spair_t *)a;
  spair_t *sb = (spair_t *)b;

  if (sa->lcm != sb->lcm) {
    return (int)monomial_cmp(sa->lcm, sb->lcm);
  } else {
    if (sa->deg != sb->deg) {
      return (sa->deg < sb->deg) ? -1 : 1;
    } else {
      if (sa->gen1 != sb ->gen1) {
        return (sa->gen1 < sb->gen1) ? -1 : 1;
      } else {
        return 0;
      }
    }
  }
}
