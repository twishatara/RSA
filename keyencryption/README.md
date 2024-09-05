# Assignment 5 : Public Key Cryptography

## Short Description: 
        This assignment has 3 main programs, a key generator, an encryptor, and a decryptor. The program implements its own number theory library and rsa library to run these programs.

## Build 
        To build the key generator manually, type "clang -Wall -Wextra -Werror -Wpedantic -o keygen keygen.c" on the command line.
        To build the encryptor manually, type "clang -Wall -Wextra -Werror -Wpedantic -o encrypt encrypt.c" on the command line.
        To build the decryptor manually, type "clang -Wall -Wextra -Werror -Wpedantic -o decrypt decrypt.c" on the command line.
        Alternatively, type "make" on the command line, using the Makefile provided.

## Running 
        To run the key generator program on the command line follow this structure for what to type on the command line, "./keygen -b <bits> -i <iterations> -n <public key file> -d <private key file> -s <seed>
        To get the usage statements and synopsis type "./keygen -h" on the command line.
        To run the encryptor program on the command line follow this structure for what to type on the command line, "./encrypt -i <input file> -o <output file> -n <public key file>
        To get the usage statements and synopsis type "./encrypt -h" on the command line.
        To run the decryptor program on the command line follow this structure for what to type on the command line, "./decrypt -i <input file> -o <output file> -n <private key file>
        To get the usage statements and synopsis type "./decrypt -h" on the command line.

## Cleaning 
        make clean
