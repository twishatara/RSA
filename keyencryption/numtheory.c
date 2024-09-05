#include "numtheory.h"
#include "randstate.h"
#include <stdint.h>
#include <stdbool.h>
#include <gmp.h>
#include <stdio.h>

// Please note that this code is based off of the provided psuedo code in the assignment doc, along
// with Eugenes section on 2_4_22 and the coding standards doc found on canvas. Additionally I had
// heavy psuedo code collaboration with my sister Krisha Sharma (krvsharm).

void pow_mod(mpz_t out, mpz_t base, mpz_t exponent, mpz_t modulus) {
    mpz_t v, p, exp, v_p, p_p;
    mpz_inits(v, p, exp, v_p, p_p, NULL);
    mpz_set_ui(v, 1);
    mpz_set(exp, exponent);
    mpz_set(p, base); // mpz_t p <- mpz_t base; // ???
    while (mpz_cmp_ui(exp, 0) == 1) {
        if (mpz_odd_p(exp) != 0) {
            mpz_mul(v_p, v, p);
            mpz_mod(v, v_p, modulus); // v = (v * p) % modulus
        }
        mpz_mul(p_p, p, p);
        mpz_mod(p, p_p, modulus); // p = (p * p) % modulus
        mpz_fdiv_q_ui(exp, exp, 2);
    }
    mpz_set(out, v); // out = v
    mpz_clears(v, p, exp, v_p, p_p, NULL);
    return;
}

bool is_prime(mpz_t n, uint64_t iters) {
    mpz_t ncopy, n_1, r, s, upper, a, j, s_1, y, e, nminus;
    mpz_inits(ncopy, n_1, r, s, upper, a, j, s_1, y, e, nminus, NULL);
    mpz_set(ncopy, n);
    mpz_sub_ui(n_1, n, 1);
    mpz_sub_ui(nminus, n, 1);
    if (mpz_cmp_ui(n, 0) == 0) {
        mpz_clears(ncopy, n_1, r, s, upper, a, j, s_1, y, e, nminus, NULL);
        return false;
    }
    if (mpz_cmp_ui(n, 1) == 0) {
        mpz_clears(ncopy, n_1, r, s, upper, a, j, s_1, y, e, nminus, NULL);
        return false;
    }
    if (mpz_cmp_ui(n, 2) == 0) {
        mpz_clears(ncopy, n_1, r, s, upper, a, j, s_1, y, e, nminus, NULL);
        return true;
    }
    if (mpz_cmp_ui(n, 3) == 0) {
        mpz_clears(ncopy, n_1, r, s, upper, a, j, s_1, y, e, nminus, NULL);
        return true;
    }
    if (mpz_odd_p(n) != 0) {
        for (mpz_set_ui(s, 0); mpz_even_p(n_1) != 0;
             mpz_add_ui(s, s, 1)) { // loop while ncopy is even
            mpz_fdiv_q_ui(n_1, ncopy, 2);
            mpz_set(ncopy, n_1); // update value
        }
        mpz_set(r, ncopy); // set end result to r
        for (uint64_t i = 1; i <= iters; i++) {
            mpz_sub_ui(upper, n, 3);
            mpz_urandomm(a, state, upper);
            mpz_add_ui(a, a, 2);
            pow_mod(y, a, r, n);
            if ((mpz_cmp_ui(y, 1) != 0) && (mpz_cmp(y, nminus) != 0)) { // y != 1 and y != n-1
                mpz_set_ui(j, 1);
                mpz_sub_ui(s_1, s, 1);
                while (
                    (mpz_cmp(j, s_1) != 1) && (mpz_cmp(y, nminus) != 0)) { // j <= s-1 and y != n-1
                    mpz_set_ui(e, 2);
                    pow_mod(y, y, e, n);
                    if (mpz_cmp_ui(y, 1) == 0) { // if y == 1
                        mpz_clears(ncopy, n_1, r, s, upper, a, j, s_1, y, e, nminus, NULL);
                        return false;
                    }
                    mpz_add_ui(j, j, 1);
                }
                if (mpz_cmp(y, nminus) != 0) {
                    mpz_clears(ncopy, n_1, r, s, upper, a, j, s_1, y, e, nminus, NULL);
                    return false;
                }
            }
        }
        mpz_clears(ncopy, n_1, r, s, upper, a, j, s_1, y, e, nminus, NULL);
        return true;
    }
    mpz_clears(ncopy, n_1, r, s, upper, a, j, s_1, y, e, nminus, NULL);
    return false;
}

void gcd(mpz_t d, mpz_t a, mpz_t b) {
    mpz_t t, acopy, bcopy;
    mpz_inits(t, acopy, bcopy, NULL); // creates space in the heap and initializez the t to zero
    mpz_set(acopy, a);
    mpz_set(bcopy, b);
    while (mpz_cmp_ui(bcopy, 0) == 1
           || mpz_cmp_ui(bcopy, 0) == -1) { // while the compare function does not return 0
        mpz_set(t, bcopy);
        mpz_mod(bcopy, acopy, bcopy); // b = a mod b
        mpz_set(acopy, t);
    }
    mpz_set(d, acopy);
    mpz_clears(t, acopy, bcopy, NULL);
    return;
}

void mod_inverse(mpz_t i, mpz_t a, mpz_t n) {
    mpz_t r, rprime, t, tprime, q, rcopy, tcopy, t_n;
    mpz_inits(r, rprime, t, tprime, q, rcopy, tcopy, t_n, NULL);
    mpz_set(r, n);
    mpz_set(rprime, a);
    mpz_set_ui(t, 0);
    mpz_set_ui(tprime, 1);
    while (mpz_cmp_ui(rprime, 0) != 0) {
        mpz_fdiv_q(q, r, rprime);
        mpz_set(rcopy, r); // rcopy = r
        mpz_set(r, rprime); // r = rprime
        mpz_submul(rcopy, q, rprime); // rcopy = rcopy (r) - q x rprime
        mpz_set(rprime, rcopy); // rprime = rcopy
        mpz_set(tcopy, t); // tcopy = t
        mpz_set(t, tprime); // t = tprime
        mpz_submul(tcopy, q, tprime); // tcopy = tcopy (t) - q x tprime
        mpz_set(tprime, tcopy); // tprime = tcopy
    }
    if (mpz_cmp_ui(r, 1) == 1) {
        mpz_set_ui(i, 0);
        mpz_clears(r, rprime, t, tprime, q, rcopy, tcopy, t_n, NULL);
        return;
    }
    if (mpz_cmp_ui(t, 0) == -1) {
        mpz_add(t_n, t, n);
        mpz_set(t, t_n);
    }
    mpz_set(i, t);
    mpz_clears(r, rprime, t, tprime, q, rcopy, tcopy, t_n, NULL);
    return;
}

void make_prime(mpz_t p, uint64_t bits, uint64_t iters) {
    mpz_t v, t, v1, v2;
    mpz_inits(v, t, v1, v2, NULL);
    mpz_set_ui(t, 2);
    mpz_pow_ui(v, t, bits); // v = 2^bits
    mpz_set_ui(v1, 0);
    mpz_set_ui(v2, 0);
    while (is_prime(v1, iters) == false) {
        mpz_rrandomb(v2, state, bits);
        mpz_add(v1, v2, v); // Credit: Eugene Section 2/4
    }
    mpz_set(p, v1);
    mpz_clears(v, t, v1, v2, NULL);
    return;
}
