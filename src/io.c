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
 * \file io.c
 * \brief Input and output handling
 *
 * \author Christian Eder <ederc@mathematik.uni-kl.de>
 */

#include "data.h"


/* note that depending on the input data we set the corresponding
 * function pointers for monomial resp. spair comparisons, taking
 * spairs by a given minimal property for symbolic preprocessing, etc. */
static val_t **import_julia_data(
    const int32_t *lens,
    const int32_t *cfs,
    const int32_t *exps,
    const int32_t nr_gens
    )
{
  int32_t i, j;
  int32_t off = 0; /* offset in arrays */
  val_t **mat = malloc((unsigned long)nr_gens * sizeof(val_t *));
  
  for (i = 0; i < nr_gens; ++i) {
    /* each matrix row has the following structure:
     * [length | offset | eh1 | cf1 | eh2 | cf2 | .. | ehl | cfl]
     * where piv? is a label for being a known or an unknown pivot */
    mat[i]    = malloc((2*(unsigned long)lens[i]+2) * sizeof(val_t));
    mat[i][0] = 2*lens[i]+2;
    mat[i][1] = 2*(lens[i] % UNROLL)+2; /* offset for starting loop unrolling */
    for (j = off; j < off+lens[i]; ++j) {
      mat[i][2*(j+1-off)]   = insert_in_local_hash_table(exps+(nvars*j));
      mat[i][2*(j+1-off)+1] = cfs[j];
    }
    /* mark initial generators, they have to be added to the basis first */
    off +=  lens[i];
  }
  npivs = nrows = nrall = nr_gens;

  /* todo: this needs to be generalized for different monomial orders */
  monomial_cmp  = monomial_cmp_drl;
  hcm_cmp       = hcm_cmp_pivots_drl;
  select_spairs = select_spairs_by_minimal_degree;

  /* todo: linear algebra routines, depending on bit size of prime */
  if (fc < pow(2, 16)) {
    reduce_dense_row_by_known_pivots = reduce_dense_row_by_known_pivots_16_bit;
  } else {
    /* TODO: 32 bit implementation */
    reduce_dense_row_by_known_pivots = reduce_dense_row_by_known_pivots_16_bit;
  }

  return mat;
}

static int32_t *export_julia_data(
    void
    )
{
  int32_t i, j, k, ctr;

  uint64_t len  = 0; /* complete length of exported array */
  uint64_t nb   = 0; /* # elemnts in basis */

  const int32_t lterm = 1 + nvars; /* length of a term */

  /* compute number of terms */
  for (i = 0; i < bload; ++i) {
    if ((long)bs[i] & bred) {
      continue;
    } else {
      len +=  (bs[i][0]-2)/2;
      nb++;
    }
  }

  /* compute the length considering the number of variables per exponent */
  len = len * lterm;
  /* add storage for length of each element */
  len = len + nb;
  /* add storage for length of complete array */
  len++;

  int32_t *basis  = (int32_t *)malloc(len * sizeof(int32_t));

  if (len > (uint64_t)(pow(2, 31))) {
    printf("basis too big\n");
    return NULL;
  }

  ctr = 0;

  basis[ctr++]  = (int32_t)len;
  for (i = 0; i < bload; ++i) {
    if ((long)bs[i] & bred) {
      continue;
    } else {
      /* length of polynomial including this length entry itself */
      basis[ctr++]  = (bs[i][0]-2)/2 * lterm + 1;
      for (j = 2; j < bs[i][0]; j += 2) {
        basis[ctr++]  = bs[i][j+1]; /* coefficient */
        for (k = 0; k < nvars; ++k) {
          basis[ctr++]  = (ev + bs[i][j])[k];
        }
      }
    }
  }

  for (i = 0; i < len; ++i) {
    printf("%d ", basis[i]);
  }
  printf("\n");

  return basis;
}
