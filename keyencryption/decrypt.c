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
    bool d_i = false;
    bool d_o = false;
    bool d_n = false;
    bool d_v = false;
    bool d_h = false;
    FILE *infile = stdin;
    FILE *outfile = stdout;
    char *pv = "rsa.priv";
    mpz_t p, q, n, e, d, s, m;
    mpz_inits(p, q, n, e, d, s, m, NULL);

    while ((opt = getopt(argc, argv, "i:o:n:vh")) != -1) {
        switch (opt) {
        case 'i':
            d_i = true;
            infile = fopen(optarg, "r");
            break;
        case 'o':
            d_o = true;
            outfile = fopen(optarg, "w");
            break;
        case 'n':
            d_n = true;
            pv = optarg;
            break;
        case 'v': d_v = true; break;
        case 'h':
            d_h = true;
            printf("// usage: ./decrypt [- "
                   "]\n");
            printf("// -i : specifies the input file to decrypt (default: stdin)\n");
            printf("// -o : specifies the output file to decrypt (default: stdout)\n");
            printf("// -n : specifies the file containing the private key\n");
            printf("// -v : enables verbose output\n");
            printf("// -h : displays the program's usage and synopsis\n");
            break;
        }
    }

    FILE *privfile = fopen(pv, "r"); // open public key files

    if (privfile == NULL) {
        printf("ERROR: file could not be opened\n");
        mpz_clears(p, q, n, e, d, s, m, NULL);
        fclose(privfile);
        return 1;
    }
    char *user = getenv("USER"); // get the current user's name -> use getenv()???
    rsa_read_priv(n, d, privfile);
    if (d_v) {
        gmp_printf("user = %s\n", user);
        size_t ns = mpz_sizeinbase(n, 2);
        gmp_printf("n (%zu bits) =  %Zd\n", ns, n);
        size_t es = mpz_sizeinbase(e, 2);
        gmp_printf("e (%zu bits) =  %Zd\n", es, e);
        return 0;
    }

    rsa_decrypt_file(infile, outfile, n, d);
    fclose(infile);
    fclose(outfile);
    fclose(privfile);
    mpz_clears(p, q, n, e, d, s, m, NULL);
    return 0;
}
