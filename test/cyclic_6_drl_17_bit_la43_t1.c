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

    const int32_t lens[]  = {6, 6, 6, 6, 6, 2}; 
    const int32_t cfs[]   = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, -1};
    const int32_t exps[]  = {1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0};

    const int32_t nr_vars           = 6;
    const int32_t nr_gens           = 6;
    const int32_t ht_size           = 12;
    const int32_t field_char        = 65521;
    const int32_t mon_order         = 0;
    /* const int32_t nr_threads        = 1; */
    const int32_t nr_threads        = 1;
    const int32_t info_level				=	2;
    const int32_t pbm_file          = 0;
		const int32_t la_option         = 43;
    const int32_t max_nr_pairs      = 0;
    const int32_t reset_hash_table  = 0;

    int32_t failure = 0;

    int32_t **basis = (int32_t **)malloc(sizeof(int32_t *));
    int64_t len     = f4_julia(
            basis, lens, cfs, exps, field_char, mon_order, nr_vars,
            nr_gens, ht_size, nr_threads, max_nr_pairs, reset_hash_table,
            la_option, pbm_file, info_level);

    if (len != 8173) {
        failure = 1;
        free(*basis);
        free(basis);
        basis = NULL;
        return failure;
    }
    int32_t val[8173]  = {45, 42, 63, 91, 161, 105, 133, 168, 217, 217, 217, 217, 217, 217, 217, 217, 196, 175, 189, 189, 189, 189, 182, 182, 182, 189, 189, 189, 189, 189, 189, 182, 189, 182, 189, 189, 189, 189, 189, 189, 189, 189, 189, 189, 189, 189, 1, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 1, 0, 2, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 65520, 0, 0, 1, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 65520, 0, 0, 0, 1, 1, 0, 2, 0, 1, 0, 0, 0, 1, 1, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 0, 2, 1, 0, 1, 2, 0, 0, 0, 65520, 0, 1, 1, 1, 0, 0, 1, 0, 0, 2, 1, 0, 0, 65520, 0, 0, 2, 0, 0, 1, 1, 0, 1, 0, 0, 1, 1, 1, 0, 0, 1, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 2, 1, 65520, 0, 1, 0, 0, 0, 2, 65519, 0, 0, 1, 0, 0, 2, 65520, 0, 0, 0, 1, 0, 2, 1, 0, 0, 0, 0, 1, 2, 65520, 0, 0, 0, 0, 0, 3, 1, 0, 1, 0, 2, 0, 0, 28081, 0, 0, 1, 2, 0, 0, 28081, 0, 0, 0, 3, 0, 0, 4680, 0, 1, 1, 0, 1, 0, 14040, 0, 0, 2, 0, 1, 0, 9360, 0, 1, 0, 1, 1, 0, 4680, 0, 0, 1, 1, 1, 0, 37441, 0, 0, 0, 2, 1, 0, 37439, 0, 1, 0, 0, 2, 0, 14040, 0, 0, 1, 0, 2, 0, 56161, 0, 0, 0, 1, 2, 0, 28080, 0, 0, 0, 0, 3, 0, 18720, 0, 1, 1, 0, 0, 1, 9360, 0, 0, 2, 0, 0, 1, 14041, 0, 1, 0, 1, 0, 1, 56162, 0, 0, 0, 2, 0, 1, 9360, 0, 1, 0, 0, 1, 1, 42120, 0, 0, 1, 0, 1, 1, 46800, 0, 0, 0, 1, 1, 1, 37441, 0, 1, 0, 0, 0, 2, 46800, 0, 0, 1, 0, 0, 2, 28081, 0, 0, 0, 1, 0, 2, 18720, 0, 0, 0, 0, 1, 2, 1, 0, 0, 2, 1, 0, 0, 2, 0, 1, 1, 0, 1, 0, 65519, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 1, 1, 0, 65519, 0, 0, 0, 2, 1, 0, 65519, 0, 1, 1, 0, 0, 1, 65520, 0, 0, 2, 0, 0, 1, 2, 0, 1, 0, 1, 0, 1, 3, 0, 0, 1, 1, 0, 1, 2, 0, 0, 0, 2, 0, 1, 65518, 0, 0, 1, 0, 1, 1, 65520, 0, 0, 0, 1, 1, 1, 65520, 0, 0, 1, 0, 0, 2, 2, 0, 0, 0, 1, 0, 2, 65520, 0, 0, 0, 0, 1, 2, 1, 0, 1, 1, 1, 0, 0, 1, 0, 0, 1, 2, 0, 0, 32758, 0, 1, 1, 0, 1, 0, 32759, 0, 0, 2, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 32760, 0, 0, 1, 1, 1, 0, 2, 0, 0, 0, 2, 1, 0, 32759, 0, 0, 1, 0, 2, 0, 1, 0, 0, 0, 1, 2, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 0, 2, 0, 0, 1, 32761, 0, 1, 0, 1, 0, 1, 65520, 0, 0, 1, 1, 0, 1, 65520, 0, 0, 0, 2, 0, 1, 65520, 0, 1, 0, 0, 1, 1, 32760, 0, 0, 1, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1, 0, 0, 2, 65520, 0, 0, 0, 1, 0, 2, 1, 0, 0, 3, 0, 0, 0, 37438, 0, 0, 1, 2, 0, 0, 37440, 0, 0, 0, 3, 0, 0, 28083, 0, 1, 1, 0, 1, 0, 18722, 0, 0, 2, 0, 1, 0, 56160, 0, 1, 0, 1, 1, 0, 28083, 0, 0, 1, 1, 1, 0, 28077, 0, 0, 0, 2, 1, 0, 28077, 0, 1, 0, 0, 2, 0, 18722, 0, 0, 1, 0, 2, 0, 9361, 0, 0, 0, 1, 2, 0, 37440, 0, 0, 0, 0, 3, 0, 46799, 0, 1, 1, 0, 0, 1, 56163, 0, 0, 2, 0, 0, 1, 18722, 0, 1, 0, 1, 0, 1, 9361, 0, 0, 0, 2, 0, 1, 56160, 0, 1, 0, 0, 1, 1, 56160, 0, 0, 1, 0, 1, 1, 18716, 0, 0, 0, 1, 1, 1, 28083, 0, 1, 0, 0, 0, 2, 18719, 0, 0, 1, 0, 0, 2, 37444, 0, 0, 0, 1, 0, 2, 46799, 0, 0, 0, 0, 1, 2, 1, 0, 0, 0, 0, 0, 3, 1, 0, 0, 1, 1, 2, 0, 54908, 0, 0, 0, 2, 2, 0, 18365, 0, 1, 0, 0, 3, 0, 55036, 0, 0, 1, 0, 3, 0, 10168, 0, 0, 0, 1, 3, 0, 50777, 0, 0, 0, 0, 4, 0, 18565, 0, 0, 1, 2, 0, 1, 18439, 0, 0, 0, 3, 0, 1, 65386, 0, 1, 1, 0, 1, 1, 46684, 0, 0, 2, 0, 1, 1, 11228, 0, 1, 0, 1, 1, 1, 625, 0, 0, 1, 1, 1, 1, 15495, 0, 0, 0, 2, 1, 1, 3361, 0, 1, 0, 0, 2, 1, 15228, 0, 0, 1, 0, 2, 1, 5610, 0, 0, 0, 1, 2, 1, 14754, 0, 0, 0, 0, 3, 1, 37521, 0, 1, 1, 0, 0, 2, 56223, 0, 0, 2, 0, 0, 2, 11160, 0, 1, 0, 1, 0, 2, 46838, 0, 0, 1, 1, 0, 2, 13557, 0, 0, 0, 2, 0, 2, 23178, 0, 1, 0, 0, 1, 2, 45734, 0, 0, 1, 0, 1, 2, 45411, 0, 0, 0, 1, 1, 2, 61073, 0, 0, 0, 0, 2, 2, 26364, 0, 1, 0, 0, 0, 3, 61173, 0, 0, 1, 0, 0, 3, 22126, 0, 0, 0, 1, 0, 3, 37521, 0, 0, 0, 0, 1, 3, 24785, 0, 0, 0, 0, 0, 4, 1, 0, 1, 0, 1, 2, 0, 20972, 0, 0, 0, 2, 2, 0, 42959, 0, 1, 0, 0, 3, 0, 27391, 0, 0, 1, 0, 3, 0, 54844, 0, 0, 0, 1, 3, 0, 41689, 0, 0, 0, 0, 4, 0, 15607, 0, 0, 1, 2, 0, 1, 9188, 0, 0, 0, 3, 0, 1, 63324, 0, 1, 1, 0, 1, 1, 45521, 0, 0, 2, 0, 1, 1, 19562, 0, 1, 0, 1, 1, 1, 36311, 0, 0, 1, 1, 1, 1, 1043, 0, 0, 0, 2, 1, 1, 10517, 0, 1, 0, 0, 2, 1, 62173, 0, 0, 1, 0, 2, 1, 11891, 0, 0, 0, 1, 2, 1, 19611, 0, 0, 0, 0, 3, 1, 46247, 0, 1, 1, 0, 0, 2, 64050, 0, 0, 2, 0, 0, 2, 53337, 0, 1, 0, 1, 0, 2, 39277, 0, 0, 1, 1, 0, 2, 52694, 0, 0, 0, 2, 0, 2, 33229, 0, 1, 0, 0, 1, 2, 25917, 0, 0, 1, 0, 1, 2, 46856, 0, 0, 0, 1, 1, 2, 37432, 0, 0, 0, 0, 2, 2, 58429, 0, 1, 0, 0, 0, 3, 56516, 0, 0, 1, 0, 0, 3, 64286, 0, 0, 0, 1, 0, 3, 46247, 0, 0, 0, 0, 1, 3, 6736, 0, 0, 0, 0, 0, 4, 1, 0, 0, 2, 0, 2, 0, 63233, 0, 0, 0, 2, 2, 0, 62026, 0, 1, 0, 0, 3, 0, 57769, 0, 0, 1, 0, 3, 0, 21415, 0, 0, 0, 1, 3, 0, 44295, 0, 0, 0, 0, 4, 0, 62797, 0, 0, 1, 2, 0, 1, 2742, 0, 0, 0, 3, 0, 1, 62018, 0, 1, 1, 0, 1, 1, 61236, 0, 0, 2, 0, 1, 1, 22539, 0, 1, 0, 1, 1, 1, 29224, 0, 0, 1, 1, 1, 1, 44983, 0, 0, 0, 2, 1, 1, 42762, 0, 1, 0, 0, 2, 1, 37980, 0, 0, 1, 0, 2, 1, 39548, 0, 0, 0, 1, 2, 1, 30196, 0, 0, 0, 0, 3, 1, 52646, 0, 1, 1, 0, 0, 2, 53427, 0, 0, 2, 0, 0, 2, 16308, 0, 1, 0, 1, 0, 2, 17159, 0, 0, 1, 1, 0, 2, 25568, 0, 0, 0, 2, 0, 2, 32964, 0, 1, 0, 0, 1, 2, 45671, 0, 0, 1, 0, 1, 2, 50367, 0, 0, 0, 1, 1, 2, 60248, 0, 0, 0, 0, 2, 2, 36342, 0, 1, 0, 0, 0, 3, 27871, 0, 0, 1, 0, 0, 3, 40810, 0, 0, 0, 1, 0, 3, 52646, 0, 0, 0, 0, 1, 3, 48108, 0, 0, 0, 0, 0, 4, 1, 0, 1, 1, 0, 2, 0, 56306, 0, 0, 0, 2, 2, 0, 38701, 0, 1, 0, 0, 3, 0, 52493, 0, 0, 1, 0, 3, 0, 22561, 0, 0, 0, 1, 3, 0, 49188, 0, 0, 0, 0, 4, 0, 60792, 0, 0, 1, 2, 0, 1, 64604, 0, 0, 0, 3, 0, 1, 41524, 0, 1, 1, 0, 1, 1, 22260, 0, 0, 2, 0, 1, 1, 56, 0, 1, 0, 1, 1, 1, 18646, 0, 0, 1, 1, 1, 1, 31169, 0, 0, 0, 2, 1, 1, 2190, 0, 1, 0, 0, 2, 1, 48696, 0, 0, 1, 0, 2, 1, 18882, 0, 0, 0, 1, 2, 1, 44141, 0, 0, 0, 0, 3, 1, 6910, 0, 1, 1, 0, 0, 2, 26175, 0, 0, 2, 0, 0, 2, 39672, 0, 1, 0, 1, 0, 2, 36352, 0, 0, 1, 1, 0, 2, 42650, 0, 0, 0, 2, 0, 2, 34754, 0, 1, 0, 0, 1, 2, 13147, 0, 0, 1, 0, 1, 2, 60394, 0, 0, 0, 1, 1, 2, 2414, 0, 0, 0, 0, 2, 2, 32755, 0, 1, 0, 0, 0, 3, 46221, 0, 0, 1, 0, 0, 3, 19545, 0, 0, 0, 1, 0, 3, 6910, 0, 0, 0, 0, 1, 3, 42706, 0, 0, 0, 0, 0, 4, 1, 0, 0, 0, 3, 1, 0, 50838, 0, 0, 0, 2, 2, 0, 48554, 0, 1, 0, 0, 3, 0, 26691, 0, 0, 1, 0, 3, 0, 33677, 0, 0, 0, 1, 3, 0, 49442, 0, 0, 0, 0, 4, 0, 19963, 0, 0, 1, 2, 0, 1, 44114, 0, 0, 0, 3, 0, 1, 16512, 0, 1, 1, 0, 1, 1, 13067, 0, 0, 2, 0, 1, 1, 9702, 0, 1, 0, 1, 1, 1, 2654, 0, 0, 1, 1, 1, 1, 60111, 0, 0, 0, 2, 1, 1, 7622, 0, 1, 0, 0, 2, 1, 27619, 0, 0, 1, 0, 2, 1, 1023, 0, 0, 0, 1, 2, 1, 23709, 0, 0, 0, 0, 3, 1, 40642, 0, 1, 1, 0, 0, 2, 44087, 0, 0, 2, 0, 0, 2, 14136, 0, 1, 0, 1, 0, 2, 11826, 0, 0, 1, 1, 0, 2, 64206, 0, 0, 0, 2, 0, 2, 32907, 0, 1, 0, 0, 1, 2, 5267, 0, 0, 1, 0, 1, 2, 58003, 0, 0, 0, 1, 1, 2, 58971, 0, 0, 0, 0, 2, 2, 26488, 0, 1, 0, 0, 0, 3, 11921, 0, 0, 1, 0, 0, 3, 64511, 0, 0, 0, 1, 0, 3, 40643, 0, 0, 0, 0, 1, 3, 8387, 0, 0, 0, 0, 0, 4, 1, 0, 0, 1, 2, 1, 0, 43216, 0, 0, 0, 2, 2, 0, 15061, 0, 1, 0, 0, 3, 0, 22689, 0, 0, 1, 0, 3, 0, 61390, 0, 0, 0, 1, 3, 0, 22370, 0, 0, 0, 0, 4, 0, 38965, 0, 0, 1, 2, 0, 1, 59493, 0, 0, 0, 3, 0, 1, 64116, 0, 1, 1, 0, 1, 1, 23741, 0, 0, 2, 0, 1, 1, 6841, 0, 1, 0, 1, 1, 1, 6473, 0, 0, 1, 1, 1, 1, 29094, 0, 0, 0, 2, 1, 1, 56554, 0, 1, 0, 0, 2, 1, 26287, 0, 0, 1, 0, 2, 1, 57981, 0, 0, 0, 1, 2, 1, 65088, 0, 0, 0, 0, 3, 1, 38281, 0, 1, 1, 0, 0, 2, 13135, 0, 0, 2, 0, 0, 2, 27942, 0, 1, 0, 1, 0, 2, 3495, 0, 0, 1, 1, 0, 2, 52706, 0, 0, 0, 2, 0, 2, 42939, 0, 1, 0, 0, 1, 2, 35813, 0, 0, 1, 0, 1, 2, 32443, 0, 0, 0, 1, 1, 2, 63236, 0, 0, 0, 0, 2, 2, 10350, 0, 1, 0, 0, 0, 3, 42426, 0, 0, 1, 0, 0, 3, 53903, 0, 0, 0, 1, 0, 3, 38280, 0, 0, 0, 0, 1, 3, 59548, 0, 0, 0, 0, 0, 4, 1, 0, 0, 0, 4, 0, 0, 10733, 0, 0, 0, 2, 2, 0, 7319, 0, 1, 0, 0, 3, 0, 40032, 0, 0, 1, 0, 3, 0, 50523, 0, 0, 0, 1, 3, 0, 8645, 0, 0, 0, 0, 4, 0, 43984, 0, 0, 1, 2, 0, 1, 14692, 0, 0, 0, 3, 0, 1, 43493, 0, 1, 1, 0, 1, 1, 10232, 0, 0, 2, 0, 1, 1, 19262, 0, 1, 0, 1, 1, 1, 22743, 0, 0, 1, 1, 1, 1, 10621, 0, 0, 0, 2, 1, 1, 62862, 0, 1, 0, 0, 2, 1, 64807, 0, 0, 1, 0, 2, 1, 29644, 0, 0, 0, 1, 2, 1, 16831, 0, 0, 0, 0, 3, 1, 37553, 0, 1, 1, 0, 0, 2, 5293, 0, 0, 2, 0, 0, 2, 11857, 0, 1, 0, 1, 0, 2, 50473, 0, 0, 1, 1, 0, 2, 58864, 0, 0, 0, 2, 0, 2, 54023, 0, 1, 0, 0, 1, 2, 12592, 0, 0, 1, 0, 1, 2, 44863, 0, 0, 0, 1, 1, 2, 55690, 0, 0, 0, 0, 2, 2, 25715, 0, 1, 0, 0, 0, 3, 8507, 0, 0, 1, 0, 0, 3, 45299, 0, 0, 0, 1, 0, 3, 37553, 0, 0, 0, 0, 1, 3, 12588, 0, 0, 0, 0, 0, 4, 1, 0, 0, 1, 3, 0, 0, 9220, 0, 0, 0, 2, 2, 0, 26815, 0, 1, 0, 0, 3, 0, 13031, 0, 0, 1, 0, 3, 0, 42965, 0, 0, 0, 1, 3, 0, 16333, 0, 0, 0, 0, 4, 0, 4730, 0, 0, 1, 2, 0, 1, 916, 0, 0, 0, 3, 0, 1, 24000, 0, 1, 1, 0, 1, 1, 43259, 0, 0, 2, 0, 1, 1, 32700, 0, 1, 0, 1, 1, 1, 46875, 0, 0, 1, 1, 1, 1, 34346, 0, 0, 0, 2, 1, 1, 30577, 0, 1, 0, 0, 2, 1, 16831, 0, 0, 1, 0, 2, 1, 13893, 0, 0, 0, 1, 2, 1, 54152, 0, 0, 0, 0, 3, 1, 58601, 0, 1, 1, 0, 0, 2, 39342, 0, 0, 2, 0, 0, 2, 58625, 0, 1, 0, 1, 0, 2, 29166, 0, 0, 1, 1, 0, 2, 22873, 0, 0, 0, 2, 0, 2, 63521, 0, 1, 0, 0, 1, 2, 52346, 0, 0, 1, 0, 1, 2, 37878, 0, 0, 0, 1, 1, 2, 30357, 0, 0, 0, 0, 2, 2, 32766, 0, 1, 0, 0, 0, 3, 19302, 0, 0, 1, 0, 0, 3, 45978, 0, 0, 0, 1, 0, 3, 58600, 0, 0, 0, 0, 1, 3, 22816, 0, 0, 0, 0, 0, 4, 1, 0, 0, 0, 0, 5, 0, 60324, 0, 0, 0, 1, 3, 1, 15663, 0, 0, 0, 0, 4, 1, 5673, 0, 0, 1, 2, 0, 2, 23844, 0, 0, 0, 3, 0, 2, 1266, 0, 1, 1, 0, 1, 2, 22083, 0, 0, 2, 0, 1, 2, 31495, 0, 1, 0, 1, 1, 2, 23177, 0, 0, 1, 1, 1, 2, 32737, 0, 0, 0, 2, 1, 2, 28964, 0, 1, 0, 0, 2, 2, 33430, 0, 0, 1, 0, 2, 2, 5209, 0, 0, 0, 1, 2, 2, 22760, 0, 0, 0, 0, 3, 2, 5109, 0, 1, 1, 0, 0, 3, 49813, 0, 0, 2, 0, 0, 3, 14999, 0, 1, 0, 1, 0, 3, 59618, 0, 0, 1, 1, 0, 3, 55367, 0, 0, 0, 2, 0, 3, 46747, 0, 1, 0, 0, 1, 3, 27195, 0, 0, 1, 0, 1, 3, 12325, 0, 0, 0, 1, 1, 3, 33109, 0, 0, 0, 0, 2, 3, 2462, 0, 1, 0, 0, 0, 4, 1074, 0, 0, 1, 0, 0, 4, 38347, 0, 0, 0, 1, 0, 4, 5108, 0, 0, 0, 0, 1, 4, 62832, 0, 0, 0, 0, 0, 5, 1, 0, 0, 1, 2, 0, 3, 50961, 0, 0, 0, 3, 0, 3, 50959, 0, 1, 1, 0, 1, 3, 32761, 0, 0, 2, 0, 1, 3, 61877, 0, 1, 0, 1, 1, 3, 3634, 0, 0, 1, 1, 1, 3, 10918, 0, 0, 0, 2, 1, 3, 43682, 0, 1, 0, 0, 2, 3, 32761, 0, 0, 1, 0, 2, 3, 36394, 0, 0, 0, 1, 2, 3, 50961, 0, 0, 0, 0, 3, 3, 40044, 0, 1, 1, 0, 0, 4, 58242, 0, 0, 2, 0, 0, 4, 54599, 0, 1, 0, 1, 0, 4, 32764, 0, 0, 1, 1, 0, 4, 21840, 0, 0, 0, 2, 0, 4, 10924, 0, 1, 0, 0, 1, 4, 36404, 0, 0, 1, 0, 1, 4, 7281, 0, 0, 0, 1, 1, 4, 65520, 0, 1, 0, 0, 0, 5, 36403, 0, 0, 1, 0, 0, 5, 21839, 0, 0, 0, 1, 0, 5, 40044, 0, 0, 0, 0, 1, 5, 25480, 0, 0, 0, 0, 0, 6, 25480, 0, 0, 0, 0, 0, 0, 1, 0, 0, 2, 0, 1, 4, 46177, 0, 1, 0, 1, 1, 4, 4877, 0, 0, 1, 1, 1, 4, 62712, 0, 0, 0, 2, 1, 4, 45778, 0, 1, 0, 0, 2, 4, 25862, 0, 0, 1, 0, 2, 4, 42896, 0, 0, 0, 1, 2, 4, 15208, 0, 0, 0, 0, 3, 4, 41142, 0, 1, 1, 0, 0, 5, 41141, 0, 0, 2, 0, 0, 5, 30991, 0, 1, 0, 1, 0, 5, 44283, 0, 0, 1, 1, 0, 5, 10827, 0, 0, 0, 2, 0, 5, 23600, 0, 1, 0, 0, 1, 5, 28454, 0, 0, 1, 0, 1, 5, 59047, 0, 0, 0, 1, 1, 5, 64678, 0, 0, 0, 0, 2, 5, 40257, 0, 1, 0, 0, 0, 6, 21564, 0, 0, 1, 0, 0, 6, 43808, 0, 0, 0, 1, 0, 6, 63091, 0, 0, 0, 0, 1, 6, 9414, 0, 0, 0, 0, 0, 7, 34139, 0, 1, 0, 0, 0, 0, 63907, 0, 0, 1, 0, 0, 0, 22077, 0, 0, 0, 1, 0, 0, 57589, 0, 0, 0, 0, 1, 0, 39295, 0, 0, 0, 0, 0, 1, 1, 0, 1, 1, 0, 1, 4, 55091, 0, 1, 0, 1, 1, 4, 847, 0, 0, 1, 1, 1, 4, 45963, 0, 0, 0, 2, 1, 4, 29016, 0, 1, 0, 0, 2, 4, 24218, 0, 0, 1, 0, 2, 4, 29208, 0, 0, 0, 1, 2, 4, 50657, 0, 0, 0, 0, 3, 4, 49524, 0, 1, 1, 0, 0, 5, 49525, 0, 0, 2, 0, 0, 5, 24559, 0, 1, 0, 1, 0, 5, 36495, 0, 0, 1, 1, 0, 5, 7375, 0, 0, 0, 2, 0, 5, 51158, 0, 1, 0, 0, 1, 5, 35244, 0, 0, 1, 0, 1, 5, 30363, 0, 0, 0, 1, 1, 5, 42577, 0, 0, 0, 0, 2, 5, 50184, 0, 1, 0, 0, 0, 6, 3169, 0, 0, 1, 0, 0, 6, 41371, 0, 0, 0, 1, 0, 6, 49679, 0, 0, 0, 0, 1, 6, 63413, 0, 0, 0, 0, 0, 7, 2551, 0, 1, 0, 0, 0, 0, 48602, 0, 0, 1, 0, 0, 0, 60939, 0, 0, 0, 1, 0, 0, 31052, 0, 0, 0, 0, 1, 0, 4513, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 3, 0, 4, 17036, 0, 1, 0, 1, 1, 4, 41568, 0, 0, 1, 1, 1, 4, 43562, 0, 0, 0, 2, 1, 4, 4670, 0, 1, 0, 0, 2, 4, 52469, 0, 0, 1, 0, 2, 4, 26689, 0, 0, 0, 1, 2, 4, 33362, 0, 0, 0, 0, 3, 4, 41694, 0, 1, 1, 0, 0, 5, 41694, 0, 0, 2, 0, 0, 5, 35798, 0, 1, 0, 1, 0, 5, 19655, 0, 0, 1, 1, 0, 5, 41491, 0, 0, 0, 2, 0, 5, 20274, 0, 1, 0, 0, 1, 5, 46050, 0, 0, 1, 0, 1, 5, 33151, 0, 0, 0, 1, 1, 5, 33135, 0, 0, 0, 0, 2, 5, 1515, 0, 1, 0, 0, 0, 6, 9018, 0, 0, 1, 0, 0, 6, 51568, 0, 0, 0, 1, 0, 6, 39984, 0, 0, 0, 0, 1, 6, 3846, 0, 0, 0, 0, 0, 7, 10055, 0, 1, 0, 0, 0, 0, 21843, 0, 0, 1, 0, 0, 0, 49211, 0, 0, 0, 1, 0, 0, 23598, 0, 0, 0, 0, 1, 0, 43315, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 4, 3, 44683, 0, 1, 0, 1, 1, 4, 59906, 0, 0, 1, 1, 1, 4, 1072, 0, 0, 0, 2, 1, 4, 5247, 0, 1, 0, 0, 2, 4, 14199, 0, 0, 1, 0, 2, 4, 3074, 0, 0, 0, 1, 2, 4, 1298, 0, 0, 0, 0, 3, 4, 53987, 0, 1, 1, 0, 0, 5, 53987, 0, 0, 2, 0, 0, 5, 8010, 0, 1, 0, 1, 0, 5, 15223, 0, 0, 1, 1, 0, 5, 20070, 0, 0, 0, 2, 0, 5, 52754, 0, 1, 0, 0, 1, 5, 11120, 0, 0, 1, 0, 1, 5, 51087, 0, 0, 0, 1, 1, 5, 52235, 0, 0, 0, 0, 2, 5, 10525, 0, 1, 0, 0, 0, 6, 44557, 0, 0, 1, 0, 0, 6, 33023, 0, 0, 0, 1, 0, 6, 17925, 0, 0, 0, 0, 1, 6, 26513, 0, 0, 0, 0, 0, 7, 21357, 0, 1, 0, 0, 0, 0, 16956, 0, 0, 1, 0, 0, 0, 604, 0, 0, 0, 1, 0, 0, 29047, 0, 0, 0, 0, 1, 0, 6750, 0, 0, 0, 0, 0, 1, 1, 0, 1, 0, 1, 0, 6, 40651, 0, 0, 1, 1, 0, 6, 40651, 0, 0, 0, 2, 0, 6, 20319, 0, 1, 0, 0, 1, 6, 60970, 0, 0, 1, 0, 1, 6, 36100, 0, 0, 0, 1, 1, 6, 60970, 0, 0, 0, 0, 2, 6, 41571, 0, 1, 0, 0, 0, 7, 13036, 0, 0, 1, 0, 0, 7, 16701, 0, 0, 0, 1, 0, 7, 60970, 0, 0, 0, 0, 1, 7, 63095, 0, 0, 0, 0, 0, 8, 38219, 0, 1, 1, 0, 0, 0, 38219, 0, 0, 2, 0, 0, 0, 21840, 0, 1, 0, 1, 0, 0, 13349, 0, 0, 1, 1, 0, 0, 40651, 0, 0, 0, 2, 0, 0, 60344, 0, 1, 0, 0, 1, 0, 35474, 0, 0, 1, 0, 1, 0, 10604, 0, 0, 0, 1, 1, 0, 35474, 0, 0, 0, 0, 2, 0, 14269, 0, 1, 0, 0, 0, 1, 16698, 0, 0, 1, 0, 0, 1, 54920, 0, 0, 0, 1, 0, 1, 35474, 0, 0, 0, 0, 1, 1, 46724, 0, 0, 0, 0, 0, 2, 1, 0, 0, 2, 0, 0, 6, 6149, 0, 0, 1, 1, 0, 6, 6149, 0, 0, 0, 2, 0, 6, 36787, 0, 1, 0, 0, 1, 6, 42936, 0, 0, 1, 0, 1, 6, 49085, 0, 0, 0, 1, 1, 6, 42936, 0, 0, 0, 0, 2, 6, 41766, 0, 1, 0, 0, 0, 7, 33647, 0, 0, 1, 0, 0, 7, 47915, 0, 0, 0, 1, 0, 7, 42936, 0, 0, 0, 0, 1, 7, 20478, 0, 0, 0, 0, 0, 8, 27575, 0, 1, 1, 0, 0, 0, 27574, 0, 0, 2, 0, 0, 0, 43836, 0, 1, 0, 1, 0, 0, 33724, 0, 0, 1, 1, 0, 0, 6149, 0, 0, 0, 2, 0, 0, 42779, 0, 1, 0, 0, 1, 0, 48928, 0, 0, 1, 0, 1, 0, 55077, 0, 0, 0, 1, 1, 0, 48928, 0, 0, 0, 0, 2, 0, 3820, 0, 1, 0, 0, 0, 1, 48383, 0, 0, 1, 0, 0, 1, 9969, 0, 0, 0, 1, 0, 1, 48928, 0, 0, 0, 0, 1, 1, 35318, 0, 0, 0, 0, 0, 2, 1, 0, 1, 1, 0, 0, 6, 55515, 0, 0, 1, 1, 0, 6, 55514, 0, 0, 0, 2, 0, 6, 3801, 0, 1, 0, 0, 1, 6, 59315, 0, 0, 1, 0, 1, 6, 49308, 0, 0, 0, 1, 1, 6, 59315, 0, 0, 0, 0, 2, 6, 18414, 0, 1, 0, 0, 0, 7, 64870, 0, 0, 1, 0, 0, 7, 8407, 0, 0, 0, 1, 0, 7, 59315, 0, 0, 0, 0, 1, 7, 39261, 0, 0, 0, 0, 0, 8, 38060, 0, 1, 1, 0, 0, 0, 38061, 0, 0, 2, 0, 0, 0, 12389, 0, 1, 0, 1, 0, 0, 28053, 0, 0, 1, 1, 0, 0, 55514, 0, 0, 0, 2, 0, 0, 1904, 0, 1, 0, 0, 1, 0, 57418, 0, 0, 1, 0, 1, 0, 47411, 0, 0, 0, 1, 1, 0, 57418, 0, 0, 0, 0, 2, 0, 56473, 0, 1, 0, 0, 0, 1, 45575, 0, 0, 1, 0, 0, 1, 46466, 0, 0, 0, 1, 0, 1, 57418, 0, 0, 0, 0, 1, 1, 33140, 0, 0, 0, 0, 0, 2, 1, 0, 0, 0, 1, 2, 5, 38018, 0, 0, 0, 0, 3, 5, 22150, 0, 0, 1, 1, 0, 6, 35659, 0, 0, 0, 2, 0, 6, 29079, 0, 1, 0, 0, 1, 6, 39405, 0, 0, 1, 0, 1, 6, 5091, 0, 0, 0, 1, 1, 6, 21203, 0, 0, 0, 0, 2, 6, 30823, 0, 1, 0, 0, 0, 7, 3388, 0, 0, 1, 0, 0, 7, 18041, 0, 0, 0, 1, 0, 7, 51774, 0, 0, 0, 0, 1, 7, 53819, 0, 0, 0, 0, 0, 8, 5064, 0, 1, 1, 0, 0, 0, 5064, 0, 0, 2, 0, 0, 0, 36795, 0, 1, 0, 1, 0, 0, 17824, 0, 0, 1, 1, 0, 0, 64772, 0, 0, 0, 2, 0, 0, 20898, 0, 1, 0, 0, 1, 0, 24675, 0, 0, 1, 0, 1, 0, 34928, 0, 0, 0, 1, 1, 0, 32440, 0, 0, 0, 0, 2, 0, 8383, 0, 1, 0, 0, 0, 1, 38792, 0, 0, 1, 0, 0, 1, 4810, 0, 0, 0, 1, 0, 1, 28139, 0, 0, 0, 0, 1, 1, 49696, 0, 0, 0, 0, 0, 2, 1, 0, 0, 1, 0, 2, 5, 32568, 0, 0, 0, 0, 3, 5, 57947, 0, 0, 1, 1, 0, 6, 8120, 0, 0, 0, 2, 0, 6, 21973, 0, 1, 0, 0, 1, 6, 64497, 0, 0, 1, 0, 1, 6, 51841, 0, 0, 0, 1, 1, 6, 60005, 0, 0, 0, 0, 2, 6, 11103, 0, 1, 0, 0, 0, 7, 26748, 0, 0, 1, 0, 0, 7, 1694, 0, 0, 0, 1, 0, 7, 5199, 0, 0, 0, 0, 1, 7, 8446, 0, 0, 0, 0, 0, 8, 16778, 0, 1, 1, 0, 0, 0, 16778, 0, 0, 2, 0, 0, 0, 20756, 0, 1, 0, 1, 0, 0, 31610, 0, 0, 1, 1, 0, 0, 64659, 0, 0, 0, 2, 0, 0, 65504, 0, 1, 0, 0, 1, 0, 47099, 0, 0, 1, 0, 1, 0, 62419, 0, 0, 0, 1, 1, 0, 13658, 0, 0, 0, 0, 2, 0, 60449, 0, 1, 0, 0, 0, 1, 18995, 0, 0, 1, 0, 0, 1, 64849, 0, 0, 0, 1, 0, 1, 11772, 0, 0, 0, 0, 1, 1, 6305, 0, 0, 0, 0, 0, 2, 1, 0, 1, 0, 0, 2, 5, 30096, 0, 0, 0, 0, 3, 5, 10440, 0, 0, 1, 1, 0, 6, 33601, 0, 0, 0, 2, 0, 6, 12791, 0, 1, 0, 0, 1, 6, 53784, 0, 0, 1, 0, 1, 6, 26065, 0, 0, 0, 1, 1, 6, 40272, 0, 0, 0, 0, 2, 6, 44232, 0, 1, 0, 0, 0, 7, 26278, 0, 0, 1, 0, 0, 7, 11855, 0, 0, 0, 1, 0, 7, 34933, 0, 0, 0, 0, 1, 7, 13152, 0, 0, 0, 0, 0, 8, 49343, 0, 1, 1, 0, 0, 0, 49343, 0, 0, 2, 0, 0, 0, 48970, 0, 1, 0, 1, 0, 0, 13560, 0, 0, 1, 1, 0, 0, 6577, 0, 0, 0, 2, 0, 0, 48597, 0, 1, 0, 0, 1, 0, 47240, 0, 0, 1, 0, 1, 0, 26445, 0, 0, 0, 1, 1, 0, 62347, 0, 0, 0, 0, 2, 0, 58150, 0, 1, 0, 0, 0, 1, 62248, 0, 0, 1, 0, 0, 1, 23377, 0, 0, 0, 1, 0, 1, 36790, 0, 0, 0, 0, 1, 1, 46807, 0, 0, 0, 0, 0, 2, 1, 0, 0, 0, 2, 1, 5, 9335, 0, 0, 0, 0, 3, 5, 21257, 0, 0, 1, 1, 0, 6, 3093, 0, 0, 0, 2, 0, 6, 6666, 0, 1, 0, 0, 1, 6, 64990, 0, 0, 1, 0, 1, 6, 23948, 0, 0, 0, 1, 1, 6, 25519, 0, 0, 0, 0, 2, 6, 31246, 0, 1, 0, 0, 0, 7, 48129, 0, 0, 1, 0, 0, 7, 39366, 0, 0, 0, 1, 0, 7, 47707, 0, 0, 0, 0, 1, 7, 17284, 0, 0, 0, 0, 0, 8, 54127, 0, 1, 1, 0, 0, 0, 54127, 0, 0, 2, 0, 0, 0, 50641, 0, 1, 0, 1, 0, 0, 25636, 0, 0, 1, 1, 0, 0, 55193, 0, 0, 0, 2, 0, 0, 24695, 0, 1, 0, 0, 1, 0, 61341, 0, 0, 1, 0, 1, 0, 51312, 0, 0, 0, 1, 1, 0, 19455, 0, 0, 0, 0, 2, 0, 29188, 0, 1, 0, 0, 0, 1, 6549, 0, 0, 1, 0, 0, 1, 20988, 0, 0, 0, 1, 0, 1, 21301, 0, 0, 0, 0, 1, 1, 38679, 0, 0, 0, 0, 0, 2, 1, 0, 0, 1, 1, 1, 5, 58907, 0, 0, 0, 0, 3, 5, 16502, 0, 0, 1, 1, 0, 6, 60872, 0, 0, 0, 2, 0, 6, 27006, 0, 1, 0, 0, 1, 6, 13614, 0, 0, 1, 0, 1, 6, 1180, 0, 0, 0, 1, 1, 6, 2522, 0, 0, 0, 0, 2, 6, 30576, 0, 1, 0, 0, 0, 7, 28129, 0, 0, 1, 0, 0, 7, 49209, 0, 0, 0, 1, 0, 7, 20265, 0, 0, 0, 0, 1, 7, 61249, 0, 0, 0, 0, 0, 8, 12802, 0, 1, 1, 0, 0, 0, 12802, 0, 0, 2, 0, 0, 0, 515, 0, 1, 0, 1, 0, 0, 30340, 0, 0, 1, 1, 0, 0, 38690, 0, 0, 0, 2, 0, 0, 23379, 0, 1, 0, 0, 1, 0, 23386, 0, 0, 1, 0, 1, 0, 56459, 0, 0, 0, 1, 1, 0, 1235, 0, 0, 0, 0, 2, 0, 36764, 0, 1, 0, 0, 0, 1, 57476, 0, 0, 1, 0, 0, 1, 47479, 0, 0, 0, 1, 0, 1, 14469, 0, 0, 0, 0, 1, 1, 60424, 0, 0, 0, 0, 0, 2, 1, 0, 1, 0, 1, 1, 5, 49734, 0, 0, 0, 0, 3, 5, 60053, 0, 0, 1, 1, 0, 6, 36124, 0, 0, 0, 2, 0, 6, 60816, 0, 1, 0, 0, 1, 6, 28675, 0, 0, 1, 0, 1, 6, 62779, 0, 0, 0, 1, 1, 6, 18762, 0, 0, 0, 0, 2, 6, 12178, 0, 1, 0, 0, 0, 7, 997, 0, 0, 1, 0, 0, 7, 26429, 0, 0, 0, 1, 0, 7, 43017, 0, 0, 0, 0, 1, 7, 58542, 0, 0, 0, 0, 0, 8, 57802, 0, 1, 1, 0, 0, 0, 57802, 0, 0, 2, 0, 0, 0, 16188, 0, 1, 0, 1, 0, 0, 50173, 0, 0, 1, 1, 0, 0, 16300, 0, 0, 0, 2, 0, 0, 60906, 0, 1, 0, 0, 1, 0, 14526, 0, 0, 1, 0, 1, 0, 47082, 0, 0, 0, 1, 1, 0, 56836, 0, 0, 0, 0, 2, 0, 54193, 0, 1, 0, 0, 0, 1, 18982, 0, 0, 1, 0, 0, 1, 12512, 0, 0, 0, 1, 0, 1, 24832, 0, 0, 0, 0, 1, 1, 36574, 0, 0, 0, 0, 0, 2, 1, 0, 0, 0, 0, 0, 9, 45703, 0, 0, 1, 2, 0, 0, 38690, 0, 0, 0, 3, 0, 0, 45484, 0, 1, 1, 0, 1, 0, 35415, 0, 0, 2, 0, 1, 0, 314, 0, 1, 0, 1, 1, 0, 55924, 0, 0, 1, 1, 1, 0, 46038, 0, 0, 0, 2, 1, 0, 35577, 0, 1, 0, 0, 2, 0, 35415, 0, 0, 1, 0, 2, 0, 20509, 0, 0, 0, 1, 2, 0, 38690, 0, 0, 0, 0, 3, 0, 18711, 0, 1, 1, 0, 0, 1, 28780, 0, 0, 2, 0, 0, 1, 14088, 0, 1, 0, 1, 0, 1, 28424, 0, 0, 1, 1, 0, 1, 4846, 0, 0, 0, 2, 0, 1, 23686, 0, 1, 0, 0, 1, 1, 49367, 0, 0, 1, 0, 1, 1, 4182, 0, 0, 0, 1, 1, 1, 38993, 0, 0, 0, 0, 2, 1, 58703, 0, 1, 0, 0, 0, 2, 43273, 0, 0, 1, 0, 0, 2, 63549, 0, 0, 0, 1, 0, 2, 18819, 0, 0, 0, 0, 1, 2, 58592, 0, 0, 0, 0, 0, 3, 1, 0, 0, 0, 1, 0, 8, 43387, 0, 0, 0, 0, 1, 8, 20667, 0, 0, 1, 2, 0, 0, 5327, 0, 0, 0, 3, 0, 0, 49263, 0, 1, 1, 0, 1, 0, 59582, 0, 0, 2, 0, 1, 0, 39231, 0, 1, 0, 1, 1, 0, 4781, 0, 0, 1, 1, 1, 0, 52025, 0, 0, 0, 2, 1, 0, 56135, 0, 1, 0, 0, 2, 0, 54577, 0, 0, 1, 0, 2, 0, 49700, 0, 0, 0, 1, 2, 0, 33751, 0, 0, 0, 0, 3, 0, 52626, 0, 1, 1, 0, 0, 1, 42307, 0, 0, 2, 0, 0, 1, 30132, 0, 1, 0, 1, 0, 1, 46531, 0, 0, 1, 1, 0, 1, 65088, 0, 0, 0, 2, 0, 1, 12820, 0, 1, 0, 0, 1, 1, 24071, 0, 0, 1, 0, 1, 1, 61570, 0, 0, 0, 1, 1, 1, 5447, 0, 0, 0, 0, 2, 1, 21877, 0, 1, 0, 0, 0, 2, 23998, 0, 0, 1, 0, 0, 2, 49746, 0, 0, 0, 1, 0, 2, 27994, 0, 0, 0, 0, 1, 2, 50181, 0, 0, 0, 0, 0, 3, 1, 0, 0, 1, 0, 0, 8, 50848, 0, 0, 1, 2, 0, 0, 9027, 0, 0, 0, 3, 0, 0, 16548, 0, 1, 1, 0, 1, 0, 41692, 0, 0, 2, 0, 1, 0, 22400, 0, 1, 0, 1, 1, 0, 35634, 0, 0, 1, 1, 1, 0, 29564, 0, 0, 0, 2, 1, 0, 54162, 0, 1, 0, 0, 2, 0, 41692, 0, 0, 1, 0, 2, 0, 59463, 0, 0, 0, 1, 2, 0, 9027, 0, 0, 0, 0, 3, 0, 15915, 0, 1, 1, 0, 0, 1, 56292, 0, 0, 2, 0, 0, 1, 27759, 0, 1, 0, 1, 0, 1, 55739, 0, 0, 1, 1, 0, 1, 41754, 0, 0, 0, 2, 0, 1, 13965, 0, 1, 0, 0, 1, 1, 59517, 0, 0, 1, 0, 1, 1, 21692, 0, 0, 0, 1, 1, 1, 31121, 0, 0, 0, 0, 2, 1, 45814, 0, 1, 0, 0, 0, 2, 50521, 0, 0, 1, 0, 0, 2, 22047, 0, 0, 0, 1, 0, 2, 15895, 0, 0, 0, 0, 1, 2, 23684, 0, 0, 0, 0, 0, 3, 1, 0, 1, 0, 0, 0, 8, 22134, 0, 0, 0, 0, 1, 8, 49868, 0, 0, 1, 2, 0, 0, 3371, 0, 0, 0, 3, 0, 0, 47709, 0, 1, 1, 0, 1, 0, 19029, 0, 0, 2, 0, 1, 0, 2506, 0, 1, 0, 1, 1, 0, 21233, 0, 0, 1, 1, 1, 0, 60242, 0, 0, 0, 2, 1, 0, 61574, 0, 1, 0, 0, 2, 0, 24034, 0, 0, 1, 0, 2, 0, 28745, 0, 0, 0, 1, 2, 0, 40468, 0, 0, 0, 0, 3, 0, 5739, 0, 1, 1, 0, 0, 1, 34419, 0, 0, 2, 0, 0, 1, 9774, 0, 1, 0, 1, 0, 1, 53989, 0, 0, 1, 1, 0, 1, 21513, 0, 0, 0, 2, 0, 1, 1072, 0, 1, 0, 0, 1, 1, 26282, 0, 0, 1, 0, 1, 1, 64594, 0, 0, 0, 1, 1, 1, 24083, 0, 0, 0, 0, 2, 1, 2667, 0, 1, 0, 0, 0, 2, 50459, 0, 0, 1, 0, 0, 2, 61399, 0, 0, 0, 1, 0, 2, 30344, 0, 0, 0, 0, 1, 2, 19004, 0, 0, 0, 0, 0, 3, 1, 0, 0, 0, 0, 2, 7, 55638, 0, 0, 0, 0, 1, 8, 40156, 0, 0, 1, 2, 0, 0, 59604, 0, 0, 0, 3, 0, 0, 13846, 0, 1, 1, 0, 1, 0, 27598, 0, 0, 2, 0, 1, 0, 9158, 0, 1, 0, 1, 1, 0, 47366, 0, 0, 1, 1, 1, 0, 21891, 0, 0, 0, 2, 1, 0, 57227, 0, 1, 0, 0, 2, 0, 33147, 0, 0, 1, 0, 2, 0, 869, 0, 0, 0, 1, 2, 0, 12502, 0, 0, 0, 0, 3, 0, 59782, 0, 1, 1, 0, 0, 1, 46030, 0, 0, 2, 0, 0, 1, 43325, 0, 1, 0, 1, 0, 1, 24573, 0, 0, 1, 1, 0, 1, 64346, 0, 0, 0, 2, 0, 1, 58405, 0, 1, 0, 0, 1, 1, 63522, 0, 0, 1, 0, 1, 1, 65212, 0, 0, 0, 1, 1, 1, 13443, 0, 0, 0, 0, 2, 1, 20341, 0, 1, 0, 0, 0, 2, 12120, 0, 0, 1, 0, 0, 2, 31846, 0, 0, 0, 1, 0, 2, 15862, 0, 0, 0, 0, 1, 2, 19484, 0, 0, 0, 0, 0, 3, 1, 0, 0, 0, 1, 1, 7, 51894, 0, 0, 0, 0, 1, 8, 31612, 0, 0, 1, 2, 0, 0, 28812, 0, 0, 0, 3, 0, 0, 17601, 0, 1, 1, 0, 1, 0, 28588, 0, 0, 2, 0, 1, 0, 59118, 0, 1, 0, 1, 1, 0, 41450, 0, 0, 1, 1, 1, 0, 920, 0, 0, 0, 2, 1, 0, 11619, 0, 1, 0, 0, 2, 0, 2369, 0, 0, 1, 0, 2, 0, 65101, 0, 0, 0, 1, 2, 0, 49117, 0, 0, 0, 0, 3, 0, 47554, 0, 1, 1, 0, 0, 1, 36567, 0, 0, 2, 0, 0, 1, 12541, 0, 1, 0, 1, 0, 1, 11919, 0, 0, 1, 1, 0, 1, 39821, 0, 0, 0, 2, 0, 1, 22221, 0, 1, 0, 0, 1, 1, 23121, 0, 0, 1, 0, 1, 1, 37385, 0, 0, 0, 1, 1, 1, 12325, 0, 0, 0, 0, 2, 1, 25909, 0, 1, 0, 0, 0, 2, 19473, 0, 0, 1, 0, 0, 2, 29734, 0, 0, 0, 1, 0, 2, 16743, 0, 0, 0, 0, 1, 2, 62737, 0, 0, 0, 0, 0, 3, 1, 0, 0, 1, 0, 1, 7, 5754, 0, 0, 0, 0, 1, 8, 40332, 0, 0, 1, 2, 0, 0, 57516, 0, 0, 0, 3, 0, 0, 55342, 0, 1, 1, 0, 1, 0, 8881, 0, 0, 2, 0, 1, 0, 36632, 0, 1, 0, 1, 1, 0, 23479, 0, 0, 1, 1, 1, 0, 27529, 0, 0, 0, 2, 1, 0, 50629, 0, 1, 0, 0, 2, 0, 54909, 0, 0, 1, 0, 2, 0, 49327, 0, 0, 0, 1, 2, 0, 37298, 0, 0, 0, 0, 3, 0, 12784, 0, 1, 1, 0, 0, 1, 59245, 0, 0, 2, 0, 0, 1, 27380, 0, 1, 0, 1, 0, 1, 3677, 0, 0, 1, 1, 0, 1, 13339, 0, 0, 0, 2, 0, 1, 24517, 0, 1, 0, 0, 1, 1, 31378, 0, 0, 1, 0, 1, 1, 63501, 0, 0, 0, 1, 1, 1, 9393, 0, 0, 0, 0, 2, 1, 54800, 0, 1, 0, 0, 0, 2, 50602, 0, 0, 1, 0, 0, 2, 64408, 0, 0, 0, 1, 0, 2, 37457, 0, 0, 0, 0, 1, 2, 17184, 0, 0, 0, 0, 0, 3, 1, 0, 1, 0, 0, 1, 7, 17757, 0, 0, 0, 0, 1, 8, 53537, 0, 0, 1, 2, 0, 0, 41305, 0, 0, 0, 3, 0, 0, 42142, 0, 1, 1, 0, 1, 0, 60700, 0, 0, 2, 0, 1, 0, 23373, 0, 1, 0, 1, 1, 0, 38905, 0, 0, 1, 1, 1, 0, 3344, 0, 0, 0, 2, 1, 0, 21132, 0, 1, 0, 0, 2, 0, 35342, 0, 0, 1, 0, 2, 0, 41178, 0, 0, 0, 1, 2, 0, 22799, 0, 0, 0, 0, 3, 0, 4809, 0, 1, 1, 0, 0, 1, 51772, 0, 0, 2, 0, 0, 1, 64846, 0, 1, 0, 1, 0, 1, 46985, 0, 0, 1, 1, 0, 1, 16484, 0, 0, 0, 2, 0, 1, 44632, 0, 1, 0, 0, 1, 1, 42404, 0, 0, 1, 0, 1, 1, 15511, 0, 0, 0, 1, 1, 1, 30639, 0, 0, 0, 0, 2, 1, 61149, 0, 1, 0, 0, 0, 2, 54722, 0, 0, 1, 0, 0, 2, 39159, 0, 0, 0, 1, 0, 2, 54915, 0, 0, 0, 0, 1, 2, 53273, 0, 0, 0, 0, 0, 3, 1, 0, 0, 0, 2, 0, 7, 38294, 0, 0, 0, 0, 1, 8, 62754, 0, 0, 1, 2, 0, 0, 1016, 0, 0, 0, 3, 0, 0, 50746, 0, 1, 1, 0, 1, 0, 18905, 0, 0, 2, 0, 1, 0, 42921, 0, 1, 0, 1, 1, 0, 37390, 0, 0, 1, 1, 1, 0, 17554, 0, 0, 0, 2, 1, 0, 24619, 0, 1, 0, 0, 2, 0, 44727, 0, 0, 1, 0, 2, 0, 549, 0, 0, 0, 1, 2, 0, 51240, 0, 0, 0, 0, 3, 0, 10271, 0, 1, 1, 0, 0, 1, 42112, 0, 0, 2, 0, 0, 1, 59137, 0, 1, 0, 1, 0, 1, 12893, 0, 0, 1, 1, 0, 1, 37513, 0, 0, 0, 2, 0, 1, 306, 0, 1, 0, 0, 1, 1, 62955, 0, 0, 1, 0, 1, 1, 26866, 0, 0, 0, 1, 1, 1, 56029, 0, 0, 0, 0, 2, 1, 8563, 0, 1, 0, 0, 0, 2, 27853, 0, 0, 1, 0, 0, 2, 30810, 0, 0, 0, 1, 0, 2, 16438, 0, 0, 0, 0, 1, 2, 3790, 0, 0, 0, 0, 0, 3, 1, 0, 0, 1, 1, 0, 7, 15224, 0, 0, 0, 0, 1, 8, 18759, 0, 0, 1, 2, 0, 0, 43449, 0, 0, 0, 3, 0, 0, 8764, 0, 1, 1, 0, 1, 0, 6701, 0, 0, 2, 0, 1, 0, 46506, 0, 1, 0, 1, 1, 0, 60380, 0, 0, 1, 1, 1, 0, 2791, 0, 0, 0, 2, 1, 0, 21511, 0, 1, 0, 0, 2, 0, 52266, 0, 0, 1, 0, 2, 0, 43369, 0, 0, 0, 1, 2, 0, 57032, 0, 0, 0, 0, 3, 0, 27991, 0, 1, 1, 0, 0, 1, 30054, 0, 0, 2, 0, 0, 1, 36909, 0, 1, 0, 1, 0, 1, 63369, 0, 0, 1, 1, 0, 1, 36747, 0, 0, 0, 2, 0, 1, 65417, 0, 1, 0, 0, 1, 1, 1, 0, 0, 1, 0, 1, 1, 59440, 0, 0, 0, 1, 1, 1, 21807, 0, 0, 0, 0, 2, 1, 54986, 0, 1, 0, 0, 0, 2, 46531, 0, 0, 1, 0, 0, 2, 10698, 0, 0, 0, 1, 0, 2, 61901, 0, 0, 0, 0, 1, 2, 24690, 0, 0, 0, 0, 0, 3, 1, 0, 0, 0, 0, 3, 6, 9305, 0, 0, 0, 0, 1, 8, 8470, 0, 0, 1, 2, 0, 0, 45164, 0, 0, 0, 3, 0, 0, 60560, 0, 1, 1, 0, 1, 0, 50229, 0, 0, 2, 0, 1, 0, 21163, 0, 1, 0, 1, 1, 0, 10446, 0, 0, 1, 1, 1, 0, 13555, 0, 0, 0, 2, 1, 0, 49091, 0, 1, 0, 0, 2, 0, 38850, 0, 0, 1, 0, 2, 0, 59954, 0, 0, 0, 1, 2, 0, 21262, 0, 0, 0, 0, 3, 0, 32682, 0, 1, 1, 0, 0, 1, 43013, 0, 0, 2, 0, 0, 1, 45510, 0, 1, 0, 1, 0, 1, 15317, 0, 0, 1, 1, 0, 1, 62928, 0, 0, 0, 2, 0, 1, 15615, 0, 1, 0, 0, 1, 1, 46063, 0, 0, 1, 0, 1, 1, 38495, 0, 0, 0, 1, 1, 1, 22003, 0, 0, 0, 0, 2, 1, 37463, 0, 1, 0, 0, 0, 2, 29538, 0, 0, 1, 0, 0, 2, 10336, 0, 0, 0, 1, 0, 2, 28086, 0, 0, 0, 0, 1, 2, 36674, 0, 0, 0, 0, 0, 3, 1, 0, 0, 0, 1, 3, 0, 54855, 0, 0, 0, 0, 4, 0, 48977, 0, 0, 1, 2, 0, 1, 61167, 0, 0, 0, 3, 0, 1, 37369, 0, 1, 1, 0, 1, 1, 5442, 0, 0, 2, 0, 1, 1, 22423, 0, 1, 0, 1, 1, 1, 17563, 0, 0, 1, 1, 1, 1, 7910, 0, 0, 0, 2, 1, 1, 60220, 0, 1, 0, 0, 2, 1, 6964, 0, 0, 1, 0, 2, 1, 10596, 0, 0, 0, 1, 2, 1, 59643, 0, 0, 0, 0, 3, 1, 23000, 0, 1, 1, 0, 0, 2, 54927, 0, 0, 2, 0, 0, 2, 57759, 0, 1, 0, 1, 0, 2, 18791, 0, 0, 1, 1, 0, 2, 3990, 0, 0, 0, 2, 0, 2, 12769, 0, 1, 0, 0, 1, 2, 34611, 0, 0, 1, 0, 1, 2, 65374, 0, 0, 0, 1, 1, 2, 19808, 0, 0, 0, 0, 2, 2, 48544, 0, 1, 0, 0, 0, 3, 54201, 0, 0, 1, 0, 0, 3, 19012, 0, 0, 0, 1, 0, 3, 23000, 0, 0, 0, 0, 1, 3, 22857, 0, 0, 0, 0, 0, 4, 1, 0, 0, 1, 0, 3, 0, 1524, 0, 0, 0, 0, 4, 0, 3699, 0, 0, 1, 2, 0, 1, 11319, 0, 0, 0, 3, 0, 1, 33888, 0, 1, 1, 0, 1, 1, 18613, 0, 0, 2, 0, 1, 1, 59642, 0, 1, 0, 1, 1, 1, 13317, 0, 0, 1, 1, 1, 1, 31851, 0, 0, 0, 2, 1, 1, 39978, 0, 1, 0, 0, 2, 1, 60517, 0, 0, 1, 0, 2, 1, 51083, 0, 0, 0, 1, 2, 1, 2177, 0, 0, 0, 0, 3, 1, 64684, 0, 1, 1, 0, 0, 2, 14438, 0, 0, 2, 0, 0, 2, 26742, 0, 1, 0, 1, 0, 2, 3555, 0, 0, 1, 1, 0, 2, 55147, 0, 0, 0, 2, 0, 2, 12660, 0, 1, 0, 0, 1, 2, 1738, 0, 0, 1, 0, 1, 2, 39686, 0, 0, 0, 1, 1, 2, 20571, 0, 0, 0, 0, 2, 2, 24490, 0, 1, 0, 0, 0, 3, 16326, 0, 0, 1, 0, 0, 3, 42305, 0, 0, 0, 1, 0, 3, 64684, 0, 0, 0, 0, 1, 3, 6096, 0, 0, 0, 0, 0, 4, 1, 0, 1, 0, 0, 3, 0, 7619, 0, 0, 0, 0, 4, 0, 9142, 0, 0, 1, 2, 0, 1, 47236, 0, 0, 0, 3, 0, 1, 14984, 0, 1, 1, 0, 1, 1, 22856, 0, 0, 2, 0, 1, 1, 22095, 0, 1, 0, 1, 1, 1, 43175, 0, 0, 1, 1, 1, 1, 37586, 0, 0, 0, 2, 1, 1, 45457, 0, 1, 0, 0, 2, 1, 35808, 0, 0, 1, 0, 2, 1, 7367, 0, 0, 0, 1, 2, 1, 34284, 0, 0, 0, 0, 3, 1, 507, 0, 1, 1, 0, 0, 2, 58156, 0, 0, 2, 0, 0, 2, 63490, 0, 1, 0, 1, 0, 2, 50536, 0, 0, 1, 1, 0, 2, 60442, 0, 0, 0, 2, 0, 2, 16506, 0, 1, 0, 0, 1, 2, 27427, 0, 0, 1, 0, 1, 2, 29966, 0, 0, 0, 1, 1, 2, 37332, 0, 0, 0, 0, 2, 2, 33523, 0, 1, 0, 0, 0, 3, 11427, 0, 0, 1, 0, 0, 3, 38348, 0, 0, 0, 1, 0, 3, 507, 0, 0, 0, 0, 1, 3, 30475, 0, 0, 0, 0, 0, 4, 1, 0, 0, 0, 2, 2, 0, 53331, 0, 0, 0, 0, 4, 0, 4715, 0, 0, 1, 2, 0, 1, 31127, 0, 0, 0, 3, 0, 1, 364, 0, 1, 1, 0, 1, 1, 10231, 0, 0, 2, 0, 1, 1, 43897, 0, 1, 0, 1, 1, 1, 55728, 0, 0, 1, 1, 1, 1, 16614, 0, 0, 0, 2, 1, 1, 60803, 0, 1, 0, 0, 2, 1, 2614, 0, 0, 1, 0, 2, 1, 53117, 0, 0, 0, 1, 2, 1, 38747, 0, 0, 0, 0, 3, 1, 44115, 0, 1, 1, 0, 0, 2, 34248, 0, 0, 2, 0, 0, 2, 54422, 0, 1, 0, 1, 0, 2, 15236, 0, 0, 1, 1, 0, 2, 42445, 0, 0, 0, 2, 0, 2, 26628, 0, 1, 0, 0, 1, 2, 48469, 0, 0, 1, 0, 1, 2, 16323, 0, 0, 0, 1, 1, 2, 32001, 0, 0, 0, 0, 2, 2, 31855, 0, 1, 0, 0, 0, 3, 6675, 0, 0, 1, 0, 0, 3, 45350, 0, 0, 0, 1, 0, 3, 44115, 0, 0, 0, 0, 1, 3, 38602, 0, 0, 0, 0, 0, 4};

    for (i = 0; i < len; ++i) {
        if (val[i] != (*basis)[i]) {
            printf("difference at position %d: %d -- %d\n", i, val[i], (*basis)[i]);
            failure = 1;
            break;
        }
    }
    free(*basis);
    free(basis);
    basis = NULL;

    return failure;
}
