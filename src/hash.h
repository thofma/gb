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
 * \file hash.h
 * \brief Implementation of the hash tables used in gb.
 *
 * \author Christian Eder <ederc@mathematik.uni-kl.de>
 */
#ifndef GB_HASH_H
#define GB_HASH_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#if defined(_MSC_VER)
     /* Microsoft C/C++-compatible compiler */
     #include <intrin.h>
#elif defined(__GNUC__) && (defined(__x86_64__) || defined(__i386__))
     /* GCC-compatible compiler, targeting x86/x86-64 */
     #include <x86intrin.h>
#elif defined(__GNUC__) && defined(__ARM_NEON__)
     /* GCC-compatible compiler, targeting ARM with NEON */
     #include <arm_neon.h>
#elif defined(__GNUC__) && defined(__IWMMXT__)
     /* GCC-compatible compiler, targeting ARM with WMMX */
     #include <mmintrin.h>
#elif (defined(__GNUC__) || defined(__xlC__)) && (defined(__VEC__) || defined(__ALTIVEC__))
     /* XLC or GCC-compatible compiler, targeting PowerPC with VMX/VSX */
     #include <altivec.h>
#elif defined(__GNUC__) && defined(__SPE__)
     /* GCC-compatible compiler, targeting PowerPC with SPE */
     #include <spe.h>
#endif
#if defined(_OPENMP)
#include <omp.h>
#endif
#include <config.h>
#include "types.h"

#ifndef META_DATA_DEBUG
#define META_DATA_DEBUG 0
#endif

#ifndef HASH_DEBUG
#define HASH_DEBUG  0
#endif

#ifndef HASH_QUADRATIC_PROBING
#define HASH_QUADRATIC_PROBING  0
#endif

#ifndef HASH_CHECK
#define HASH_CHECK 0
#endif

#ifndef COUNT_DIV_HITS
#define COUNT_DIV_HITS 0
#endif

#ifndef DIVMAP_RECALCULATE_COUNTER
#define DIVMAP_RECALCULATE_COUNTER  100000000
#endif


/***************************
 * OUR HASH TABLE IS GLOBAL
 **************************/
extern ht_t *ht;
extern hash_t *lms;

/* global variables used as random seeds, initialized to max unsigned values
 * depending on available wordsize of the machine */
/**
 * \brief Generates pseudo random numbers using xorshifts using global defined
 * random_seed variable
 *
 * \return some pseudo random number
 */
static inline hash_t pseudo_random_generator(hash_t random_seed)
/* static inline uint32_t pseudo_random_generator(uint32_t random_seed) */
{
/*   random_seed ^=  (random_seed << 13);
 *   random_seed ^=  (random_seed << 17);
 *   random_seed ^=  (random_seed << 5);
 *
 *   return random_seed; */
  /* return (uint32_t) ((1103515245 * ((uint64_t)random_seed) + 12345) & 0x7fffffffUL); */
  return (hash_t) ((110351523 * ((uint64_t)random_seed) + 54321) & 0x7fffffffUL);
  /* return random_seed; */
  /* random_seed = 36969*(random_seed & 65535) * (random_seed >> 16);
   * random_seed = 18000*(random_seed & 65535) ^ (random_seed >> 16);
   * return (random_seed << 16) * random_seed; */
}

/********************************************************************************
 * FOLLOWING HASH TABLE IMPLEMENTATION IS COPIED FROM COMPACT F4 IMPLEMENTATION 
 * BY MONAGAN AND PIERCE (see PASCO 2015)
 *******************************************************************************/
/**
 * \brief Inserts random seeds in hash table random array
 *
 * \param hash table ht
 */
static inline void set_random_seed(ht_t *ht)
{
  hash_t i;

  uint32_t random_seed  = 2463534242;
/* uint32_t random_seed  = 0xFFFFFFFF; */
  /* use random_seed, no zero values are allowed */
  for (i=0; i<ht->nv; ++i) {
    random_seed = pseudo_random_generator(random_seed);
    ht->rand[i] = random_seed | 1;
  }
}

/**
 * \brief Generates divmask for given exponent exp.
 *
 * \param exponent vector exp
 *
 * \param hash table ht
 *
 * \return divmask of exp
 */
#if 1
static inline divm_t generate_divmask(const exp_t *exp, ht_t *ht)
{
  /* for (nelts_t i=0; i<32; ++i)
   *   printf("%2u|%2u ", i, ht->divmap[i]);
   * printf("\n"); */
  /* for (nelts_t i=0; i<ht->nv; ++i)
   *   printf("%u ", exp[i]); */
  nelts_t ctr = 0;
  divm_t dm = 0;
  for (nelts_t i=0; i<ht->ndv; ++i) {
    /* printf("i %u\n", i); */
    for (nelts_t j=0; j<ht->bpv; ++j) {
      if (exp[i] >= ht->divmap[ctr]) {
        /* printf("i %u, j %u, ctr %u, ht->ndv %u, ht->bpv %u\n", i, j, ctr, ht->ndv, ht->bpv); */
        dm |= 1 << ctr;
      }
      ctr++;
    }
  }
  /* printf("--> %d\n", dm); */
  return dm;
}

/**
 * \brief Recalculates the divmaps for each variable and updated the divmasks
 * correspondingly. ivmaps depend on the average of max and min exponent
 * appearing in the hash table for a variable index i.
 *
 * \param hash table ht
 */
static inline void recalculate_divmaps(ht_t *ht)
{
  /* printf("recalculate: ndv %u | bpv %u \n",ht->ndv, ht->bpv);
   * for (nelts_t i=0; i<32; ++i)
   *   printf("%2u|%2u ", i, ht->divmap[i]);
   * printf("\n"); */
  deg_t *max_exponents = (deg_t *)malloc(ht->ndv * sizeof(deg_t));
  deg_t *min_exponents = (deg_t *)malloc(ht->ndv * sizeof(deg_t));
  
  exp_t *ev = ht->exp + (ht->nv * 1);

  /* get initial values for min and max from first hash table entry */
  for (nelts_t j=0; j<ht->ndv; ++j)
    max_exponents[j]  = min_exponents[j]  = ev[j]; 
  /* printf("max and min initial\n");
   * for (nelts_t i=0; i<ht->ndv; ++i)
   *   printf("%3u ", max_exponents[i]);
   * printf("\n");
   * for (nelts_t i=0; i<ht->ndv; ++i)
   *   printf("%3u ", min_exponents[i]);
   * printf("\n"); */
  /* now calculate min and max over the full hash table */
  for (nelts_t i=2; i<ht->load; ++i) {
    ev  = ht->exp + (ht->nv * i);
    for (nelts_t j=0; j<ht->ndv; ++j) {
      if (ev[j] > max_exponents[j])
        max_exponents[j]  = ev[j];
      if (ev[j] < min_exponents[j])
        min_exponents[j]  = ev[j];
    }
  }
  /* printf("max and min\n");
   * for (nelts_t i=0; i<ht->ndv; ++i)
   *   printf("%3u ", max_exponents[i]);
   * printf("\n");
   * for (nelts_t i=0; i<ht->ndv; ++i)
   *   printf("%3u ", min_exponents[i]);
   * printf("\n"); */
  /* reset divmap values */
  nelts_t ctr = 0;
  for (nelts_t i=0; i<ht->ndv; ++i) {
    /* take average and increment it over the bits per variable area */
    deg_t inc = (max_exponents[i] - min_exponents[i]) / ht->bpv;
    if (inc == 0)
      inc = 1;
    for (nelts_t j=0; j<ht->bpv; ++j) {
      ht->divmap[ctr]   =   min_exponents[i];
      min_exponents[i]  +=  inc;
      ctr++;
    }
  }
  /* printf("recalculated divmap\n");
   * for (nelts_t i=0; i<CHAR_BIT * sizeof(divm_t); ++i)
   *   printf("%u | %u\n", i, ht->divmap[i]); */
  /* recalculate divmask entries for all elements in hash table */
  for (nelts_t i=1; i<ht->load; ++i) {
    ht->dm[i] = generate_divmask(ht->exp + (ht->nv * i), ht);
  }

  /* reset recalculate counter for divmaps */
  ht->rcdm  = ht->muldm * ht->nv * DIVMAP_RECALCULATE_COUNTER;
  ht->muldm++;
  
  free(max_exponents);
  free(min_exponents);
}
#endif
/**
 * \brief Generates hash table as defined in compact F4 implementation by
 * Monagan and Pearce (see PASCO 2015)
 *
 * \param hash table size index ht_si
 *
 * \param number of variables in given polynomial ring nv
 *
 * \return hash table
 */
static inline ht_t *init_hash_table(const ht_size_t ht_si,
    const nvars_t nv)
{
  hash_t i;

  ht_t *ht = (ht_t *)malloc(sizeof(ht_t));

  /* global table data */
  /* ht->sz   = (ht_size_t)8388607; */
  /* ht->sz    = (ht_size_t)524287; */
  ht->sz    = (ht_size_t)(pow(2,ht_si));
  /* we add one extra variable in case we have to homogenize the system */
  ht->nv    = nv;
  if (ht->nv % 2 == 0)
    ht->offset =  0;
  else
    ht->offset =  1;
  /* for easier divisibility checks we start at index 1. If the divisibility
   * check routines return 0, there is no division. */
  ht->load    = 1;
  ht->load_ls = 1;
  ht->lut     = (ht_size_t *)calloc(ht->sz, sizeof(ht_size_t));
  ht->val     = (hash_t *)calloc(ht->sz, sizeof(hash_t));
  ht->deg     = (deg_t *)calloc(ht->sz, sizeof(deg_t));
  ht->div     = (nelts_t *)calloc(ht->sz, sizeof(nelts_t));
  ht->idx     = (ht_size_t *)calloc(ht->sz, sizeof(ht_size_t));
#if 1
  ht->dm      = (divm_t *)calloc(ht->sz, sizeof(divm_t));
  /* the divmap has to work on exactly 32 entries independently of the
   * number of variables: initially all exponents are set to zero, after reading
   * in all the input elements we recalculate the possible exponents for the
   * divmap for the first time */
  ht->divmap  = (deg_t *)calloc(CHAR_BIT * sizeof(divm_t), sizeof(deg_t)); 
  ht->bpv     = (CHAR_BIT * sizeof(divm_t)) / ht->nv;
  if (ht->bpv == 0)
    ht->bpv = 1;
  ht->ndv     = ht->nv < (CHAR_BIT * sizeof(divm_t)) ?
    ht->nv : (CHAR_BIT * sizeof(divm_t));
  /* ht->rcdm    = DIVMAP_RECALCULATE_COUNTER; */
  ht->rcdm    = DIVMAP_RECALCULATE_COUNTER;
  ht->muldm   = 1;
#endif
#if HASH_CHECK
  ht->ctr     = (ht_size_t *)calloc(ht->sz, sizeof(ht_size_t));
#endif
  ht->rand    = (hash_t *)malloc(ht->nv * sizeof(hash_t));
  ht->exp     = (exp_t *)calloc(ht->sz * ht->nv, sizeof(exp_t));
  /* get memory for each exponent */
  /* for (i=0; i<ht->sz; ++i) {
   *   ht->exp[i]  = (exp_t *)calloc(ht->nv, sizeof(exp_t));
   * } */
  /* use random_seed, no zero values are allowed */
  set_random_seed(ht);

  return ht;
}

/**
 * \brief Inserts in hash table using quadratic probing when enlarging table
 *
 * \param hash value to be inserted hash
 *
 * \param hash table hash_table
 */
static inline void insert_while_enlarging(const hash_t hash, const ht_size_t pos, ht_t *ht)
{
  ht_size_t tmp_h = 0;

  for (size_t i=0; i<ht->sz; ++i) {
    tmp_h = (ht_size_t)(hash+i) & (ht->sz-1);
    if (ht->lut[tmp_h] == 0)
      break;
#if HASH_DEBUG
      for (int i=0; i<ht->nv; ++i)
        printf("%u ",ht->exp{(ht->nv * pos) + i]);
      printf(" ||| ");
      printf("%11u | %11u | %5u\n",hash, pos, ht->deg[pos]);
#endif
  }
  ht->lut[tmp_h]  = pos;
}

/**
 * \brief Enlarges hash table to next prime number size
 *
 * \param hash table ht
 */
static inline void enlarge_hash_table(ht_t *ht)
{
  ht_size_t i;
  hash_t hash;

  const ht_size_t old_sz  = ht->sz;
  ht->sz  = 2*ht->sz;
#if HASH_DEBUG
  printf("enlarging hash table: %10u --> %10u\n", old_sz, ht->sz);
#endif
  ht->lut   = realloc(ht->lut, ht->sz * sizeof(ht_size_t));
  ht->val   = realloc(ht->val, ht->sz * sizeof(hash_t));
  ht->deg   = realloc(ht->deg, ht->sz * sizeof(deg_t));
  ht->idx   = realloc(ht->idx, ht->sz * sizeof(ht_size_t));
#if 1
  ht->dm    = realloc(ht->dm, ht->sz * sizeof(divm_t));
#endif
#if HASH_CHECK
  ht->ctr   = realloc(ht->ctr, ht->sz * sizeof(ht_size_t));
#endif
  ht->div   = realloc(ht->div, ht->sz * sizeof(nelts_t));
  /* set mew values for divisors and index to zero */
  memset(ht->idx+old_sz, 0, (ht->sz-old_sz) * sizeof(ht_size_t));
#if HASH_CHECK
  memset(ht->ctr+old_sz, 0, (ht->sz-old_sz) * sizeof(ht_size_t));
#endif
  memset(ht->lut+old_sz, 0, (ht->sz-old_sz) * sizeof(ht_size_t));
  memset(ht->div+old_sz, 0, (ht->sz-old_sz) * sizeof(nelts_t));
  ht->exp   = realloc(ht->exp, ht->nv * ht->sz * sizeof(exp_t *));
  /* for (i=old_sz; i<ht->sz; ++i) {
   *   ht->exp[i]  = (exp_t *)calloc(ht->nv, sizeof(exp_t));
   * } */
  /* re-insert all elements in block */
  memset(ht->lut+1, 0, (ht->sz-1) * sizeof(ht_size_t));
  for (i=1; i<ht->load; ++i) {
    hash  = ht->val[i];
    /* printf("coming from position %u ---> ",i); */
    insert_while_enlarging(hash, i, ht);
  }
}

/**
 * \brief Frees dynamically allocated data in hash table
 *
 * \param hash table ht
 */
static inline void free_hash_table(ht_t **ht_in)
{
  ht_t *ht = *ht_in;
  if (ht) {

    hash_t i;

    free(ht->lut);
    free(ht->val);
    free(ht->rand);
    free(ht->deg);
    free(ht->div);
    free(ht->idx);
    free(ht->dm);
    free(ht->divmap);
#if HASH_CHECK
    free(ht->ctr);
#endif
    /* for (i=0; i<ht->sz; ++i) {
     *   free(ht->exp[i]);
     * } */
    free(ht->exp);
  }

  free(ht);
  ht      = NULL;
  *ht_in  = ht;
}

/**
 * \brief Get hash value
 *
 * \param exponent vector exp
 *
 * \param hash table hash_table
 *
 * \return hash value
 */
static inline hash_t get_hash(const exp_t *exp, const ht_t *ht)
{
  hash_t hash = 0;
  if (ht->offset)
    hash  +=  ht->rand[0] * exp[0];
  for (size_t i = ht->offset; i < ht->nv; i = i+2) {
    hash  +=  ht->rand[i] * exp[i];
    hash  +=  ht->rand[i+1] * exp[i+1];
  }
#if HASH_DEBUG
  for (nelts_t i=0; i<ht->nv; ++i)
    printf("%u ", exp[i]);
  printf(" --> %lu\n", hash);
#endif
  return hash;
}

/**
 * \brief Inserts a new element to the hash table
 *
 * \note The exponent to be checked is already stored in ht->exp[ht->load]. If
 * it has to be inserted, we have it already at the correct place. Otherwise,
 * ht->load will not be increased and ht->exp[ht->load] will be overwritten
 * later on.
 *
 * \param hash value of exp hash
 *
 * \param position in lookup table pos
 *
 * \param hash table ht
 *
 * \return position of hash of exp in table
 */
static inline hash_t insert_in_hash_table(const hash_t hash,
    const ht_size_t pos,  ht_t *ht)
{
/* the new exponent is already stored in ht->exp[ht->load] and also
 * ht->deg[ht->load] is already set
 *
 * ht->div and ht->idx are already initialized with 0, so nothing to do there */
  /* ht->val[ht->load]   = hash; */
  ht->lut[pos]        = ht->load;
  if (ht->rcdm == 0)
    recalculate_divmaps(ht);
  ht->dm[ht->load]  = generate_divmask(ht->exp + (ht->nv * ht->load), ht);
  ht->rcdm--;
#if HASH_DEBUG
  for (int i=0; i<ht->nv; ++i)
    printf("%u ",ht->exp[(ht->nv * ht->load) + i]);
  printf(" ||| ");
  printf("%11u | %11u | %5u\n",hash, ht->load, ht->deg[ht->load]);
#endif
  ht->load++;

  /* we need to keep one place open in ht->exp since the next element to be
   * checked against the hash table will be intermediately stored there */
  if (ht->load >= ht->sz)
    enlarge_hash_table(ht);

  return (ht->load-1);
}

/**
 * \brief Checks if the given monomial exponent is already in the hash table. If
 * not, it is added to the table
 *
 * \note The exponent to be checked is already stored in ht->exp[ht->load]. If
 * it has to be inserted, we have it already at the correct place. Otherwise,
 * ht->load will not be increased and ht->exp[ht->load] will be overwritten
 * later on.
 *
 * \param hash table ht
 *
 * \return position of hash of exp in table
 */
static inline hash_t check_in_hash_table(ht_t *ht)
{
  
  hash_t hash = ht->val[ht->load];

  ht_size_t tmp_h = 0, tmp_l = 0;
  exp_t *exp  = ht->exp + (ht->nv * ht->load);

  /* printf("hash %d\n", hash);
   * for (size_t i = 0; i < ht->nv; ++i)
   *   printf("%d ", exp[i]);
   * printf("\n");
   * printf("hash %d | ht->load %u\n", hash, ht->load); */

  /* remaining checks with probing */
  for (size_t i = 0; i < ht->sz;  ++i) {
    tmp_h = (ht_size_t) (hash+i) & (ht->sz-1);
    tmp_l = ht->lut[tmp_h];
    /* if (hash == -829905355) {
     *   printf("tmp_h %d | tmp_l %d\n", tmp_h, tmp_l);
     * } */
    if (tmp_l == 0 && tmp_h != 0)
      break;
    if (ht->val[tmp_l] != hash)
       continue;
    if (memcmp(exp, ht->exp + (ht->nv * tmp_l), ht->nv*sizeof(exp_t)) == 0) {
      /* printf("found %d | %d\n", tmp_l, ht->load); */
      return tmp_l;
    }
  }
  /* at this point we know that we do not have the hash value of exp in the
   * table, so we have to insert it */
  hash_t ret =  insert_in_hash_table(hash, tmp_h, ht);
  /* printf("ret %u | ht->load %d\n", ret, ht->load); */
  return ret;
}

/**
 * \brief Finds the product of the given two monomial exponents is already
 * in the hash table.
 *
 * \note This function is used when constructing the gbla matrix, it is assumed
 * that the product is in the hash table, thus the hash table is used const.
 *
 * \note We use this also in the parallel construction of gbla matrices, thus we
 * have to make this procedure threadsafe, i.e. we are NOT allowed to store the
 * product of the hashes intermediately in ht->exp[ht->load] resp.
 * ht->ev[ht->load]. Otherwise, other threads will overwrite this value!
 *
 * \param monomial 1 mon_1
 *
 * \param monomial 2 mon_2
 *
 * \param hash table ht
 *
 * \return position of hash of exp in table
 */
static inline hash_t find_in_hash_table_product(const hash_t mon_1, const hash_t mon_2,
    const ht_t *ht)
{
  ht_size_t i;
  hash_t hash;
  exp_t exp[ht->nv];
  exp_t *ev1 = ht->exp + (ht->nv * mon_1);
  exp_t *ev2 = ht->exp + (ht->nv * mon_2);
  for (i = 0; i < ht->nv; ++i) {
    exp[i]    = (exp_t)(ev1[i] + ev2[i]);
  }
  /* hash value of the product is the sum of the hash values in our setting */
  hash   = ht->val[mon_1] + ht->val[mon_2];
  ht_size_t tmp_h;
  ht_size_t tmp_l;         /* temporary lookup table value */

  for (i = 0; i < ht->sz; ++i) {
    /* tmp_h = (tmp_h+i) & (ht->sz-1); */
    tmp_h = (hash+i) & (ht->sz-1);
    tmp_l = ht->lut[tmp_h];
    if (ht->val[tmp_l] != hash)
      continue;
    if (memcmp(exp, ht->exp+(ht->nv * tmp_l), ht->nv*sizeof(exp_t)) == 0)
      return tmp_l;
  }
  return 0;
}


/**
 * \brief Checks if the product of the given two monomial exponents is already
 * in the hash table. If not, it is added to the table
 *
 * \note We use the sum of the hash values of mon_1 and mon_2 as hash value for
 * the product of mon_1 and mon_2.
 *
 * \note The exponent to be checked is already stored in ht->exp[ht->load]. If
 * it has to be inserted, we have it already at the correct place. Otherwise,
 * ht->load will not be increased and ht->exp[ht->load] will be overwritten
 * later on.
 *
 * \param monomial 1 mon_1
 *
 * \param monomial 2 mon_2
 *
 * \param hash table ht
 *
 * \return position of hash of exp in table
 */
static inline hash_t check_in_hash_table_product_special(
    const hash_t mon, const hash_t mul_hash, const deg_t mul_deg,
    const exp_t *mul_exp, ht_t *ht)
{
  exp_t *ev = ht->exp + (ht->nv * ht->load);
  exp_t *em = ht->exp + (ht->nv * mon);
  for (size_t i = 0; i < ht->nv; ++i) {
    ev[i]  = (exp_t)(em[i] + mul_exp[i]);
  }
  /* for (size_t i = 0; i < ht->nv; ++i) {
   *   printf("%d ", ht->exp[ht->load][i]);
   * } */
  ht->deg[ht->load] = ht->deg[mon] + mul_deg;
  ht->val[ht->load] = ht->val[mon] + mul_hash;
  /* printf("%d + %d = %d", ht->val[mon], mul_hash, ht->val[ht->load]);
   * printf("\n"); */

  return check_in_hash_table(ht);
}

static inline hash_t check_in_hash_table_product(const hash_t mon_1, const hash_t mon_2,
    ht_t *ht)
{
  exp_t *ev   = ht->exp + (ht->nv * ht->load);
  exp_t *ev1  = ht->exp + (ht->nv * mon_1);
  exp_t *ev2  = ht->exp + (ht->nv * mon_2);
  for (size_t i = 0; i < ht->nv; ++i) {
    ev[i]  = (exp_t)(ev1[i] + ev2[i]);
  }
  ht->deg[ht->load] = ht->deg[mon_1] + ht->deg[mon_2];
  ht->val[ht->load] = ht->val[mon_1] + ht->val[mon_2];

  return check_in_hash_table(ht);
}

/**
 * \brief Returns position of lcm of the exponents stored at position h1 and h2
 * in the hash table.
 *
 * \param position of first generator monomial h1
 *
 * \param position of second generator monomial h2
 *
 * \param hash table ht
 *
 * \return position of lcm of generators h1 and h2 in hash table ht
 */
static inline hash_t get_lcm(const hash_t h1, const hash_t h2, ht_t *ht)
{
  nvars_t i;
  deg_t deg = 0;

  /* use first free entry in hash table ht to store possible new lcm monomial */
  exp_t *lcm = ht->exp + (ht->load * ht->nv);
  const exp_t *e1  = ht->exp + (h1 * ht->nv);
  const exp_t *e2  = ht->exp + (h2 * ht->nv);

  for (i=0; i<ht->nv; ++i) {
    deg +=  lcm[i]  = e1[i] < e2[i] ? e2[i] : e1[i];
    /* printf("%u ", lcm[i]); */
  }
  /* printf(" -- "); */
  ht->deg[ht->load] = deg;
  ht->val[ht->load] = get_hash(lcm, ht);
  return check_in_hash_table(ht);
}

/**
 * \brief Tests if exp of h1 is divisible by exp of h2. If divisibility is
 * fulfilled we add the multiplier to the hash table and return its hash
 * position. Else we return 0.
 *
 * \param hash position h1
 *
 * \param hash position h2
 *
 * \param hash table ht
 *
 * \return hash position of multiplier or 0
 */
static inline hash_t monomial_division(const hash_t h1, const hash_t h2, ht_t *ht)
{
  if ((ht->dm[h2] & ~ht->dm[h1])) {
#if COUNT_DIV_HITS
    meta_data->non_div_found++;
    meta_data->non_div++;
#endif
    return 0;
  }
  if (ht->deg[h1] < ht->deg[h2]) {
#if COUNT_DIV_HITS
    meta_data->non_div++;
#endif
    return 0;
  }
  nvars_t i = 0;
  exp_t *e, *e1, *e2;

  e   = ht->exp + (ht->nv * ht->load);
  e1  = ht->exp + (ht->nv * h1);
  e2  = ht->exp + (ht->nv * h2);

  if (e1[0] < e2[0]) {
#if COUNT_DIV_HITS
    meta_data->non_div++;
#endif
    return 0;
  }
  e[i]  = (exp_t)(e1[i] - e2[i]);
  i = ht->nv & 1 ? 1 : 0;
  for (; i<ht->nv; i=i+2) {
    if (e1[i] < e2[i] || e1[i+1] < e2[i+1]) {
#if COUNT_DIV_HITS
      meta_data->non_div++;
#endif
      return 0;
    }
    e[i]    = (exp_t)(e1[i] - e2[i]);
    e[i+1]  = (exp_t)(e1[i+1] - e2[i+1]);
  }
  ht->deg[ht->load] = ht->deg[h1] - ht->deg[h2];
  ht->val[ht->load] = ht->val[h1] - ht->val[h2];
  /* ht->val[ht->load] = get_hash(lcm, ht); */
  return check_in_hash_table(ht);
}

/**
 * \brief Tests if exp of h1 is divisible by exp of h2. If divisibility is
 * fulfilled only 1 is returned, else 0.
 *
 * \note This procedure only tests "if" divisible, but not "by which" it is
 * divisible. This is enough for detecting redundant elements when new elements
 * are added to the intermediate groebner basis.
 *
 * \param hash position h1
 *
 * \param hash position h2
 *
 * \param hash table ht
 *
 * \return 0 if not divisible, 1 if divisible
 */
static inline int check_monomial_division(const hash_t h1, const hash_t h2, const ht_t *ht)
{
  if ((ht->dm[h2] & ~ht->dm[h1])) {
#if COUNT_DIV_HITS
    meta_data->non_div_found++;
    meta_data->non_div++;
#endif
    return 0;
  }
/*   if (ht->deg[h1] < ht->deg[h2]) {
 * #if COUNT_DIV_HITS
 *     meta_data->non_div++;
 * #endif
 *     return 0;
 *   } */
  /* nvars_t i; */
  const exp_t * const exp1  = ht->exp + (h1 * ht->nv);
  const exp_t * const exp2  = ht->exp + (h2 * ht->nv);

  if (exp1[0] < exp2[0]) {
#if COUNT_DIV_HITS
    meta_data->non_div++;
#endif
    return 0;
  }
  for (size_t i = ht->offset; i < ht->nv; i = i+2) {
    if (exp1[i] < exp2[i] || exp1[i+1] < exp2[i+1]) {
#if COUNT_DIV_HITS
      meta_data->non_div++;
#endif
      return 0;
    }
  }
  return 1;
}

/**
 * \brief Tests if exp of h1 is divisible by exp of h2. If divisibility is
 * fulfilled only 1 is returned, else 0. This is a special version where the
 * homogenization variable (last variable) is not taking care of. It is needed
 * for redundancy checks when saturating polynomials in homogenized
 * computations.
 *
 * \note This procedure only tests "if" divisible, but not "by which" it is
 * divisible. This is enough for detecting redundant elements when new elements
 * are added to the intermediate groebner basis.
 *
 * \param hash position h1
 *
 * \param hash position h2
 *
 * \param hash table ht
 *
 * \return 0 if not divisible, 1 if divisible (w.r.t to all variables besides
 * the last one)
 */
static inline int check_monomial_division_saturated(const hash_t h1, const hash_t h2, const ht_t *ht)
{
  if ((ht->dm[h2] & ~ht->dm[h1])) {
    return 0;
  }
  /* do not do the degree check: for saturated polynomials we have not computed
   * the correct degree! */
  const exp_t * const exp1  = ht->exp + (h1 * ht->nv);
  const exp_t * const exp2  = ht->exp + (h2 * ht->nv);

  /* note that we explicitly do not check w.r.t. the last variable! */
  if (exp1[0] < exp2[0])
    return 0;
  for (size_t i = ht->offset; i < ht->nv-1; i = i+2) {
    if (exp1[i] < exp2[i])
      return 0;
    if (exp1[i+1] < exp2[i+1])
      return 0;
  }
  return 1;
}

/**
 * \brief Returns the multiplier needed to multiply h2 with in order to get
 * h1
 *
 * \note It is nearly the same function as monomial_division, but here we know
 * that h2 divides h1, so we do not have to check this.
 *
 * \param hash position h1
 *
 * \param hash position h2
 *
 * \param hash table ht
 *
 * \return hash position of multiplier 
 */
#if DIRECT_HASH_ENTRY
static inline hash_t get_multiplier(const hash_t h1, const hash_t h2, ht_t *ht)
{
  const exp_t * const e1  = ht->exp + (h1 * ht->nv);
  const exp_t * const e2  = ht->exp + (h2 * ht->nv);
  exp_t *e  = (exp_t *)malloc(ht->nv * sizeof(exp_t));

  /* we know that exp e2 divides exp e1, so no check for e1[i] < e2[i] */
  for (size_t i = 0;i < ht->nv; ++i) {
    e[i]    = (exp_t)(e1[i] - e2[i]);
  }
  return check_in_hash_table(e, ht);
}
#else
static inline hash_t get_multiplier(const hash_t h1, const hash_t h2, ht_t *ht)
{
  exp_t *e  = ht->exp + (ht->load * ht->nv);
  const exp_t * const e1  = ht->exp + (h1 * ht->nv);
  const exp_t * const e2  = ht->exp + (h2 * ht->nv);

  /* we know that exp e2 divides exp e1, so no check for e1[i] < e2[i] */
  e[0]  = (exp_t)(e1[0] - e2[0]);
  for (size_t i = ht->offset; i < ht->nv; i = i+2) {
    e[i]    = (exp_t)(e1[i] - e2[i]);
    e[i+1]  = (exp_t)(e1[i+1] - e2[i+1]);
  }
  ht->deg[ht->load] = ht->deg[h1] - ht->deg[h2];
  ht->val[ht->load] = ht->val[h1] - ht->val[h2];
  return check_in_hash_table(ht);
}
#endif

/**
 * \brief Resets all idx entries of the hash table to zero
 *
 * \param hash table ht
 */
static inline void clear_hash_table_idx(ht_t *ht)
{
  memset(ht->idx, 0, ht->load * sizeof(ht_size_t));
  /* memset(ht->ctr, 0, ht->sz * sizeof(ht_size_t)); */
}

static inline ht_size_t get_hash_position(const hash_t hash, const ht_t *ht)
{
  ht_size_t tmp_h = 0, tmp_l = 0;

  /* remaining checks with probing */
  for (size_t i = 0; i < ht->sz;  ++i) {
    tmp_h = (ht_size_t) (hash+i) & (ht->sz-1);
    tmp_l = ht->lut[tmp_h];
    if (ht->val[tmp_l] != hash)
      continue;
    return tmp_h;
  }
}

static inline ht_t *clear_hash_table_after_symbolic_preprocessing_new(
    ht_t *ht, pre_t *mon, ps_t *ps, const gb_t *basis, const nelts_t nlm)
{
  ht_t *ht2 = init_hash_table((unsigned int)(log(ht->sz) / log(2)), ht->nv);
  ht2->sort = ht->sort;
  recalculate_divmaps(ht2);
/*   printf("ht2->sz %u\n", ht2->sz);
 *
 *   for (size_t i = 0; i < ht->load; ++i) {
 *     printf("%5u %d | ", i, ht->val[i]);
 *     for (size_t j = 0; j < ht->nv; ++j) {
 *       printf("%d ", ht->exp[i][j]);
 *     }
 *     printf("\n");
 *   } */

  for (size_t i = basis->load-1; i > 0; --i) {
  /* for (size_t i = 1; i < basis->load; ++i) { */
    /* printf("i %u\n", i); */
    for (size_t j = 2; j < basis->p[i][1]; j = j+2) {
      ht2->val[ht2->load] = ht->val[basis->p[i][j]];
      ht2->deg[ht2->load] = ht->deg[basis->p[i][j]];
      memcpy(ht2->exp+(ht->nv * ht2->load), ht->exp+(ht->nv * basis->p[i][j]), ht->nv * sizeof(exp_t));
      basis->p[i][j]  = check_in_hash_table(ht2);
    }
    /* printf("lm of %u  ", i);
     * for (size_t k = 0; k < ht2->nv; ++k) {
     *   printf("%d ", ht2->exp[basis->p[i][2]][k]);
     * }
     * rintf("\n"); */
  }
  for (size_t i = 0; i < ps->load; ++i) {
    /* printf("gen1 %u | gen2 %u\n", ps->pairs[i].gen1, ps->pairs[i].gen2); */
    ps->pairs[i].lcm  = get_lcm(basis->p[ps->pairs[i].gen1][2],
        basis->p[ps->pairs[i].gen2][2], ht2);
  }
  /* for (size_t i = 0; i < ht2->load; ++i) {
   *   printf("%5u %d | ", i, ht2->val[i]);
   *   for (size_t j = 0; j < ht2->nv; ++j) {
   *     printf("%d ", ht2->exp[i][j]);
   *   }
   *   printf("\n");
   * } */
  free_hash_table(&ht);
/*   for (size_t i = 0; i < ht2->load; ++i) {
 *     printf("%5u %d | ", i, ht2->val[i]);
 *     for (size_t j = 0; j < ht2->nv; ++j) {
 *       printf("%d ", ht2->exp[i][j]);
 *     }
 *     printf("\n");
 *   }
 *
 *   printf("ht2->load %u -- done\n", ht2->load); */
  return ht2;
}
      
static inline void clear_hash_table_after_symbolic_preprocessing(
    ht_t *ht, pre_t *mon, ps_t *ps, const gb_t *basis, const nelts_t nlm)
{

  const ht_size_t ht_length = ht->load;
#if 1
  if (nlm > 0) {
    nelts_t ctr = ht->load_ls;
    /* printf("ht->load_ls %u | mon->load %u\n", ht->load_ls, mon->load); */
    /* for (size_t i = 0; i < mon->load; ++i) {
     *   printf("%u | %u | %u || ", i, mon->hash[i], ht->idx[mon->hash[i]]);
     *   for (size_t j = 0; j < ht->nv; ++j) {
     *     printf("%d ", ht->exp[mon->hash[i]][j]);
     *   }
     *   printf("\n");
     * } */
    for (size_t i = ht->load_ls; i < ht->load; ++i) {
      if (ht->idx[i] < nlm) {
        ht->lut[get_hash_position(ht->val[i], ht)] = 0;
        ht->val[i]  = 0;
        ht->deg[i]  = 0;
        ht->dm[i]   = 0;
        /* printf("ht->idx[%u] = %u\n", i, ht->idx[i]); */
      } else {
        if (ctr != i) {
          /* printf("move ! ctr %u | i %u\n", ctr, i);
           * for (size_t j = 0; j < ht->nv; ++j) {
           *   printf("%d ", ht->exp[i][j]);
           * }
           * printf("\n"); */
          ht->val[ctr]  = ht->val[i];
          ht->deg[ctr]  = ht->deg[i];
          ht->dm[ctr]   = ht->dm[i];
          ht->div[ctr]  = ht->div[i];
          /* no need for resetting the idx entry, it is set to zero later on
           * ht->idx[ctr]  = ht->idx[i]; */
          /* for (size_t j = 0; j < ht->nv; ++j) {
           *   printf("%d ", ht->exp[i][j]);
           * }
           * printf("\n"); */
          memcpy(ht->exp+(ht->nv * ctr), ht->exp+(ht->nv * i), ht->nv * sizeof(exp_t));
          /* printf("%p : ", ht->exp[ctr]);
           * for (size_t j = 0; j < ht->nv; ++j) {
           *   printf("%d ", ht->exp[ctr][j]);
           * }
           * printf(" | ");
           * for (size_t j = 0; j < ht->nv; ++j) {
           *   printf("%d ", ht->exp[22][j]);
           * }
           * printf("\n"); */
          memset(ht->exp+(ht->nv * i), 0, ht->nv * sizeof(exp_t));

          /* printf("hash pos %d\n",get_hash_position(ht->val[i], ht)); */
          ht->lut[get_hash_position(ht->val[i], ht)]  = ctr; 
          /* ht->lut[get_hash_position(ht->val[i], ht)]  = ht->lut[i]; */
          /* printf("ht->idx[%u] = %u\n", i, ht->idx[i]); */
          mon->hash[ht->idx[i]] = ctr;
        }
        ctr++;
      }
    }
    /* printf("mon->load %u\n", mon->load);
     * for (size_t i = 0; i < mon->load; ++i) {
     *   printf("%u | %u | %u || %p : ", i, mon->hash[i], ht->idx[mon->hash[i]],
     *       ht->exp[mon->hash[i]]);
     *   for (size_t j = 0; j < ht->nv; ++j) {
     *     printf("%d ", ht->exp[mon->hash[i]][j]);
     *   }
     *   printf("\n");
     * } */
    /* printf("ht->load %u | ctr %u | nlm %u\n", ht->load, ctr, nlm); */
    ht->load_ls = ht->load = ctr;
    /* ht->load_ls = ht->load; */

    /* regenerate lcms of s-pairs if the system is affine */
    /* if (basis->hom == 0) {
     *   for (size_t i = 0; i < ps->load; ++i) {
     *     printf("pair check %u / %u\n", i, ps->load);
     *     ps->pairs[i].lcm  = get_lcm(
     *         basis->p[ps->pairs[i].gen1][2],
     *         basis->p[ps->pairs[i].gen2][2],
     *         ht);
     *   }
     * } */
  }
#endif
  /* clear div entries for freed ht entries */
  memset(ht->div+ht->load_ls, 0, (ht_length-ht->load) * sizeof(ht_size_t));
  /* clear idx entries */
  memset(ht->idx, 0, ht_length * sizeof(ht_size_t));
  /* printf("---> %u / %u\n", ht->load, ht_length); */
}

#if 0
static inline void clear_hash_table_after_update(
    ht_t *ht, pre_t *mon, ps_t *ps, const gb_t *basis, const nelts_t nlm)
{
  const ht_size_t ht_length = ht->load;
  if (nlm > 0) {
    nelts_t ctr = ht->load_ls;
    /* printf("ht->load_ls %u | mon->load %u\n", ht->load_ls, mon->load); */
    /* for (size_t i = 0; i < mon->load; ++i) {
     *   printf("%u | %u | %u || ", i, mon->hash[i], ht->idx[mon->hash[i]]);
     *   for (size_t j = 0; j < ht->nv; ++j) {
     *     printf("%d ", ht->exp[mon->hash[i]][j]);
     *   }
     *   printf("\n");
     * } */

    /* copy all newly added information (val & exp) of the last step in
     * temporary storage */
    size_t len  = ht->load - ht->load_ls;
    hash_t *val  = (hash_t *)malloc(len * sizeof(hash_t));
    memcpy(val, ht->val+ht->load_ls, len * sizeof(hash_t));
    exp_t* exp  = (exp_t *)malloc(len * ht->nv * sizeof(exp_t));
    memcpy(exp, ht->exp+(ht->load_ls*ht->nv), len * ht->nv * sizeof(exp_t));

    /* clear corresponding data in hash table */
    memset(ht->val+ht->load_ls, 0, len * sizeof(hash_t));
    memset(ht->exp+(ht->load_ls*ht->nv), 0, len * ht->nv * sizeof(exp_t));
    memset(ht->div+ht->load_ls, 0, (ht_length-ht->load) * sizeof(ht_size_t));

    /* clear also ht->idx for further usage in next symbolic preprocessing step */
    memset(ht->idx, 0, ht_length * sizeof(ht_size_t));
    /* reset hash table load */
    ht->load  = ht->load_ls;

    for (size_t i = basis->load_ls; i < basis->load; ++i) {
      for (size_t j = 2; j < basis->p[i][1]; j = j+2) {
        if (basis->p[i][j] >= ht->load_ls) {
          ht->val[ht->load] = val[basis->p[i][j] - ht->load_ls];
          memcpy(
              ht->exp + (ht->load * ht->nv),
              exp + (basis->p[i][j]
            check_in_hash_table();
        }
      }
    }
      
    /* regenerate lcms of s-pairs if the system is affine */
    if (basis->hom == 0) {
      for (size_t i = 0; i < ps->load; ++i) {
        printf("pair check %u / %u\n", i, ps->load);
        ps->pairs[i].lcm  = get_lcm(
            basis->p[ps->pairs[i].gen1][2],
            basis->p[ps->pairs[i].gen2][2],
            ht);
      }
    }
  }
}
#endif

static inline void set_column_indices_in_ht_idx(
    ht_t *ht, const pre_t *mon)
{
  for (size_t i = 0; i < mon->load; ++i)
    ht->idx[mon->hash[i]] = (ht_size_t)i;
}
#endif /* GB_HASH_TABLE_H */
