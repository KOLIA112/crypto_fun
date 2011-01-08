/**************************************************************************
 * metakey.h                                                              *
 * 4096R/B7B720D6 "Kyle Isom <coder@kyleisom.net>"                        *
 * 2011-01-08                                                             *
 *                                                                        *
 * defines functions on metakeys - symmetric keys with attached           *
 *  information about the algorithm and keysize.                          *
 **************************************************************************/

#ifndef __METAKEY_H
#define __METAKEY_H

#include "config.h"

#include <gcrypt.h>
#include "crypto.h"

/**************************************************************************/
/*                  note on automatic key generation                      */
/**************************************************************************/
/* 
 * automatic key generation is determined by the state of the static int
 * generate_keys, and can be interacted with via the set_autogen, 
 * unset_autogen, and autogen_status functions. 
 *
 * automatic key generation takes place under the following conditions:
 *  a key should have been read from keyfile, but either:
 *      1. the file could not be opened
 *      2. the file could be opened but the key read was the wrong size
 *
 * the following crypto_key_return values cover the status of automatic
 * key generation; see crypto.h for their meanings.
 *      KEYGEN, SIZE_MISMATCH, KEYGEN_ERR
 */


/**************************************************************************/
/*                         metakey functions                              */
/**************************************************************************/

/* crypto_genkey: generate a new symmetric key; keys are non-null-terminated
 *                buffers containing unsigned chars.
 *      arguments: the metakey_t containing the key to be generated and
 *                a size_t specifying the key size to generate.
 *      returns: a crypto_key_return_t with the standard error codes 
 *                defined in crypto.h.
 */
crypto_key_return_t crypto_genkey( metakey_t, size_t );

/* crypto_loadkey: load a symmetric key from a file. if automatic key
 *                 generation is enabled and there is an error reading the
 *                 key from the file, a key will be generated.
 *      arguments: const char * containing the filename, a metakey_t 
 *                 containing the key to loaded, and a size_t specifying
 *                 the size of the key to be read (and possibly generated).
 *      returns: a crypto_key_return_t with the standard error codes defined
 *                 in crypto.h. of particular note is that if the
 *                function returns with INCONSISTENT_STATE, no key was 
 *                generated or loaded, and the metakey_t is not initialised
 */
crypto_key_return_t crypto_loadkey( const char *, metakey_t, size_t );

/* crypto_dumpkey: write a symmetric key to a file. automatic key generation
 *                 is not used in the event of error, as it is assumed the
 *                 key specified is already valid.
 *      arguments: const char * containaing the file to be written (which
 *                 will be overwritten if it already exists), and a 
 *                 metakey_t containing the key to be written. the keysize
 *                 in the metakey_t will be used to define the size of the
 *                 key to be written.
 */
crypto_key_return_t crypto_dumpkey( const char *, metakey_t );
crypto_key_return_t crypto_zerokey( metakey_t );

/********************************/
/* keyring functions            */
/********************************/
crypto_key_return_t crypto_zerokeyring( void );

/********************************/
/* miscellaneous functions      */
/********************************/
crypto_key_return_t set_autogen( void );
crypto_key_return_t unset_autogen( void );
int crypto_autokeygen_status( void );
crypto_key_return_t crypto_wipe_keyfile( const char *, int );



#endif