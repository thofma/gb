#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "../src/gb.c"

int main(
        void
        )
{
    int32_t i, j, k;
    len_t *hcm;

    int32_t round = 0;

    const int32_t lens[]  = {4, 4, 4, 2}; 
    const int32_t cfs[]   = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, -1};
    const int32_t exps[]  = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 0, 1, 0, 0, 1, 0, 0, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0};

    const int32_t nr_vars           = 4;
    const int32_t nr_gens           = 4;
    const int32_t ht_size           = 12;
    const int32_t field_char        = 65521;
    const int32_t mon_order         = 1;
    const int32_t nr_threads        = 1;
    const int32_t info_level				=	2;
		const int32_t la_option         = 1;
    const int32_t pbm_file          = 0;
    const int32_t max_nr_pairs      = 0;
    const int32_t reset_hash_table  = 0;

    int32_t failure = 0;

    /* returned basis data as pointers for interfaces */
    int32_t *bld    = (int32_t *)malloc(sizeof(int32_t));
    int32_t **blen  = (int32_t **)malloc(sizeof(int32_t *));
    exp_t **bexp    = (exp_t **)malloc(sizeof(exp_t *));
    void **bcf      = (void **)malloc(sizeof(void *));

    /* f4 computation:
     * ret = 0 if computation correct
     * ret = 1 if something went wrong */
    int ret = f4_ff_julia(
            bld, blen, bexp, bcf, lens, cfs, exps, field_char, mon_order, nr_vars,
            nr_gens, ht_size, nr_threads, max_nr_pairs, reset_hash_table,
            la_option, pbm_file, info_level);

    int ctr = 0;
    int32_t nterms  = 0;
    for (i = 0; i < (*bld); ++i) {
        nterms += (*blen)[i];
    }
    if ((*bld) != 6 || nterms != 24) {
        failure = 1;
    }

    int32_t tlen[6] = {4, 3, 4, 4, 5, 4};
    int32_t tcf[24] = {1, 1, 1, 1, 1, 2, 1, 1, 1,
        65520, 65520, 1, 65520, 1, 65520, 1, 65520,
        1, 1, 65519, 1, 65520, 65520, 1};
    exp_t texp[96] = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1,
        0, 0, 0, 0, 1, 0, 2, 0, 0, 0, 1, 0, 1, 0, 0, 0,
        2, 0, 0, 3, 2, 0, 0, 2, 3, 0, 0, 1, 0, 0, 0, 0, 1,
        0, 1, 0, 4, 0, 1, 0, 0, 0, 0, 0, 5, 0, 0, 0, 1, 0,
        1, 1, 0, 0, 1, 0, 1, 0, 0, 2, 4, 0, 0, 1, 1, 0, 0,
        0, 2, 0, 0, 2, 6, 0, 0, 2, 2, 0, 0, 0, 4, 0, 0, 0, 0};

    for (i = 0; i < (*bld); ++i) {
        if (tlen[i] != (*blen)[i]) {
            failure = 1;
            break;
        }
    }
    int32_t **cf  = (int32_t **)bcf;
    for (i = 0; i < nterms; ++i) {
        if (tcf[i] != (*cf)[i]) {
            failure = 1;
            break;
        }
    }
    for (i = 0; i < nterms * nr_vars; ++i) {
        if (texp[i] != (*bexp)[i]) {
            failure = 1;
            break;
        }
    }
    free(*blen);
    free(*bexp);
    free(*cf);
    free(blen);
    free(bexp);
    free(bcf);
    free(bld);
    return failure;
}
