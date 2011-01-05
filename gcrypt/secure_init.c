/*
 * secure_init.c
 * Kyle Isom <coder@kyleisom.net>
 *
 * gcrypt init code and tests.
 *
 */

#include "secure_init.h"

char *key;

int main( ) {
    if (crypto_init()) {
        return EXIT_FAILURE;
    }

    if (crypto_genkey()) {
        return EXIT_FAILURE;
    }

    return crypto_shutdown();
}


int crypto_init( ) {
    printf("[+] performing basic initialisation...\n");
    printf("[+] ...\n");
    if (!gcry_check_version(NULL)) {
        printf("could not initialize crypto libraries!\n");
        exit(2);
    }

    /* suspend warnings about secure memory - still have more initialisation 
     * to do before enabling secure memory.
     */
    printf("[+] suspend secure memory warnings...\n");
    gcry_control(GCRYCTL_SUSPEND_SECMEM_WARN);

    /* set up secure PRNG */
    printf("[+] initialise secure PRNG...\n");
    gcry_control(GCRYCTL_USE_SECURE_RNDPOOL);
    
    /* initialise secure memory */
    printf("[+] initialise secure memory...\n");
    gcry_control(GCRYCTL_INIT_SECMEM, SECMEM_SZ, 0);

    /* initialisation complete */
    printf("[+] finished!\n");
    gcry_control(GCRYCTL_INITIALIZATION_FINISHED, 0);


    return EXIT_SUCCESS;
}

int crypto_shutdown( ) {
    if (! gcry_control(GCRYCTL_ANY_INITIALIZATION_P)) {
        return EXIT_SUCCESS;
    }
    
    /* reclaim the key */
    printf("[+] freeing key...\n");
    gcry_free(key);
    key = NULL;

    /* tell gcrypt to zerioise and disable secure memory */
    printf("[+] zeroising secure memory...\n");
    gcry_control(GCRYCTL_TERM_SECMEM); 
    gcry_control(GCRYCTL_DISABLE_SECMEM);

    return EXIT_SUCCESS;
}


int crypto_encrypt_file( FILE *inf, FILE *outf ) {
    if (!gcry_control(GCRYCTL_INITIALIZATION_FINISHED_P)) {
        printf("crypto library has not been initialised!\n");
        return EXIT_FAILURE;
    }


    return EXIT_SUCCESS;

}


int crypto_genkey( ) {
    /* sanity check to ensure gcrypt has been initialised */
    if (!gcry_control(GCRYCTL_INITIALIZATION_FINISHED_P)) {
        printf("crypto library has not been initialised!\n");
        return EXIT_FAILURE;
    }
    
    printf("[+] generating key..."); 
    fflush(stdout);
    /* generate key in secure memory */
    key = (char *) gcry_random_bytes_secure(KEYSIZE * sizeof(char), 
                                            GCRY_VERY_STRONG_RANDOM);
    if (NULL == key) {
        printf("\t\t\t\tFAILED!\n");
        return EXIT_FAILURE;
    }

    printf("\t\t\t\tOK!\n");
    return EXIT_SUCCESS;
}

int crypto_loadkey( ) {
    /* determine if a keyfile is present */
    if (-1 == access( KEYFILE, R_OK )) {
        if (! crypto_genkey() ) {
            return EXIT_FAILURE;
        }
    } else {
        /* allocate secure memory for key */
        char *key   = gcry_calloc_secure(KEYSIZE, sizeof(char));
        if (NULL == key) {
            printf("error allocating secure memory for key!\n");
            return EXIT_FAILURE;
        }

        /* open key file */
        FILE *kd    = fopen( KEYFILE, O_RDONLY );
        if (KEYSIZE < fread( key, sizeof(char), KEYSIZE, kd )) {
            printf("key size mismatch, generating new key...\n");
            if (! crypto_genkey() ) {
                return EXIT_FAILURE;
            }
        }
        if (fclose(kd)) {
            printf("error release key file!\n");
            return EXIT_FAILURE;
        }

    }

    return EXIT_SUCCESS;
}

