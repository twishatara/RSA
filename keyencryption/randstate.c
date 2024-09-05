#include "randstate.h"
#include <gmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

// Please note that this code is based off of the provided psuedo code in the assignment doc, along
// with Eugenes section on 1_28_22 and the coding standards doc found on canvas. Additionally I had
// heavy psuedo code collaboration with my sister Krisha Sharma (krvsharm).

gmp_randstate_t state;

void randstate_init(uint64_t seed) {
    gmp_randinit_mt(state);
    gmp_randseed_ui(state, seed);
    srandom(seed);
}

void randstate_clear(void) {
    gmp_randclear(state);
}
