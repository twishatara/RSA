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
    bool e_i = false;
    bool e_o = false;
    bool e_n = false;
    bool e_v = false;
    bool e_h = false;
    FILE *infile = stdin;
    FILE *outfile = stdout;
    char *pb = "rsa.pub";
    mpz_t p, q, n, e, d, s, m;
    mpz_inits(p, q, n, e, d, s, m, NULL);

    while ((opt = getopt(argc, argv, "i:o:n:vh")) != -1) {
        switch (opt) {
        case 'i':
            e_i = true;
            infile = fopen(optarg, "r");
            break;
        case 'o':
            e_o = true;
            outfile = fopen(optarg, "w");
            break;
        case 'n':
            e_n = true;
            pb = optarg;
            break;
        case 'v': e_v = true; break;
        case 'h':
            e_h = true;
            printf("// usage: ./encrypt [- "
                   "]\n");
            printf("// -i : specifies the input file to encrypt (default: stdin)\n");
            printf("// -o : specifies the output file to encrypt (default: stdout)\n");
            printf("// -n : specifies the file containing the public key\n");
            printf("// -v : enables verbose output\n");
            printf("// -h : displays the program's usage and synopsis\n");
            break;
        }
    }

    FILE *pubfile = fopen(pb, "r"); // open public key files

    if (pubfile == NULL) {
        printf("ERROR: file could not be opened\n");
        mpz_clears(p, q, n, e, d, s, m, NULL);
        fclose(pubfile);
        return 1;
    }
    char *user = getenv("USER"); // get the current user's name -> use getenv()???
    rsa_read_pub(n, e, s, user, pubfile);
    if (e_v) {
        gmp_printf("user = %s\n", user);
        size_t bs = mpz_sizeinbase(s, 2);
        gmp_printf("s (%zu bits) =  %Zd\n", bs, s);
        size_t ns = mpz_sizeinbase(n, 2);
        gmp_printf("n (%zu bits) =  %Zd\n", ns, n);
        size_t es = mpz_sizeinbase(e, 2);
        gmp_printf("e (%zu bits) =  %Zd\n", es, e);
    }
    mpz_set_str(m, user, 62);
    if (rsa_verify(m, s, e, n) == false) {
        printf("Error: Signature could not be verified\n");
        fclose(pubfile);
        mpz_clears(p, q, n, e, d, s, m, NULL);
        return 1;
    }

    rsa_encrypt_file(infile, outfile, n, e);
    fclose(infile);
    fclose(outfile);
    fclose(pubfile);
    mpz_clears(p, q, n, e, d, s, m, NULL);
    return 0;
}
