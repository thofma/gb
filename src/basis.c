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
 * \file basis.c
 * \brief General and global data
 *
 * \author Christian Eder <ederc@mathematik.uni-kl.de>
 */

#include "data.h"

static void free_basis_elements(
        bs_t *bs
        )
{
    len_t i, j, len;
    if (bs->cf_8) {
        for (i = 0; i < bs->ld; ++i) {
            free(bs->cf_8[i]);
            if (bs->red[i] == 1) {
                free(bs->hm[i]);
            }
        }
    }
    if (bs->cf_16) {
        for (i = 0; i < bs->ld; ++i) {
            free(bs->cf_16[i]);
            if (bs->red[i] == 1) {
                free(bs->hm[i]);
            }
        }
    }
    if (bs->cf_32) {
        for (i = 0; i < bs->ld; ++i) {
            free(bs->cf_32[i]);
            if (bs->red[i] == 1) {
                free(bs->hm[i]);
            }
        }
    }
    if (bs->cf_qq) {
        for (i = 0; i < bs->ld; ++i) {
            len = bs->hm[i][2];
            mpz_t *coeffs =  bs->cf_qq[bs->hm[i][0]];
            for (j = 0; j < len; ++j) {
                mpz_clear(coeffs[j]);
            }
            free(bs->cf_qq[bs->hm[i][0]]);
            if (bs->red[i] == 1) {
                free(bs->hm[i]);
            }
        }
    }
    bs->ld  = 0;
}

static void free_basis(
        bs_t **bsp
        )
{
    len_t i, j, len;
    bs_t *bs  = *bsp;
    if (bs->cf_8) {
        for (i = 0; i < bs->ld; ++i) {
            free(bs->cf_8[i]);
            free(bs->hm[i]);
        }
        free(bs->cf_8);
        bs->cf_8  = NULL;
        free(bs->hm);
        bs->hm  = NULL;
    }
    if (bs->cf_16) {
        for (i = 0; i < bs->ld; ++i) {
            free(bs->cf_16[i]);
            free(bs->hm[i]);
        }
        free(bs->cf_16);
        bs->cf_16 = NULL;
        free(bs->hm);
        bs->hm  = NULL;
    }
    if (bs->cf_32) {
        for (i = 0; i < bs->ld; ++i) {
            free(bs->cf_32[i]);
            free(bs->hm[i]);
        }
        free(bs->cf_32);
        bs->cf_32 = NULL;
        free(bs->hm);
        bs->hm  = NULL;
    }
    if (bs->cf_qq) {
        for (i = 0; i < bs->ld; ++i) {
            len = bs->hm[i][2];
            mpz_t *coeffs =  bs->cf_qq[bs->hm[i][0]];
            for (j = 0; j < len; ++j) {
                mpz_clear(coeffs[j]);
            }
            free(bs->cf_qq[bs->hm[i][0]]);
            free(bs->hm[i]);
        }
        free(bs->cf_qq);
        bs->cf_qq  = NULL;
        free(bs->hm);
        bs->hm  = NULL;
    }
    free(bs->lmps);
    bs->lmps  = NULL;
    free(bs->lm);
    bs->lm  = NULL;
    free(bs->red);
    bs->red = NULL;
    free(bs);
    bs  = NULL;
    *bsp  = bs;
}

/* finite field stuff  --  8 bit */
static bs_t *initialize_basis_ff_8(
        const int32_t ngens
        )
{
    bs_t *bs  = (bs_t *)malloc(sizeof(bs_t));
    bs->lo  = 0;
    bs->ld  = 0;
    bs->lml = 0;
    bs->sz  = 2*ngens;

    bs->mltdeg  = 0;

    bs->cf_8  = (cf8_t **)malloc((unsigned long)bs->sz * sizeof(cf8_t *));
    bs->cf_16 = NULL;
    bs->cf_32 = NULL;
    bs->cf_qq = NULL;
    bs->hm    = (hm_t **)malloc((unsigned long)bs->sz * sizeof(hm_t *));
    bs->lm    = (sdm_t *)malloc((unsigned long)bs->sz * sizeof(sdm_t));
    bs->lmps  = (bl_t *)malloc((unsigned long)bs->sz * sizeof(bl_t));
    bs->red   = (int8_t *)calloc((unsigned long)bs->sz, sizeof(int8_t));

    return bs;
}

static inline void check_enlarge_basis_ff_8(
        bs_t *bs,
        const len_t added
        )
{
    if (bs->ld + added >= bs->sz) {
        bs->sz    = bs->sz * 2 > bs->ld + added ? bs->sz * 2 : bs->ld + added;
        bs->cf_8  = realloc(bs->cf_8,
                (unsigned long)bs->sz * sizeof(cf8_t *));
        bs->hm    = realloc(bs->hm, (unsigned long)bs->sz * sizeof(hm_t *));
        bs->lm    = realloc(bs->lm, (unsigned long)bs->sz * sizeof(sdm_t));
        bs->lmps  = realloc(bs->lmps, (unsigned long)bs->sz * sizeof(bl_t));
        bs->red   = realloc(bs->red, (unsigned long)bs->sz * sizeof(int8_t));
        memset(bs->red+bs->ld, 0,
                (unsigned long)(bs->sz-bs->ld) * sizeof(int8_t));
    }
}

static inline void normalize_initial_basis_ff_8(
        bs_t *bs,
        const int32_t fc
        )
{
    len_t i, j;
    int64_t tmp1, tmp2, tmp3, tmp4;

    cf8_t **cf         = bs->cf_8;
    hm_t * const *hm    = bs->hm;
    const bl_t ld       = bs->ld;
    const int8_t fc8    = (int8_t)fc;

    for (i = 0; i < ld; ++i) {
        cf8_t *row  = cf[hm[i][0]];

        const int8_t inv  = mod_p_inverse_8(row[0], fc8);
        const len_t os    = hm[i][1];
        const len_t len   = hm[i][2];

        for (j = 0; j < os; ++j) {
            tmp1    =   ((int64_t)row[j] * inv) % fc8;
            tmp1    +=  (tmp1 >> 63) & fc8;
            row[j]  =   (cf8_t)tmp1;
        }
        for (j = os; j < len; j += 4) {
            tmp1      =   ((int64_t)row[j] * inv) % fc8;
            tmp2      =   ((int64_t)row[j+1] * inv) % fc8;
            tmp3      =   ((int64_t)row[j+2] * inv) % fc8;
            tmp4      =   ((int64_t)row[j+3] * inv) % fc8;
            tmp1      +=  (tmp1 >> 63) & fc8;
            tmp2      +=  (tmp2 >> 63) & fc8;
            tmp3      +=  (tmp3 >> 63) & fc8;
            tmp4      +=  (tmp4 >> 63) & fc8;
            row[j]    =   (cf8_t)tmp1;
            row[j+1]  =   (cf8_t)tmp2;
            row[j+2]  =   (cf8_t)tmp3;
            row[j+3]  =   (cf8_t)tmp4;
        }
    }
}

/* finite field stuff  --  16 bit */
static bs_t *initialize_basis_ff_16(
        const int32_t ngens
        )
{
    bs_t *bs  = (bs_t *)malloc(sizeof(bs_t));
    bs->lo  = 0;
    bs->ld  = 0;
    bs->lml = 0;
    bs->sz  = 2*ngens;

    bs->mltdeg  = 0;

    bs->cf_8  = NULL;
    bs->cf_16 = (cf16_t **)malloc((unsigned long)bs->sz * sizeof(cf16_t *));
    bs->cf_32 = NULL;
    bs->cf_qq = NULL;
    bs->hm    = (hm_t **)malloc((unsigned long)bs->sz * sizeof(hm_t *));
    bs->lm    = (sdm_t *)malloc((unsigned long)bs->sz * sizeof(sdm_t));
    bs->lmps  = (bl_t *)malloc((unsigned long)bs->sz * sizeof(bl_t));
    bs->red   = (int8_t *)calloc((unsigned long)bs->sz, sizeof(int8_t));

    return bs;
}

static inline void check_enlarge_basis_ff_16(
        bs_t *bs,
        const len_t added
        )
{
    if (bs->ld + added >= bs->sz) {
        bs->sz    = bs->sz * 2 > bs->ld + added ? bs->sz * 2 : bs->ld + added;
        bs->cf_16 = realloc(bs->cf_16,
                (unsigned long)bs->sz * sizeof(cf16_t *));
        bs->hm    = realloc(bs->hm, (unsigned long)bs->sz * sizeof(hm_t *));
        bs->lm    = realloc(bs->lm, (unsigned long)bs->sz * sizeof(sdm_t));
        bs->lmps  = realloc(bs->lmps, (unsigned long)bs->sz * sizeof(bl_t));
        bs->red   = realloc(bs->red, (unsigned long)bs->sz * sizeof(int8_t));
        memset(bs->red+bs->ld, 0,
                (unsigned long)(bs->sz-bs->ld) * sizeof(int8_t));
    }
}

static inline void normalize_initial_basis_ff_16(
        bs_t *bs,
        const int32_t fc
        )
{
    len_t i, j;
    int64_t tmp1, tmp2, tmp3, tmp4;

    cf16_t **cf         = bs->cf_16;
    hm_t * const *hm    = bs->hm;
    const bl_t ld       = bs->ld;
    const int16_t fc16  = (int16_t)fc;

    for (i = 0; i < ld; ++i) {
        cf16_t *row = cf[hm[i][0]];

        const int16_t inv = mod_p_inverse_16(row[0], fc16);
        const len_t os    = hm[i][1];
        const len_t len   = hm[i][2];

        for (j = 0; j < os; ++j) {
            tmp1    =   ((int64_t)row[j] * inv) % fc16;
            tmp1    +=  (tmp1 >> 63) & fc16;
            row[j]  =   (cf16_t)tmp1;
        }
        for (j = os; j < len; j += 4) {
            tmp1      =   ((int64_t)row[j] * inv) % fc16;
            tmp2      =   ((int64_t)row[j+1] * inv) % fc16;
            tmp3      =   ((int64_t)row[j+2] * inv) % fc16;
            tmp4      =   ((int64_t)row[j+3] * inv) % fc16;
            tmp1      +=  (tmp1 >> 63) & fc16;
            tmp2      +=  (tmp2 >> 63) & fc16;
            tmp3      +=  (tmp3 >> 63) & fc16;
            tmp4      +=  (tmp4 >> 63) & fc16;
            row[j]    =   (cf16_t)tmp1;
            row[j+1]  =   (cf16_t)tmp2;
            row[j+2]  =   (cf16_t)tmp3;
            row[j+3]  =   (cf16_t)tmp4;
        }
    }
}

/* finite field stuff  --  32 bit */
static bs_t *initialize_basis_ff_32(
        const int32_t ngens
        )
{
    bs_t *bs  = (bs_t *)malloc(sizeof(bs_t));
    bs->lo  = 0;
    bs->ld  = 0;
    bs->lml = 0;
    bs->sz  = 2*ngens;

    bs->mltdeg  = 0;

    bs->cf_8  = NULL;
    bs->cf_16 = NULL;
    bs->cf_32 = (cf32_t **)malloc((unsigned long)bs->sz * sizeof(cf32_t *));
    bs->cf_qq = NULL;
    bs->hm    = (hm_t **)malloc((unsigned long)bs->sz * sizeof(hm_t *));
    bs->lm    = (sdm_t *)malloc((unsigned long)bs->sz * sizeof(sdm_t));
    bs->lmps  = (bl_t *)malloc((unsigned long)bs->sz * sizeof(bl_t));
    bs->red   = (int8_t *)calloc((unsigned long)bs->sz, sizeof(int8_t));

    return bs;
}

static inline void check_enlarge_basis_ff_32(
        bs_t *bs,
        const len_t added
        )
{
    if (bs->ld + added >= bs->sz) {
        bs->sz    = bs->sz * 2 > bs->ld + added ? bs->sz * 2 : bs->ld + added;
        bs->cf_32 = realloc(bs->cf_32,
                (unsigned long)bs->sz * sizeof(cf32_t *));
        bs->hm    = realloc(bs->hm, (unsigned long)bs->sz * sizeof(hm_t *));
        bs->lm    = realloc(bs->lm, (unsigned long)bs->sz * sizeof(sdm_t));
        bs->lmps  = realloc(bs->lmps, (unsigned long)bs->sz * sizeof(bl_t));
        bs->red   = realloc(bs->red, (unsigned long)bs->sz * sizeof(int8_t));
        memset(bs->red+bs->ld, 0,
                (unsigned long)(bs->sz-bs->ld) * sizeof(int8_t));
    }
}

static inline void normalize_initial_basis_ff_32(
        bs_t *bs,
        const int32_t fc
        )
{
    len_t i, j;
    int64_t tmp1, tmp2, tmp3, tmp4;

    cf32_t **cf       = bs->cf_32;
    hm_t * const *hm  = bs->hm;
    const bl_t ld     = bs->ld;

    for (i = 0; i < ld; ++i) {
        cf32_t *row = cf[hm[i][0]];

        const int32_t inv = mod_p_inverse_32((int32_t)row[0], (int32_t)fc);
        const len_t os    = hm[i][1]; 
        const len_t len   = hm[i][2]; 

        for (j = 0; j < os; ++j) {
            tmp1    =   ((int64_t)row[j] * inv) % fc;
            tmp1    +=  (tmp1 >> 63) & fc;
            row[j]  =   (cf32_t)tmp1;
        }
        for (j = os; j < len; j += 4) {
            tmp1      =   ((int64_t)row[j] * inv) % fc;
            tmp2      =   ((int64_t)row[j+1] * inv) % fc;
            tmp3      =   ((int64_t)row[j+2] * inv) % fc;
            tmp4      =   ((int64_t)row[j+3] * inv) % fc;
            tmp1      +=  (tmp1 >> 63) & fc;
            tmp2      +=  (tmp2 >> 63) & fc;
            tmp3      +=  (tmp3 >> 63) & fc;
            tmp4      +=  (tmp4 >> 63) & fc;
            row[j]    =   (cf32_t)tmp1;
            row[j+1]  =   (cf32_t)tmp2;
            row[j+2]  =   (cf32_t)tmp3;
            row[j+3]  =   (cf32_t)tmp4;
        }
    }
}

/* characteristic zero stuff */
static bs_t *initialize_basis_qq(
        const int32_t ngens
        )
{
    bs_t *bs  = (bs_t *)malloc(sizeof(bs_t));
    bs->lo  = 0;
    bs->ld  = 0;
    bs->lml = 0;
    bs->sz  = 2*ngens;

    bs->mltdeg  = 0;

    bs->cf_8  = NULL;
    bs->cf_16 = NULL;
    bs->cf_32 = NULL;
    bs->cf_qq = (mpz_t **)malloc((unsigned long)bs->sz * sizeof(mpz_t *));
    bs->hm    = (hm_t **)malloc((unsigned long)bs->sz * sizeof(hm_t *));
    bs->lm    = (sdm_t *)malloc((unsigned long)bs->sz * sizeof(sdm_t));
    bs->lmps  = (bl_t *)malloc((unsigned long)bs->sz * sizeof(bl_t));
    bs->red   = (int8_t *)calloc((unsigned long)bs->sz, sizeof(int8_t));

    return bs;
}

static inline void check_enlarge_basis_qq(
        bs_t *bs,
        const len_t added
        )
{
    if (bs->ld + added >= bs->sz) {
        bs->sz    = bs->sz * 2 > bs->ld + added ? bs->sz * 2 : bs->ld + added;
        bs->cf_qq = realloc(bs->cf_qq,
                (unsigned long)bs->sz * sizeof(mpz_t *));
        bs->hm    = realloc(bs->hm, (unsigned long)bs->sz * sizeof(hm_t *));
        bs->lm    = realloc(bs->lm, (unsigned long)bs->sz * sizeof(sdm_t));
        bs->lmps  = realloc(bs->lmps, (unsigned long)bs->sz * sizeof(bl_t));
        bs->red   = realloc(bs->red, (unsigned long)bs->sz * sizeof(int8_t));
        memset(bs->red+bs->ld, 0,
                (unsigned long)(bs->sz-bs->ld) * sizeof(int8_t));
    }
}

static inline void remove_content_of_initial_basis(
        bs_t *bs
        )
{
    len_t i, j;

    mpz_t **cf        = bs->cf_qq;
    hm_t * const *hm  = bs->hm;
    const bl_t ld     = bs->ld;

    mpz_t content;
    mpz_init(content);
    /* compute content, i.e. gcd of all coefficients */
    i = 0;
next_poly:
    for (; i < ld; ++i) {
        mpz_t *row = cf[hm[i][0]];
        mpz_set(content, row[0]);
        const len_t os  = hm[i][1];
        const len_t len = hm[i][2];
        for (j = 1; j < len; ++j) {
            mpz_gcd(content, content, row[j]);
            if (mpz_cmp_si(content, 1) == 0) {
                i++;
                goto next_poly;
            }
        }
        /* remove content */
        for (j = 0; j < os; ++j) {
            mpz_divexact(row[j], row[j], content);
        }
        for (; j < len; j += 4) {
            mpz_divexact(row[j], row[j], content);
            mpz_divexact(row[j+1], row[j+1], content);
            mpz_divexact(row[j+2], row[j+2], content);
            mpz_divexact(row[j+3], row[j+3], content);
        }
    }
    mpz_clear(content);

    /* make lead coefficient positive */
    for (i = 0; i < ld; ++i) {
        mpz_t *row = cf[hm[i][0]];
        const len_t os  = hm[i][1];
        const len_t len = hm[i][2];
        if (mpz_sgn(row[0]) == -1) {
            for (j = 0; j < os; ++j) {
                mpz_neg(row[j], row[j]);
            }
            for (; j < len; j += 4) {
                mpz_neg(row[j], row[j]);
                mpz_neg(row[j+1], row[j+1]);
                mpz_neg(row[j+2], row[j+2]);
                mpz_neg(row[j+3], row[j+3]);
            }
        }
    }
}
