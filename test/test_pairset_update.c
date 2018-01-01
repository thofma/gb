#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "../src/gb.c"

int main(
    void
    )
{
  int32_t i;

  const int32_t lens[]  = {2,2,2}; 
  const int32_t cfs[]   = {1, 1, 1, 3, 5, 3};
  const int32_t exps[]  = {1, 0, 0, 1, 3, 0, 0, 1, 1, 1, 1, 0};

  const int32_t nr_vars     = 2;
  const int32_t nr_gens     = 3;
  const int32_t ht_size     = 12;
  const int32_t field_char  = 101;

  val_t **mat;
  
  /* initialize stuff */
  initialize_basis(nr_gens);
  initialize_pairset();
  initialize_global_hash_table(nr_vars, ht_size, field_char);
  initialize_local_hash_table(ht_size);

  mat = import_julia_data(lens, cfs, exps, nr_gens);
  calculate_divmask();

  for (i = 0; i < nrows; ++i) {
    normalize_matrix_row(mat[i]);
  }

  update_basis(mat);

  if (pload != 2) {
    return 1;
  }

  if (ps[0].gen1 != 0) {
    return 1;
  }
  if (ps[0].gen2 != 2) {
    return 1;
  }
  if (ps[0].deg != 2) {
    return 1;
  }

  if (ps[1].gen1 != 1) {
    return 1;
  }
  if (ps[1].gen2 != 2) {
    return 1;
  }
  if (ps[1].deg != 3) {
    return 1;
  }
  
  /* free and clean up */
  free_local_hash_table();
  free_global_hash_table();
  free_pairset();
  free(mat);
  free_basis();
  return 0;
}
