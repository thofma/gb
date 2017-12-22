/* gb: Gröbner Basis
 * Copyright (C) 2015 Christian Eder <ederc@mathematik.uni-kl.de>
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
 * \file basis.c
 * \brief General and global data
 *
 * \author Christian Eder <ederc@mathematik.uni-kl.de>
 */

#include "data.h"

static void initialize_basis(
    int32_t ngens
    )
{
  bload = 0;
  bsize = 2*ngens;

  bs  = malloc((unsigned long)bsize * sizeof(poly_t));
}

static void free_basis(
    void
    )
{
  int32_t i;
  if (bs) {
    for (i = 0; i < bload; ++i) {
      free(bs[i]);
    }
    free(bs);
    bload = 0;
    bsize = 0;
  }
}