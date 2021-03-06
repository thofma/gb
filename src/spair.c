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
 * \file spair.c
 * \brief Implementation of handling of pair sets.
 *
 * \author Christian Eder <ederc@mathematik.uni-kl.de>
 */
#include "spair.h"
double t_sort_spairs        = 0;
double t_gm_1        = 0;
double t_gm_2        = 0;
double t_gm_3        = 0;
double t_gm_remove        = 0;
struct timeval t_start;

inline ps_t *initialize_pair_set(const gb_t *basis)
{
  ps_t *ps  = (ps_t *)malloc(sizeof(ps_t));
  ps->size  = 2 * basis->size;
  ps->pairs = (spair_t *)malloc(ps->size * sizeof(spair_t));
  ps->load  = 0;

  /* enter input elements as special spairs */
  enter_input_elements_to_pair_set(ps, basis);

  return ps;
}

void enter_input_elements_to_pair_set(ps_t *ps, const gb_t *basis)
{
  nelts_t i;

  for (i=1; i<basis->st; ++i) {
    generate_input_element_spair(ps, i, basis, ht);
  }
}

inline void update_pair_set_many(ps_t *ps, const gb_t *basis, const nelts_t fidx)
{
  nelts_t i;

  nelts_t idx = fidx;
  /* maximal number of pairs to be added */
  nelts_t mnp = 0;
  for (nelts_t j = fidx-1; j < basis->load-1; ++j)
    mnp +=  j;
  /* we get maximal mnp new pairs */
  if (ps->size <= ps->load + mnp) {
    enlarge_pair_set(ps, ps->size + mnp);
  }
  while (idx < basis->load) {
    /* generate spairs with the initial elements in basis
     * See note on gb_t in src/types.h why we start at position 1 here. */

    for (i=basis->st; i<idx; ++i) {
      generate_spair(ps, idx, i, basis, ht);
#if SPAIR_DEBUG
      printf("pair %u, %u + %u | %u\n",idx,i,ps->load,ps->pairs[ps->load+i-basis->st]->deg);
#endif
    }
    /* we do not update ps->load at the moment in order to be able to distinguish
     * old and new pairs for the gebauer-moeller update following */

    /* check product and chain criterion in gebauer moeller style
     * note that we have already marked the pairs for which the product criterion
     * applies in generate_spair() */
    if (idx > basis->st)
      gebauer_moeller(ps, basis, idx);

    /* fix pair set and remove detected pairs */
    meta_data->ncrit_last   =   remove_detected_pairs(ps, idx-basis->st);
    meta_data->ncrit_total  +=  meta_data->ncrit_last;

    ++idx;
  }
}


#if 0
void gebauer_moeller(ps_t *ps, const gb_t *basis, const nelts_t idx)
{
  nelts_t pos1, pos2;
  /* current length can be computed already, need to adjust by the starting
   * position in basis */
  const int cur_len = (int)(ps->load + idx - basis->st);
  /* printf("curlen %u for idx %u | %u\n",cur_len,idx, idx-basis->st); */
  const hash_t hash     = basis->p[idx][2];
  int i, j; /* we need ints to cover cases where i=0 and j=i-1 */
  const int load  = (int)ps->load;

  /* printf("idx %u | psl %u | cl %u\n", idx, ps->load, cur_len); */
  /* first step: remove elements already in ps due to chain criterion with new
   * pairs in new_pairs */
  /* gettimeofday(&t_start, NULL); */
  for (i=0; i<load; ++i) {
#if 0
    if ((basis->red[ps->pairs[i]->gen1] != 0 && basis->red[ps->pairs[i]->gen1] != ps->pairs[i]->gen2)
       )
#else
    if ((basis->red[ps->pairs[i].gen2] != 0) ||
        (basis->red[ps->pairs[i].gen1] != 0 && basis->red[ps->pairs[i].gen1] != ps->pairs[i].gen2)
       )
#endif
      ps->pairs[i].crit  = CHAIN_CRIT;
  }
  for (i=0; i<load; ++i) {
    /* do not check on initial spairs */
    if (ps->pairs[i].crit == NO_CRIT) {
    /* if (ps->pairs[i]->crit == NO_CRIT && ps->pairs[i]->gen1 != ps->pairs[i]->gen2) { */
      /* See note on gb_t in src/types.h why we adjust position by -basis->st. */
      pos1  = ps->pairs[i].gen1 - basis->st;
      pos2  = ps->pairs[i].gen2 - basis->st;
      if (check_monomial_division(ps->pairs[i].lcm, hash, ht) != 0 &&
          ps->pairs[i].lcm != ps->pairs[ps->load+pos1].lcm &&
          ps->pairs[i].lcm != ps->pairs[ps->load+pos2].lcm
          ) {
        ps->pairs[i].crit  = CHAIN_CRIT;
#if SPAIR_DEBUG
        printf("CC for (%u,%u)\n",pos1+1, pos2+1);
#endif
      }
    }
  }
  /* t_gm_1 +=  walltime(t_start); */

  /* next: sort new pairs */
  /* printf("ps->load %u | idx %u | basis->st %u | idx-basis->st %u | ps->size %u\n",
   *     ps->load, idx, basis->st, idx-basis->st, ps->size);
   * for (size_t ii=0; ii < ps->size; ++ii)
   *   printf("%u | %u -- %u\n", ii, ps->pairs[ii].gen1, ps->pairs[ii].gen2); */
  /* gettimeofday(&t_start, NULL); */
  qsort(ps->pairs+ps->load, idx-basis->st, sizeof(spair_t), ht->sort.compare_spairs);
  /* t_sort_spairs +=  walltime(t_start); */

  /* second step: remove new pairs by themselves w.r.t the chain criterion */
  /* gettimeofday(&t_start, NULL); */
  for (i=load; i<cur_len; ++i) {
    if (ps->pairs[i].crit != NO_CRIT)
      continue;
    for (j=load; j<i; ++j) {
      /* if (ps->pairs[j].crit != NO_CRIT) [> smaller lcm eliminated j <]
       *   continue; */
      /* if (ps->pairs[i]->lcm == ps->pairs[j]->lcm) { */
      if (check_monomial_division(ps->pairs[i].lcm, ps->pairs[j].lcm, ht) != 0 &&
          ps->pairs[i].lcm != ps->pairs[j].lcm
) {
      /* if (ps->pairs[i]->lcm != ps->pairs[j]->lcm &&
       *     check_monomial_division(ps->pairs[i]->lcm, ps->pairs[j]->lcm, ht) != 0) { */
        ps->pairs[i].crit  = CHAIN_CRIT;
#if SPAIR_DEBUG
        printf("2CC for (%u,%u) by (%u,%u)\n",ps->pairs[i].gen1, ps->pairs[i].gen2,
            ps->pairs[j].gen1, ps->pairs[j].gen2);
#endif
        break;
      }
    }
  }
  /* t_gm_2 +=  walltime(t_start); */

  /* gettimeofday(&t_start, NULL); */
  for (i=(int)ps->load; i<cur_len; ++i) {
    switch (ps->pairs[i].crit) {
      case CHAIN_CRIT:
        continue;
      case PROD_CRIT:
        for (j=(int)ps->load; j<cur_len; ++j) {
          if (ps->pairs[j].lcm == ps->pairs[i].lcm) {
            ps->pairs[j].crit  = CHAIN_CRIT;
          }
        }
        continue;
      case NO_CRIT:
        for (j=i-1; j>(int)(ps->load-1); --j) {
          if (ps->pairs[j].lcm == ps->pairs[i].lcm) {
            ps->pairs[i].crit  = CHAIN_CRIT;
            break;
          }
        }
        continue;
      default:
        break;
    }
  }
  /* t_gm_3 +=  walltime(t_start);
   * printf("%9.3f sec | %9.3f sec | %9.3f sec | %9.3f sec || %9.3f sec | \n",
   *     t_gm_1 / (1000000), t_gm_2 / (1000000), t_gm_3 / (1000000),
   *     t_sort_spairs / (1000000), (t_gm_1+t_gm_2+t_gm_3+t_sort_spairs) / (1000000)); */
}
#else
void gebauer_moeller(ps_t *ps, const gb_t *basis, const nelts_t idx)
{
  nelts_t pos1, pos2;
  /* current length can be computed already, need to adjust by the starting
   * position in basis */
  int cur_len = (int)(ps->load + idx - basis->st);
  /* printf("curlen %u for idx %u | %u\n",cur_len,idx, idx-basis->st); */
  const hash_t hash     = basis->p[idx][2];
  int i, j; /* we need ints to cover cases where i=0 and j=i-1 */
  const int load  = (int)ps->load;

  spair_t *sp = ps->pairs;

  /* gettimeofday(&t_start, NULL); */
  for (i=0; i<load; ++i) {
    /* do not check on initial spairs */
    /* See note on gb_t in src/types.h why we adjust position by -basis->st. */
    if (sp[i].gen1 > basis->st) {
      pos1  = sp[i].gen1 - basis->st;
      pos2  = sp[i].gen2 - basis->st;
      if (check_monomial_division(sp[i].lcm, hash, ht) != 0 &&
          sp[i].lcm != sp[ps->load+pos1].lcm &&
          sp[i].lcm != sp[ps->load+pos2].lcm) {
        sp[i].crit  = CHAIN_CRIT;
      }
    }
    /* if (sp[i].lcm != sp[ps->load+pos1].lcm &&
     *     sp[i].lcm != sp[ps->load+pos2].lcm &&
     *     check_monomial_division(sp[i].lcm, hash, ht) != 0) {
     *   sp[i].crit  = CHAIN_CRIT;
     * } */
    /* if (check_monomial_division(sp[i].lcm, hash, ht) != 0 &&
     *     sp[i].lcm != get_lcm(hash, basis->eh[sp[i].gen1][0], ht) &&
     *     sp[i].lcm != get_lcm(hash, basis->eh[sp[i].gen2][0], ht)) {
     *   sp[i].crit  = CHAIN_CRIT;
     * } */
  }
  /* t_gm_1 +=  walltime(t_start); */

  /* for each lcm the corresponding pairs are now sorted in the following order:
   * PROD_CRIT < NO_CRIT < CHAIN_CRIT */
  /* next: sort new pairs */
  /* gettimeofday(&t_start, NULL); */
  qsort(sp+ps->load, idx-basis->st, sizeof(spair_t), ht->sort.compare_spairs);
  /* t_sort_spairs +=  walltime(t_start); */


  /* second step: remove new pairs by themselves w.r.t the chain criterion */

  /* gettimeofday(&t_start, NULL); */
  int next;
  for (j = load; j < cur_len; ++j) {
    next  = j;
    if (sp[j].crit != CHAIN_CRIT) {
      i = j+1;
      while (i < cur_len && sp[i].lcm == sp[j].lcm)
        ++i;
      next  = i-1;
      while (i < cur_len) {
        /* if (check_monomial_division(sp[i].lcm, sp[j].lcm, ht) != 0) { */
        if (sp[i].crit == NO_CRIT &&
            check_monomial_division(sp[i].lcm, sp[j].lcm, ht) != 0) {
          sp[i].crit  = CHAIN_CRIT;
        }
        ++i;
      }
    }
    j = next;
  }
  /* t_gm_2 +=  walltime(t_start); */

  /* nelts_t new_len = remove_chain_crit_pairs(ps, cur_len);
   * [> fix pair set and remove detected pairs <]
   * meta_data->ncrit_last   =   cur_len - (int)new_len;
   * meta_data->ncrit_total  +=  meta_data->ncrit_last;
   * [> printf("old cur_len %d | new cur_len %u\n", cur_len, new_len); <]
   * cur_len = (int)new_len; */

  /* gettimeofday(&t_start, NULL); */
  /* third step */
  for (i = load; i < cur_len; ++i) {
    if (sp[i].crit == CHAIN_CRIT) {
      continue;
    }
    j = i+1;
    while(j < cur_len && sp[j].lcm == sp[i].lcm) {
      sp[j].crit = CHAIN_CRIT;
      j++;
    }
    i = j-1;
  }
  /* t_gm_3 +=  walltime(t_start); */
  /* NOTE: Gebauer-Moeller usually has a third step removing all same-lcm pairs.
   *       We do not need to do this step here since the pairs part is already
   *       sorted in a way such that for the same lcm the pairs have the
   *       following order: PROD_CRIT < NO_CRIT < CHAIN_CRIT.
   *       Thus for each new lcm we find we only need to check if the first
   *       element has NO_CRIT. This element is then added, all other of the
   *       same lcm are deleted. This we do in the remove_detected_pairs()
   *       function directly. */


  /* fix pair set and remove detected pairs */
  /* gettimeofday(&t_start, NULL); */
  meta_data->ncrit_last   =   remove_detected_pairs(ps, idx-basis->st);
  meta_data->ncrit_total  +=  meta_data->ncrit_last;
  /* t_gm_remove +=  walltime(t_start); */

  /* printf("%9.3f sec | %9.3f sec | %9.3f sec | %9.3f sec | %9.3f sec || %9.3f sec | \n",
   *     t_gm_1 / (1000000), t_gm_2 / (1000000), t_gm_3 / (1000000), t_gm_remove / (1000000),
   *     t_sort_spairs / (1000000), (t_gm_1+t_gm_2+t_gm_3+t_gm_remove+t_sort_spairs) / (1000000)); */
}
#endif

inline nelts_t remove_detected_pairs(ps_t *ps, const nelts_t ctr)
{
  /* current length can be computed already, need to adjust by the starting
   * position in basis */
  const nelts_t cur_len = ps->load + ctr;
  nelts_t i, j, nremoved;

  j         = 0;
  nremoved  = 0;
  for (i=0; i<cur_len; ++i) {
    if (ps->pairs[i].crit != NO_CRIT) {
#if SPAIR_DEBUG
      printf("REMOVED (%u,%u)\n",ps->pairs[i]->gen1, ps->pairs[i]->gen2);
#endif
      nremoved++;
      /* printf("%p %u\n", ps->pairs[i], i); */
      /* free(ps->pairs[i]);
       * ps->pairs[i]  = NULL; */
      continue;
    }
    ps->pairs[j++]  = ps->pairs[i];
  }
  ps->load  = j;

  return nremoved;
}


inline void enlarge_pair_set(ps_t *ps, const nelts_t new_size)
{
  ps->pairs = (spair_t *)realloc(ps->pairs, new_size * sizeof(spair_t));
  ps->size  = new_size;
}

inline void generate_input_element_spair(ps_t *ps, const nelts_t gen2, const gb_t *basis, ht_t *ht)
{
  /* spair_t *sp = (spair_t *)malloc(sizeof(spair_t)); */
  /* sp->gen1  = 0; */
  spair_t *sp = ps->pairs + ps->load;
  
  sp->gen1    = gen2;
  sp->gen2    = gen2;
  sp->lcm     = basis->p[gen2][2];
  sp->deg     = ht->deg[sp->lcm];
  sp->crit    = NO_CRIT;

  ps->load++;
}
