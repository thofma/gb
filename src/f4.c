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
 * \file f4.c
 * \brief Implementation of the F4 Algorithm
 *
 * \author Christian Eder <ederc@mathematik.uni-kl.de>
 */

#include "data.h"

/* we get from julia the generators as three arrays:
 * 0.  a pointer to an int32_t array for returning the basis to julia
 * 1.  an array of the lengths of each generator
 * 2.  an array of all coefficients of all generators in the order:
 *     first all coefficients of generator 1, then all of generator 2, ...
 * 3.  an array of all exponents of all generators in the order:
 *     first all exponents of generator 1, then all of generator 2, ...
 *
 *  RETURNs the length of the jl_basis array */
int64_t f4_julia(
        int32_t **jl_basis,
        const int32_t *lens,
        const int32_t *cfs,
        const int32_t *exps,
        const int32_t field_char,
        const int32_t mon_order,
        const int32_t nr_vars,
        const int32_t nr_gens,
        const int32_t ht_size,
        const int32_t nr_threads,
        const int32_t max_nr_pairs,
        const int32_t reset_hash_table,
        const int32_t la_option,
        const int32_t pbm_file,
        const int32_t info_level
        )
{
    /* timings */
    double ct0, ct1, rt0, rt1;
    double rct0, rct1, rrt0, rrt1; /* for one round only */
    ct0 = cputime();
    rt0 = realtime();

    int32_t round;
    hl_t *hcm; /* hash-column-map */
    /* matrix holding sparse information generated
     * during symbolic preprocessing */
    dt_t **mat;

    ps_t * ps  = initialize_pairset();

    /* initialize stuff */
    stat_t *st  = initialize_statistics();

    /* checks and set all meta data. if a nonzero value is returned then
     * some of the input data is corrupted. */
    if (check_and_set_meta_data(ps, st, lens, cfs, exps, field_char, mon_order,
                nr_vars, nr_gens, ht_size, nr_threads, max_nr_pairs,
                reset_hash_table, la_option, pbm_file, info_level)) {
        return 0;
    }
    if (st->info_level > 0) {
        print_initial_statistics(st);
    }

    initialize_basis(nr_gens);
    initialize_basis_hash_table();
    initialize_update_hash_table();
    initialize_symbolic_hash_table();

    import_julia_data_ff(lens, cfs, exps, nr_gens);

    /* for faster divisibility checks, needs to be done after we have
     * read some input data for applying heuristics */
    calculate_divmask();

    /* sort initial elements, smallest lead term first */
    qsort(gbdt, (unsigned long)nrows, sizeof(dt_t *),
            matrix_row_initial_input_cmp);
    /* normalize input generators */
    normalize_matrix_rows(gbcf_ff, gbdt);

    /* move input generators to basis and generate first spairs */
    update_basis(ps, st);

    /* let's start the f4 rounds,  we are done when no more spairs
     * are left in the pairset */
    if (st->info_level > 1) {
        printf("\ndeg     sel   pairs        mat          density \
          new data             time(rd)\n");
        printf("-------------------------------------------------\
----------------------------------------\n");
    }
    for (round = 1; ps->ld > 0; ++round) {
        if (round % rht == 0) {
            reset_basis_hash_table(ps, st);
        }
        rct0  = cputime();
        rrt0  = realtime();
        st->max_ht_size = hsz;
        st->current_rd  = round;

        /* preprocess data for next reduction round */
        mat = select_spairs_by_minimal_degree(ps, mat, st);
        mat = symbolic_preprocessing(mat, st);
        hcm = convert_hashes_to_columns(mat, st);
        mat = sort_matrix_rows(mat);
        /* print pbm files of the matrices */
        if (st->gen_pbm_file != 0) {
            write_pbm_file(mat, st); 
        }
        /* linear algebra, depending on choice, see set_function_pointers() */
        mat = linear_algebra(mat, st);
        /* columns indices are mapped back to exponent hashes */
        if (npivs > 0) {
            convert_sparse_matrix_rows_to_basis_elements(mat, hcm, st);
        }
        reset_symbolic_hash_table();
        free(mat);
        mat = NULL;
        free(hcm);
        hcm = NULL;

        update_basis(ps, st);

        rct1 = cputime();
        rrt1 = realtime();
        if (st->info_level > 1) {
            printf("%13.3f sec\n", rrt1-rrt0);
        }
    }
    if (st->info_level > 1) {
        printf("-------------------------------------------------\
----------------------------------------\n");
    }

    st->len_output  = export_julia_data_ff(jl_basis);
    st->size_basis  = (*jl_basis)[0];

    /* timings */
    ct1 = cputime();
    rt1 = realtime();
    st->overall_ctime = ct1 - ct0;
    st->overall_rtime = rt1 - rt0;

    if (st->info_level > 0) {
        print_final_statistics(st);
    }

    /* free and clean up */
    free_symbolic_hash_table();
    free_update_hash_table();
    free_basis_hash_table();
    free_pairset(&ps);
    /* note that all rows kept from mat during the overall computation are
     * basis elements and thus we do not need to free the rows itself, but
     * just the matrix structure */
    free(mat);
    free_basis();

    int64_t output  = st->len_output;
    free(st);

    return output;
}
