#include <stdlib.h>
#include <stdio.h>
#include "randstate.h"
#include "numtheory.h"
#include "rsa.h"
#include <time.h>
#include <gmp.h>
#include <unistd.h>
#include <sys/stat.h>

// Please note that this code is based off of the provided psuedo code in the assignment doc, along
// with Eugenes section on 2_4_22 and the coding standards doc found on canvas. Additionally I had
// heavy psuedo code collaboration with my sister Krisha Sharma (krvsharm).

int main(int argc, char **argv) {
    int opt = 0;
    bool k_b = false;
    bool k_i = false;
    bool k_n = false;
    bool k_d = false;
    bool k_s = false;
    bool k_v = false;
    bool k_h = false;
    uint64_t seed = time(NULL);
    uint64_t nbits = 256;
    uint64_t iters = 50;
    char *pb = "rsa.pub";
    char *pv = "rsa.priv";
    mpz_t p, q, n, e, d, s, m;
    mpz_inits(p, q, n, e, d, s, m, NULL);

    while ((opt = getopt(argc, argv, "b:i:n:d:s:vh")) != -1) {
        switch (opt) {
        case 'b':
            k_b = true;
            nbits = strtod(optarg, NULL);
            break;

        case 'i':
            k_i = true;
            iters = strtod(optarg, NULL);
            break;

        case 'n':
            k_n = true;
            pb = optarg;
            break;

        case 'd':
            k_d = true;
            pv = optarg;
            break;

        case 's':
            k_s = true;
            seed = strtod(optarg, NULL);
            break;

        case 'v': k_v = true; break;

        case 'h':
            k_h = true;
            printf("// usage: ./keygen [- "
                   "]\n");
            printf("// -b : specifies the minimum nits needed for the public modulus n\n");
            printf("// -i : specifies the number of Miller-Rabin iterations for testing primes "
                   "(default 50)\n");
            printf("// -n : specifies the public key file\n");
            printf("// -d : specifies the private key file\n");
            printf("// -s : specifies the random seed for the random state initialization "
                   "(default: the seconds since the UNIX epoch)\n");
            printf("// -v : enables verbose output\n");
            printf("// -h : displays the program's usage and synopsis\n");
            break;
        }
    }

    FILE *pubfile = fopen(pb, "w"); // open public key files
    FILE *privfile = fopen(pv, "w"); // open private keys fles

    if (pubfile == NULL) {
        printf("ERROR: file could not be opened\n");
        fclose(pubfile);
        fclose(privfile);
        return 1;
    }

    if (privfile == NULL) {
        printf("ERROR: file could not be opened\n");
        fclose(pubfile);
        fclose(privfile);
        return 1;
    }

    fchmod(fileno(privfile), 0660); // Credit: Eugene OH 2/16/22

    randstate_init(seed);
    rsa_make_pub(p, q, n, e, nbits, iters); // generate the public key
    rsa_make_priv(d, e, p, q); // generate the private key
    char *username = getenv("USER"); // get the current user's name -> use getenv()???
    mpz_set_str(m, username, 62); // convert the username into mpz_t (base 62)
    rsa_sign(s, m, d, n);
    rsa_write_pub(n, e, s, username, pubfile);
    rsa_write_priv(n, d, privfile);

    if (k_v) {
        gmp_printf("user = %s\n", username);
        size_t bs = mpz_sizeinbase(s, 2);
        gmp_printf("s (%zu bits) =  %Zd\n", bs, s);
        size_t ps = mpz_sizeinbase(p, 2);
        gmp_printf("p (%zu bits) =  %Zd\n", ps, p);
        size_t qs = mpz_sizeinbase(q, 2);
        gmp_printf("q (%zu bits) =  %Zd\n", qs, q);
        size_t ns = mpz_sizeinbase(n, 2);
        gmp_printf("n (%zu bits) =  %Zd\n", ns, n);
        size_t es = mpz_sizeinbase(e, 2);
        gmp_printf("e (%zu bits) =  %Zd\n", es, e);
        size_t ds = mpz_sizeinbase(d, 2);
        gmp_printf("d (%zu bits) =  %Zd\n", ds, d);
    }

    fclose(pubfile);
    fclose(privfile);
    randstate_clear();
    mpz_clears(p, q, n, e, d, s, m, NULL);
    return 0;
}
