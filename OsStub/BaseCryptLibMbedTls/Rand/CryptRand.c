/** @file
  Pseudorandom Number Generator Wrapper Implementation over OpenSSL.

Copyright (c) 2010 - 2013, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include "Hal/Base.h"
#include "InternalCryptLib.h"
#include "mbedtls/ctr_drbg.h"
int rand ();

mbedtls_ctr_drbg_context gDrbgContext;
static BOOLEAN           mInitFlag = FALSE;



/**
  MbedTls entropy callback function implement for OsTest

  This function generate entropy for the mbedtls pseudorandom number generator.


  @param[in]  EntropyContext      Pointer to entropy generate
                                  If NULL, default entropy seed is used.
  @param[in]  Buf                 entropy buf.
  @param[in]  BufSize             Size of entropy buf.

  @retval TRUE   Pseudorandom number generator has enough entropy for random generation.
  @retval FALSE  Pseudorandom number generator does not have enough entropy for random generation.

**/
int OsTestEntropyFnc(
   void * EntropyContext,
   unsigned char * Buf,
   size_t BufSize)
{
  UINTN Count = BufSize/4;
  UINTN Final = BufSize%4;
  UINTN Index = 0;
  UINT32 Data32;

  for (Index = 0; Index < Count; Index++) {
    *(UINT32 *)(Buf + Index * 4) = rand();
  }
  if (Final != 0) {
    Data32 = rand();
    CopyMem (Buf + Count * 4, &Data32, Final);
  }

  return 0;
}

/**
  Sets up the seed value for the pseudorandom number generator.

  This function sets up the seed value for the pseudorandom number generator.
  If Seed is not NULL, then the seed passed in is used.
  If Seed is NULL, then default seed is used.

  @param[in]  Seed      Pointer to seed value.
                        If NULL, default seed is used.
  @param[in]  SeedSize  Size of seed value.
                        If Seed is NULL, this parameter is ignored.

  @retval TRUE   Pseudorandom number generator has enough entropy for random generation.
  @retval FALSE  Pseudorandom number generator does not have enough entropy for random generation.

**/
BOOLEAN
EFIAPI
RandomSeed (
  IN  CONST  UINT8  *Seed  OPTIONAL,
  IN  UINTN         SeedSize
  )
{
  int ret = -1;
  if (mInitFlag == FALSE) {
    mbedtls_ctr_drbg_init(&gDrbgContext);
    if (Seed == NULL) {
      ret = mbedtls_ctr_drbg_seed(&gDrbgContext, OsTestEntropyFnc, NULL, NULL, 0);
    } else {
      // TBD
    }
  }

  if (ret == 0) {
      mInitFlag = TRUE;
  }

  return ret == 0;
}

/**
  Generates a pseudorandom byte stream of the specified size.

  If Output is NULL, then return FALSE.

  @param[out]  Output  Pointer to buffer to receive random value.
  @param[in]   Size    Size of random bytes to generate.

  @retval TRUE   Pseudorandom byte stream generated successfully.
  @retval FALSE  Pseudorandom number generator fails to generate due to lack of entropy.

**/
BOOLEAN
EFIAPI
RandomBytes (
  OUT  UINT8  *Output,
  IN   UINTN  Size
  )
{
  int ret = -1;
  ret = mbedtls_ctr_drbg_random(&gDrbgContext, Output, Size);
  return ret == 0;
}

int myrand( void *rng_state, unsigned char *output, size_t len )
{
  RandomBytes (output, len);

  return 0;
}
