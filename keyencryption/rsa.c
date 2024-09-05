#include "numtheory.h"
#include "randstate.h"
#include "rsa.h"
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <gmp.h>
#include <stdlib.h>
#include <time.h>

// Please note that this code is based off of the provided psuedo code in the assignment doc, along
// with Eugenes section on 2_4_22 and the coding standards doc found on canvas. Additionally I had
// heavy psuedo code collaboration with my sister Krisha Sharma (krvsharm).

void rsa_make_pub(mpz_t p, mpz_t q, mpz_t n, mpz_t e, uint64_t nbits, uint64_t iters) {
    uint64_t pbits = (rand() % (nbits / 2) + nbits / 4); // generate pbits random bit number
    uint64_t qbits = nbits - pbits; // remain bits are qbits
    make_prime(p, pbits, iters); // create a prime number p
    make_prime(q, qbits, iters); // create a prime number q
    mpz_mul(n, p, q); // n = p x q
    //mpz_sizeinbase(n, 2); // equiv to log2(n)?
    mpz_t p_1, q_1, gcd_out, p1_q1, lcm_out, gcd_e;
    mpz_inits(p_1, q_1, gcd_out, p1_q1, lcm_out, gcd_e, NULL);
    mpz_sub_ui(p_1, p, 1); // p_1 = p - 1
    mpz_sub_ui(q_1, q, 1); // q_1 = q - 1
    gcd(gcd_out, p_1, q_1); // gcd of p_1 and q_1 stored in gcd_out
    mpz_mul(p1_q1, p_1, q_1); // p_1 x q_1 stored in p1_q1
    mpz_fdiv_q(lcm_out, p1_q1, gcd_out); // lcm_out(lambda n) = (p_1 x q_1) / gcd(p_1, q_1)
    mpz_set_ui(gcd_e, 0);
    while (
        mpz_cmp_ui(gcd_e, 1) != 0) { // while e isnt coprime (gcd of e and lambda n doesnt equal 1)
        mpz_urandomb(e, state, nbits); // generate random number of nbits
        gcd(gcd_e, e, lcm_out); // find gcd of the random number and lambda n
    }
    // the value e holds should be a coprime of lambda n -> its now the public exponent
    mpz_clears(p_1, q_1, gcd_out, p1_q1, lcm_out, gcd_e, NULL);
    return;
}

void rsa_write_pub(mpz_t n, mpz_t e, mpz_t s, char username[], FILE *pbfile) {
    gmp_fprintf(pbfile, "%Zx\n", n);
    gmp_fprintf(pbfile, "%Zx\n", e);
    gmp_fprintf(pbfile, "%Zx\n", s);
    gmp_fprintf(pbfile, "%s\n", username);
}

void rsa_read_pub(mpz_t n, mpz_t e, mpz_t s, char username[], FILE *pbfile) {
    gmp_fscanf(pbfile, "%Zx\n", n);
    gmp_fscanf(pbfile, "%Zx\n", e);
    gmp_fscanf(pbfile, "%Zx\n", s);
    gmp_fscanf(pbfile, "%s\n", username);
}

void rsa_make_priv(mpz_t d, mpz_t e, mpz_t p, mpz_t q) {
    mpz_t p_1, q_1, gcd_out, p1_q1, lcm_out;
    mpz_inits(p_1, q_1, gcd_out, p1_q1, lcm_out, NULL);
    mpz_sub_ui(p_1, p, 1); // p_1 = p - 1
    mpz_sub_ui(q_1, q, 1); // q_1 = q - 1
    gcd(gcd_out, p_1, q_1); // gcd of p_1 and q_1 stored in gcd_out
    mpz_mul(p1_q1, p_1, q_1); // p_1 x q_1 stored in p1_q1
    mpz_fdiv_q(lcm_out, p1_q1, gcd_out); // lcm_out(lambda n) = (p_1 x q_1) / gcd(p_1, q_1)
    mod_inverse(d, e, lcm_out);
    mpz_clears(p_1, q_1, gcd_out, p1_q1, lcm_out, NULL);
    return;
}

void rsa_write_priv(mpz_t n, mpz_t d, FILE *pvfile) {
    gmp_fprintf(pvfile, "%Zx\n", n);
    gmp_fprintf(pvfile, "%Zx\n", d);
}

void rsa_read_priv(mpz_t n, mpz_t d, FILE *pvfile) {
    gmp_fscanf(pvfile, "%Zx\n", n);
    gmp_fscanf(pvfile, "%ZX\n", d);
}

void rsa_encrypt(mpz_t c, mpz_t m, mpz_t e, mpz_t n) {
    pow_mod(c, m, e, n);
    return;
}

void rsa_encrypt_file(FILE *infile, FILE *outfile, mpz_t n, mpz_t e) {
    mpz_t m, encrypt;
    mpz_inits(m, encrypt, NULL);
    size_t j = 0;
    size_t logn = mpz_sizeinbase(n, 2); // logn = log2n
    logn = logn - 1; //mpz_sub_ui(logn, logn, 1);
    size_t k = logn / 8; //mpz_fdiv_q_ui(k, logn, 8);
    uint8_t *block
        = (uint8_t *) calloc(k, sizeof(uint8_t)); // allocate an array that can hold k bytes
    block[0] = 0xFF; // set the 0th byte of the block to 0xFF
    while (!feof(infile)) {
        j = fread(&block[1], sizeof(uint8_t), k - 1, infile); // read the file f read
        mpz_import(
            m, j + 1, 1, sizeof(uint8_t), 1, 0, block); // mpz import turn the bytes into number
        rsa_encrypt(encrypt, m, e, n); // encrypt use rsa encrypt
        gmp_fprintf(outfile, "%Zx\n", encrypt); // gmp fprintf write block (number) to output
    }
    mpz_clears(m, encrypt, NULL);
    free(block);
}

void rsa_decrypt(mpz_t m, mpz_t c, mpz_t d, mpz_t n) {
    pow_mod(m, c, d, n);
    return;
}

void rsa_decrypt_file(FILE *infile, FILE *outfile, mpz_t n, mpz_t d) {
    mpz_t c, decrypt;
    mpz_inits(c, decrypt, NULL);
    size_t j;
    size_t logn = mpz_sizeinbase(n, 2); // logn = log2n
    logn = logn - 1;
    size_t k = logn / 8;
    uint8_t *block
        = (uint8_t *) calloc(k, sizeof(uint8_t)); // allocate an array that can hold k bytes
    block[0] = 0xFF; // set the 0th byte of the block to 0xFF
    while (!feof(infile)) {
        gmp_fscanf(infile, "%Zx\n", c); // gmp_fscanf
        rsa_decrypt(decrypt, c, d, n); // decrypt using rsa decrypt
        mpz_export(block, &j, 1, sizeof(uint8_t), 1, 0, decrypt); // mpz_export to turn the
        fwrite(&block[1], sizeof(uint8_t), j - 1,
            outfile); // write the file fwrite?? IDK ABOUT THIS ONE
    }
    mpz_clears(c, decrypt, NULL);
    free(block);
}

void rsa_sign(mpz_t s, mpz_t m, mpz_t d, mpz_t n) {
    pow_mod(s, m, d, n);
    return;
}

bool rsa_verify(mpz_t m, mpz_t s, mpz_t e, mpz_t n) {
    mpz_t t;
    mpz_init(t);
    pow_mod(t, s, e, n);
    if (mpz_cmp(t, m) == 0) {
        mpz_clears(t, NULL);
        return true;
    } else {
        mpz_clears(t, NULL);
        return false;
    }
}
