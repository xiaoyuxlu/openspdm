/** @file
  Application for Elliptic Curve Key Retrieving (from PEM and X509) & Signature Validation.

Copyright (c) 2020, Intel Corporation. All rights reserved.<BR>
This program and the accompanying materials
are licensed and made available under the terms and conditions of the BSD License
which accompanies this distribution.  The full text of the license may be found at
http://opensource.org/licenses/bsd-license.php

THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include "Cryptest.h"

//
// Root CA X509 Certificate for X509 Verification Routine (Generated by OpenSSL utility).
//
GLOBAL_REMOVE_IF_UNREFERENCED CONST UINT8 EccTestRootCer[]= {
0x30, 0x82, 0x01, 0xd2, 0x30, 0x82, 0x01, 0x77, 0xa0, 0x03, 0x02, 0x01, 0x02, 0x02, 0x09, 0x00, 
0xcc, 0x10, 0x45, 0x50, 0xaf, 0x50, 0x1b, 0xe2, 0x30, 0x0a, 0x06, 0x08, 0x2a, 0x86, 0x48, 0xce, 
0x3d, 0x04, 0x03, 0x02, 0x30, 0x45, 0x31, 0x0b, 0x30, 0x09, 0x06, 0x03, 0x55, 0x04, 0x06, 0x13, 
0x02, 0x41, 0x55, 0x31, 0x13, 0x30, 0x11, 0x06, 0x03, 0x55, 0x04, 0x08, 0x0c, 0x0a, 0x53, 0x6f, 
0x6d, 0x65, 0x2d, 0x53, 0x74, 0x61, 0x74, 0x65, 0x31, 0x21, 0x30, 0x1f, 0x06, 0x03, 0x55, 0x04, 
0x0a, 0x0c, 0x18, 0x49, 0x6e, 0x74, 0x65, 0x72, 0x6e, 0x65, 0x74, 0x20, 0x57, 0x69, 0x64, 0x67, 
0x69, 0x74, 0x73, 0x20, 0x50, 0x74, 0x79, 0x20, 0x4c, 0x74, 0x64, 0x30, 0x1e, 0x17, 0x0d, 0x32, 
0x30, 0x30, 0x32, 0x32, 0x37, 0x31, 0x32, 0x30, 0x32, 0x31, 0x30, 0x5a, 0x17, 0x0d, 0x33, 0x30, 
0x30, 0x32, 0x32, 0x34, 0x31, 0x32, 0x30, 0x32, 0x31, 0x30, 0x5a, 0x30, 0x45, 0x31, 0x0b, 0x30, 
0x09, 0x06, 0x03, 0x55, 0x04, 0x06, 0x13, 0x02, 0x41, 0x55, 0x31, 0x13, 0x30, 0x11, 0x06, 0x03, 
0x55, 0x04, 0x08, 0x0c, 0x0a, 0x53, 0x6f, 0x6d, 0x65, 0x2d, 0x53, 0x74, 0x61, 0x74, 0x65, 0x31, 
0x21, 0x30, 0x1f, 0x06, 0x03, 0x55, 0x04, 0x0a, 0x0c, 0x18, 0x49, 0x6e, 0x74, 0x65, 0x72, 0x6e, 
0x65, 0x74, 0x20, 0x57, 0x69, 0x64, 0x67, 0x69, 0x74, 0x73, 0x20, 0x50, 0x74, 0x79, 0x20, 0x4c, 
0x74, 0x64, 0x30, 0x59, 0x30, 0x13, 0x06, 0x07, 0x2a, 0x86, 0x48, 0xce, 0x3d, 0x02, 0x01, 0x06, 
0x08, 0x2a, 0x86, 0x48, 0xce, 0x3d, 0x03, 0x01, 0x07, 0x03, 0x42, 0x00, 0x04, 0x11, 0xa4, 0x06, 
0x65, 0xb6, 0x79, 0x6e, 0x72, 0xb6, 0xd8, 0x09, 0x84, 0x92, 0x86, 0x11, 0x09, 0xde, 0xea, 0xd0, 
0x0c, 0x60, 0xf1, 0x8a, 0xff, 0x7c, 0xde, 0xce, 0xec, 0x07, 0xba, 0xa5, 0xb8, 0xd5, 0x17, 0xe5, 
0x62, 0x33, 0x2d, 0x88, 0xb1, 0x9a, 0xe6, 0xf3, 0x09, 0x43, 0x0e, 0xa9, 0xf7, 0x3c, 0xe9, 0x20, 
0xba, 0xbd, 0xb1, 0x3c, 0x03, 0x89, 0x1e, 0x2a, 0xff, 0x6e, 0x08, 0xff, 0x2e, 0xa3, 0x50, 0x30, 
0x4e, 0x30, 0x1d, 0x06, 0x03, 0x55, 0x1d, 0x0e, 0x04, 0x16, 0x04, 0x14, 0x62, 0xe6, 0xd9, 0xa0, 
0xee, 0x38, 0x18, 0x83, 0xfa, 0xe3, 0xed, 0x44, 0xa4, 0x37, 0xfd, 0x4a, 0x04, 0xdf, 0xe1, 0xd5, 
0x30, 0x1f, 0x06, 0x03, 0x55, 0x1d, 0x23, 0x04, 0x18, 0x30, 0x16, 0x80, 0x14, 0x62, 0xe6, 0xd9, 
0xa0, 0xee, 0x38, 0x18, 0x83, 0xfa, 0xe3, 0xed, 0x44, 0xa4, 0x37, 0xfd, 0x4a, 0x04, 0xdf, 0xe1, 
0xd5, 0x30, 0x0c, 0x06, 0x03, 0x55, 0x1d, 0x13, 0x04, 0x05, 0x30, 0x03, 0x01, 0x01, 0xff, 0x30, 
0x0a, 0x06, 0x08, 0x2a, 0x86, 0x48, 0xce, 0x3d, 0x04, 0x03, 0x02, 0x03, 0x49, 0x00, 0x30, 0x46, 
0x02, 0x21, 0x00, 0x99, 0x2f, 0x43, 0xeb, 0xdc, 0x4e, 0x53, 0xc7, 0xc1, 0xbd, 0xed, 0x95, 0xdc, 
0xae, 0xd3, 0x75, 0xfa, 0xc4, 0xf7, 0xa4, 0x61, 0x00, 0x57, 0xce, 0xf3, 0xe0, 0x23, 0xf6, 0xf0, 
0x41, 0x6f, 0xb5, 0x02, 0x21, 0x00, 0xf3, 0x97, 0x11, 0x06, 0x61, 0x10, 0xc7, 0x35, 0xe9, 0xf8, 
0x3b, 0x59, 0xec, 0xf5, 0x51, 0xa0, 0xa6, 0x64, 0x6e, 0xe1, 0x44, 0xc7, 0xe1, 0xa2, 0xce, 0x90, 
0x7f, 0xae, 0xad, 0xf4, 0xa9, 0xfa, };


//
// PEM Key data for EC Private Key Retrieving.
// (Generated by OpenSSL utility).
//
GLOBAL_REMOVE_IF_UNREFERENCED CONST UINT8 EccTestPemKey[]= {
0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x42, 0x45, 0x47, 0x49, 0x4e, 0x20, 0x45, 0x43, 0x20, 0x50, 0x41, 
0x52, 0x41, 0x4d, 0x45, 0x54, 0x45, 0x52, 0x53, 0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x0a, 0x42, 0x67, 
0x67, 0x71, 0x68, 0x6b, 0x6a, 0x4f, 0x50, 0x51, 0x4d, 0x42, 0x42, 0x77, 0x3d, 0x3d, 0x0a, 0x2d, 
0x2d, 0x2d, 0x2d, 0x2d, 0x45, 0x4e, 0x44, 0x20, 0x45, 0x43, 0x20, 0x50, 0x41, 0x52, 0x41, 0x4d, 
0x45, 0x54, 0x45, 0x52, 0x53, 0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x0a, 0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 
0x42, 0x45, 0x47, 0x49, 0x4e, 0x20, 0x45, 0x43, 0x20, 0x50, 0x52, 0x49, 0x56, 0x41, 0x54, 0x45, 
0x20, 0x4b, 0x45, 0x59, 0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x0a, 0x4d, 0x48, 0x63, 0x43, 0x41, 0x51, 
0x45, 0x45, 0x49, 0x4d, 0x4a, 0x54, 0x69, 0x75, 0x34, 0x56, 0x54, 0x54, 0x57, 0x68, 0x78, 0x4b, 
0x62, 0x51, 0x65, 0x78, 0x6e, 0x30, 0x43, 0x58, 0x41, 0x77, 0x33, 0x44, 0x57, 0x6b, 0x6f, 0x78, 
0x79, 0x77, 0x6b, 0x7a, 0x46, 0x50, 0x62, 0x32, 0x48, 0x68, 0x5a, 0x6e, 0x5a, 0x52, 0x6f, 0x41, 
0x6f, 0x47, 0x43, 0x43, 0x71, 0x47, 0x53, 0x4d, 0x34, 0x39, 0x0a, 0x41, 0x77, 0x45, 0x48, 0x6f, 
0x55, 0x51, 0x44, 0x51, 0x67, 0x41, 0x45, 0x45, 0x61, 0x51, 0x47, 0x5a, 0x62, 0x5a, 0x35, 0x62, 
0x6e, 0x4b, 0x32, 0x32, 0x41, 0x6d, 0x45, 0x6b, 0x6f, 0x59, 0x52, 0x43, 0x64, 0x37, 0x71, 0x30, 
0x41, 0x78, 0x67, 0x38, 0x59, 0x72, 0x2f, 0x66, 0x4e, 0x37, 0x4f, 0x37, 0x41, 0x65, 0x36, 0x70, 
0x62, 0x6a, 0x56, 0x46, 0x2b, 0x56, 0x69, 0x4d, 0x79, 0x32, 0x49, 0x0a, 0x73, 0x5a, 0x72, 0x6d, 
0x38, 0x77, 0x6c, 0x44, 0x44, 0x71, 0x6e, 0x33, 0x50, 0x4f, 0x6b, 0x67, 0x75, 0x72, 0x32, 0x78, 
0x50, 0x41, 0x4f, 0x4a, 0x48, 0x69, 0x72, 0x2f, 0x62, 0x67, 0x6a, 0x2f, 0x4c, 0x67, 0x3d, 0x3d, 
0x0a, 0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x45, 0x4e, 0x44, 0x20, 0x45, 0x43, 0x20, 0x50, 0x52, 0x49, 
0x56, 0x41, 0x54, 0x45, 0x20, 0x4b, 0x45, 0x59, 0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x0a, };

//
// Payload for PKCS#7 Signing & Verification Validation.
//
GLOBAL_REMOVE_IF_UNREFERENCED CONST CHAR8 *EcPayload = "Payload Data for PKCS#7 EC Signing";

/**
  Validate UEFI-OpenSSL EC Key Retrieving (from PEM & X509) & Signature Interfaces.

  @retval  EFI_SUCCESS  Validation succeeded.
  @retval  EFI_ABORTED  Validation failed.

**/
EFI_STATUS
ValidateCryptEc2 (
  VOID
  )
{
  BOOLEAN Status;
  VOID    *EcPrivKey;
  VOID    *EcPubKey;
  UINT8   HashValue[SHA256_DIGEST_SIZE];
  UINTN   HashSize;
  UINT8   Signature[512]; // 0x48/72, 0x68/104, 0x8A/138
  UINTN   SigSize;

  Print (L"\nUEFI-OpenSSL EC Key Retrieving Testing: ");

  //
  // Retrieve EC private key from PEM data.
  //
  Print (L"\n- Retrieve EC Private Key for PEM ...");
  Status = EcGetPrivateKeyFromPem (EccTestPemKey, sizeof (EccTestPemKey), NULL, &EcPrivKey);
  if (!Status) {
    Print (L"[Fail]");
    return EFI_ABORTED;
  } else {
    Print (L"[Pass]");
  }

  //
  // Retrieve EC public key from X509 Certificate.
  //
  Print (L"\n- Retrieve EC Public Key from X509 ... ");
  Status    = EcGetPublicKeyFromX509 (EccTestRootCer, sizeof (EccTestRootCer), &EcPubKey);
  if (!Status) {
    Print (L"[Fail]");
    return EFI_ABORTED;
  } else {
    Print (L"[Pass]");
  }
  
  //
  // Verify EC-DSA
  //
  HashSize = sizeof(HashValue);
  SigSize = sizeof(Signature);
  Print (L"\n- EC-DSA Signing ... ");
  Status  = EcDsaSign (EcPrivKey, HashValue, HashSize, Signature, &SigSize);
  if (!Status) {
    Print (L"[Fail]");
    return EFI_ABORTED;
  } else {
    Print (L"[Pass]");
  }

  Print (L"\n- EC-DSA Verification ... ");
  Status = EcDsaVerify (EcPubKey, HashValue, HashSize, Signature, SigSize);
  if (!Status) {
    Print (L"[Fail]");
    return EFI_ABORTED;
  } else {
    Print (L"[Pass]\n");
  }
  
  return EFI_SUCCESS;
}

/**
  Validate UEFI-OpenSSL PKCS#7 Signing & Verification Interfaces for EC.

  @retval  EFI_SUCCESS  Validation succeeded.
  @retval  EFI_ABORTED  Validation failed.

**/
EFI_STATUS
ValidateCryptPkcs7Ec (
  VOID
  )
{
  BOOLEAN  Status;
  UINT8    *P7SignedData;
  UINTN    P7SignedDataSize;
  UINT8    *SignCert;

  P7SignedData = NULL;
  SignCert     = NULL;

  Print (L"\nUEFI-OpenSSL PKCS#7 Signing & Verification for EC Testing: ");

  Print (L"\n- Create PKCS#7 certificate ...");

  //
  // Construct Signer Certificate from RAW data.
  //
  Status = X509ConstructCertificate (EccTestRootCer, sizeof (EccTestRootCer), (UINT8 **) &SignCert);
  if (!Status || SignCert == NULL) {
    Print (L"[Fail]");
    goto _Exit;
  } else {
    Print (L"[Pass]");
  }
  
  Print (L"\n- Create PKCS#7 signedData ...");

  //
  // Create PKCS#7 signedData on Payload. 
  // Note: Caller should release P7SignedData manually.
  //
  Status = Pkcs7Sign (
             EccTestPemKey,
             sizeof (EccTestPemKey),
             "",
             (UINT8 *) EcPayload,
             AsciiStrLen (EcPayload),
             SignCert,
             NULL,
             &P7SignedData,
             &P7SignedDataSize
             );
  if (!Status || P7SignedDataSize == 0) {
    Print (L"[Fail]");
    goto _Exit;
  } else {
    Print (L"[Pass]");
  }

  Print (L"\n- Verify PKCS#7 signedData ...");

  Status = Pkcs7Verify (
             P7SignedData,
             P7SignedDataSize,
             EccTestRootCer,
             sizeof (EccTestRootCer),
             (UINT8 *) EcPayload,
             AsciiStrLen (EcPayload)
             );
  if (!Status) {
    Print (L"[Fail]");
  } else {
    Print (L"[Pass]");
  }

_Exit:
  if (P7SignedData != NULL) {
    FreePool (P7SignedData);
  }
  if (SignCert != NULL) {
    X509Free (SignCert);
  }

  Print (L"\n");
  return EFI_SUCCESS;
}
