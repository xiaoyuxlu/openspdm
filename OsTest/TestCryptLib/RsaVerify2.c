/** @file
  Application for RSA Key Retrieving (from PEM and X509) & Signature Validation.

Copyright (c) 2010 - 2011, Intel Corporation. All rights reserved.<BR>
This program and the accompanying materials
are licensed and made available under the terms and conditions of the BSD License
which accompanies this distribution.  The full text of the license may be found at
http://opensource.org/licenses/bsd-license.php

THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include "Cryptest.h"

//
// X509 Cert Data for RSA Public Key Retrieving and X509 Verification (Generated by OpenSSL utility).
//
GLOBAL_REMOVE_IF_UNREFERENCED CONST UINT8 TestCert[] = {
  0x30, 0x82, 0x02, 0x76, 0x30, 0x82, 0x01, 0xdf, 0x02, 0x09, 0x00, 0xa9, 0xff, 0x92, 0x73, 0xf6,
  0x74, 0xe0, 0xb0, 0x30, 0x0d, 0x06, 0x09, 0x2a, 0x86, 0x48, 0x86, 0xf7, 0x0d, 0x01, 0x01, 0x05,
  0x05, 0x00, 0x30, 0x7d, 0x31, 0x0b, 0x30, 0x09, 0x06, 0x03, 0x55, 0x04, 0x06, 0x13, 0x02, 0x43,
  0x4e, 0x31, 0x11, 0x30, 0x0f, 0x06, 0x03, 0x55, 0x04, 0x08, 0x13, 0x08, 0x53, 0x68, 0x61, 0x6e,
  0x67, 0x68, 0x61, 0x69, 0x31, 0x11, 0x30, 0x0f, 0x06, 0x03, 0x55, 0x04, 0x07, 0x13, 0x08, 0x53,
  0x68, 0x61, 0x6e, 0x67, 0x68, 0x61, 0x69, 0x31, 0x0e, 0x30, 0x0c, 0x06, 0x03, 0x55, 0x04, 0x0a,
  0x13, 0x05, 0x49, 0x4e, 0x54, 0x45, 0x4c, 0x31, 0x0c, 0x30, 0x0a, 0x06, 0x03, 0x55, 0x04, 0x0b,
  0x13, 0x03, 0x50, 0x53, 0x49, 0x31, 0x0d, 0x30, 0x0b, 0x06, 0x03, 0x55, 0x04, 0x03, 0x13, 0x04,
  0x55, 0x45, 0x46, 0x49, 0x31, 0x1b, 0x30, 0x19, 0x06, 0x09, 0x2a, 0x86, 0x48, 0x86, 0xf7, 0x0d,
  0x01, 0x09, 0x01, 0x16, 0x0c, 0x75, 0x65, 0x66, 0x69, 0x40, 0x70, 0x73, 0x69, 0x2e, 0x63, 0x6f,
  0x6d, 0x30, 0x1e, 0x17, 0x0d, 0x31, 0x30, 0x31, 0x30, 0x30, 0x38, 0x31, 0x38, 0x32, 0x35, 0x35,
  0x39, 0x5a, 0x17, 0x0d, 0x32, 0x30, 0x31, 0x30, 0x30, 0x35, 0x31, 0x38, 0x32, 0x35, 0x35, 0x39,
  0x5a, 0x30, 0x81, 0x81, 0x31, 0x0b, 0x30, 0x09, 0x06, 0x03, 0x55, 0x04, 0x06, 0x13, 0x02, 0x43,
  0x4e, 0x31, 0x11, 0x30, 0x0f, 0x06, 0x03, 0x55, 0x04, 0x08, 0x13, 0x08, 0x53, 0x68, 0x61, 0x6e,
  0x67, 0x68, 0x61, 0x69, 0x31, 0x11, 0x30, 0x0f, 0x06, 0x03, 0x55, 0x04, 0x07, 0x13, 0x08, 0x53,
  0x68, 0x61, 0x6e, 0x67, 0x68, 0x61, 0x69, 0x31, 0x0e, 0x30, 0x0c, 0x06, 0x03, 0x55, 0x04, 0x0a,
  0x13, 0x05, 0x49, 0x4e, 0x54, 0x45, 0x4c, 0x31, 0x0c, 0x30, 0x0a, 0x06, 0x03, 0x55, 0x04, 0x0b,
  0x13, 0x03, 0x50, 0x53, 0x49, 0x31, 0x0f, 0x30, 0x0d, 0x06, 0x03, 0x55, 0x04, 0x03, 0x13, 0x06,
  0x43, 0x6c, 0x69, 0x65, 0x6e, 0x74, 0x31, 0x1d, 0x30, 0x1b, 0x06, 0x09, 0x2a, 0x86, 0x48, 0x86,
  0xf7, 0x0d, 0x01, 0x09, 0x01, 0x16, 0x0e, 0x63, 0x6c, 0x69, 0x65, 0x6e, 0x74, 0x40, 0x70, 0x73,
  0x69, 0x2e, 0x63, 0x6f, 0x6d, 0x30, 0x81, 0x9f, 0x30, 0x0d, 0x06, 0x09, 0x2a, 0x86, 0x48, 0x86,
  0xf7, 0x0d, 0x01, 0x01, 0x01, 0x05, 0x00, 0x03, 0x81, 0x8d, 0x00, 0x30, 0x81, 0x89, 0x02, 0x81,
  0x81, 0x00, 0xc2, 0xfe, 0xbb, 0xbd, 0x92, 0x60, 0x6c, 0x2b, 0x75, 0x43, 0x6e, 0xd5, 0x91, 0x61,
  0x2a, 0xec, 0x15, 0x84, 0xce, 0x83, 0xc2, 0x51, 0xf6, 0x81, 0x93, 0xe6, 0x38, 0xd4, 0x85, 0xb1,
  0x02, 0x97, 0xb2, 0x7e, 0x74, 0x70, 0x57, 0x09, 0x72, 0xff, 0xb0, 0x7e, 0xd1, 0x9b, 0x34, 0x52,
  0xbb, 0x8e, 0xaf, 0x62, 0x26, 0xad, 0xfa, 0xc5, 0x9e, 0x5c, 0xbc, 0xb9, 0x9e, 0xfe, 0xa0, 0x33,
  0x30, 0x03, 0x9d, 0x3a, 0x09, 0xbb, 0xa5, 0xa9, 0x85, 0x35, 0x73, 0x52, 0xc3, 0xed, 0x10, 0x7f,
  0x83, 0x06, 0xe5, 0x2b, 0x3e, 0x39, 0xd9, 0xdf, 0x34, 0x7e, 0x15, 0x53, 0xbb, 0x82, 0x98, 0xe3,
  0xd8, 0x7e, 0xb3, 0x7e, 0xc0, 0x7f, 0x54, 0x67, 0x57, 0x19, 0xf0, 0xb2, 0xf6, 0x45, 0xaf, 0x43,
  0x05, 0xa5, 0x81, 0xc2, 0x15, 0xd7, 0x26, 0x85, 0xf7, 0xa7, 0x42, 0x36, 0x19, 0x19, 0xba, 0x0a,
  0x04, 0x9d, 0x02, 0x03, 0x01, 0x00, 0x01, 0x30, 0x0d, 0x06, 0x09, 0x2a, 0x86, 0x48, 0x86, 0xf7,
  0x0d, 0x01, 0x01, 0x05, 0x05, 0x00, 0x03, 0x81, 0x81, 0x00, 0x25, 0xbf, 0x8d, 0x88, 0xfc, 0xae,
  0x5e, 0xbe, 0x04, 0xed, 0x4e, 0xf2, 0x2e, 0x2f, 0x55, 0x9e, 0x21, 0x77, 0x86, 0x53, 0x16, 0xc0,
  0x04, 0x11, 0xa2, 0xeb, 0x1e, 0xf1, 0xbc, 0xfa, 0x96, 0xa3, 0xa2, 0x77, 0xe4, 0x61, 0x1b, 0x4a,
  0x69, 0x60, 0x16, 0x6f, 0xcb, 0xc6, 0xe2, 0x72, 0x72, 0xd1, 0x42, 0x7d, 0x83, 0x3d, 0xc5, 0x61,
  0x86, 0x78, 0x4b, 0x95, 0x69, 0x20, 0x88, 0xd1, 0x3c, 0x9b, 0xed, 0x2e, 0x3b, 0xeb, 0xaa, 0x99,
  0x7d, 0x9f, 0x24, 0xe6, 0xa9, 0x57, 0x31, 0x66, 0xe2, 0xe3, 0x3c, 0xd8, 0xb1, 0xf4, 0x33, 0x5d,
  0x8c, 0x21, 0xe0, 0x77, 0x82, 0x6b, 0x44, 0xb0, 0x04, 0x68, 0x25, 0xc8, 0xa1, 0xa2, 0x81, 0x7d,
  0x2e, 0xd5, 0xbb, 0xd2, 0x1d, 0x13, 0x3c, 0x22, 0x6d, 0xc5, 0x4d, 0xec, 0x76, 0x0a, 0x1c, 0xb0,
  0x1e, 0x80, 0xc1, 0xa0, 0xcc, 0x91, 0xd5, 0x7a, 0x5c, 0xf1
  };

//
// Test CA X509 Certificate for X509 Verification Routine (Generated by OpenSSL utility).
//
GLOBAL_REMOVE_IF_UNREFERENCED CONST UINT8 TestCACert[] = {
  0x30, 0x82, 0x02, 0x71, 0x30, 0x82, 0x01, 0xda, 0x02, 0x09, 0x00, 0x91, 0x9b, 0x90, 0x19, 0x9c,
  0x81, 0x28, 0x47, 0x30, 0x0d, 0x06, 0x09, 0x2a, 0x86, 0x48, 0x86, 0xf7, 0x0d, 0x01, 0x01, 0x05,
  0x05, 0x00, 0x30, 0x7d, 0x31, 0x0b, 0x30, 0x09, 0x06, 0x03, 0x55, 0x04, 0x06, 0x13, 0x02, 0x43,
  0x4e, 0x31, 0x11, 0x30, 0x0f, 0x06, 0x03, 0x55, 0x04, 0x08, 0x13, 0x08, 0x53, 0x68, 0x61, 0x6e,
  0x67, 0x68, 0x61, 0x69, 0x31, 0x11, 0x30, 0x0f, 0x06, 0x03, 0x55, 0x04, 0x07, 0x13, 0x08, 0x53,
  0x68, 0x61, 0x6e, 0x67, 0x68, 0x61, 0x69, 0x31, 0x0e, 0x30, 0x0c, 0x06, 0x03, 0x55, 0x04, 0x0a,
  0x13, 0x05, 0x49, 0x4e, 0x54, 0x45, 0x4c, 0x31, 0x0c, 0x30, 0x0a, 0x06, 0x03, 0x55, 0x04, 0x0b,
  0x13, 0x03, 0x50, 0x53, 0x49, 0x31, 0x0d, 0x30, 0x0b, 0x06, 0x03, 0x55, 0x04, 0x03, 0x13, 0x04,
  0x55, 0x45, 0x46, 0x49, 0x31, 0x1b, 0x30, 0x19, 0x06, 0x09, 0x2a, 0x86, 0x48, 0x86, 0xf7, 0x0d,
  0x01, 0x09, 0x01, 0x16, 0x0c, 0x75, 0x65, 0x66, 0x69, 0x40, 0x70, 0x73, 0x69, 0x2e, 0x63, 0x6f,
  0x6d, 0x30, 0x1e, 0x17, 0x0d, 0x31, 0x30, 0x31, 0x30, 0x30, 0x38, 0x31, 0x38, 0x31, 0x35, 0x33,
  0x36, 0x5a, 0x17, 0x0d, 0x32, 0x30, 0x31, 0x30, 0x30, 0x35, 0x31, 0x38, 0x31, 0x35, 0x33, 0x36,
  0x5a, 0x30, 0x7d, 0x31, 0x0b, 0x30, 0x09, 0x06, 0x03, 0x55, 0x04, 0x06, 0x13, 0x02, 0x43, 0x4e,
  0x31, 0x11, 0x30, 0x0f, 0x06, 0x03, 0x55, 0x04, 0x08, 0x13, 0x08, 0x53, 0x68, 0x61, 0x6e, 0x67,
  0x68, 0x61, 0x69, 0x31, 0x11, 0x30, 0x0f, 0x06, 0x03, 0x55, 0x04, 0x07, 0x13, 0x08, 0x53, 0x68,
  0x61, 0x6e, 0x67, 0x68, 0x61, 0x69, 0x31, 0x0e, 0x30, 0x0c, 0x06, 0x03, 0x55, 0x04, 0x0a, 0x13,
  0x05, 0x49, 0x4e, 0x54, 0x45, 0x4c, 0x31, 0x0c, 0x30, 0x0a, 0x06, 0x03, 0x55, 0x04, 0x0b, 0x13,
  0x03, 0x50, 0x53, 0x49, 0x31, 0x0d, 0x30, 0x0b, 0x06, 0x03, 0x55, 0x04, 0x03, 0x13, 0x04, 0x55,
  0x45, 0x46, 0x49, 0x31, 0x1b, 0x30, 0x19, 0x06, 0x09, 0x2a, 0x86, 0x48, 0x86, 0xf7, 0x0d, 0x01,
  0x09, 0x01, 0x16, 0x0c, 0x75, 0x65, 0x66, 0x69, 0x40, 0x70, 0x73, 0x69, 0x2e, 0x63, 0x6f, 0x6d,
  0x30, 0x81, 0x9f, 0x30, 0x0d, 0x06, 0x09, 0x2a, 0x86, 0x48, 0x86, 0xf7, 0x0d, 0x01, 0x01, 0x01,
  0x05, 0x00, 0x03, 0x81, 0x8d, 0x00, 0x30, 0x81, 0x89, 0x02, 0x81, 0x81, 0x00, 0xa3, 0x32, 0x20,
  0x1d, 0x10, 0x11, 0x9c, 0x9e, 0xa2, 0x42, 0x48, 0x9b, 0x15, 0xac, 0x66, 0xa2, 0xc8, 0x25, 0x11,
  0x4c, 0xc8, 0x1e, 0x2e, 0x35, 0xd6, 0xc4, 0x43, 0x2e, 0x39, 0xf3, 0xac, 0x2b, 0xd6, 0x98, 0x5c,
  0xbe, 0x62, 0xfe, 0x95, 0x8c, 0xd6, 0xb5, 0x4e, 0x9e, 0x0f, 0xee, 0x0e, 0xb1, 0xcc, 0x0a, 0x72,
  0xc6, 0x47, 0x66, 0xfe, 0x6a, 0x8b, 0xde, 0x34, 0x0d, 0x62, 0x81, 0xd7, 0xa4, 0x30, 0x3a, 0xe6,
  0x24, 0x3b, 0xe3, 0x5a, 0xd6, 0x2b, 0xec, 0x4a, 0xb7, 0x22, 0x36, 0xed, 0x3a, 0x71, 0xfa, 0xb1,
  0x3f, 0x91, 0xd3, 0x11, 0xac, 0x52, 0xee, 0xbc, 0x37, 0x0e, 0x9e, 0x45, 0xe4, 0x4d, 0x33, 0x83,
  0xef, 0x0c, 0xb3, 0x5a, 0xbe, 0x9e, 0x5c, 0x64, 0xd2, 0x9f, 0x70, 0xf4, 0xaa, 0xd0, 0x15, 0x0e,
  0x60, 0xe5, 0xeb, 0x34, 0xfd, 0xd6, 0x70, 0x64, 0x11, 0x20, 0x60, 0x8c, 0xad, 0x02, 0x03, 0x01,
  0x00, 0x01, 0x30, 0x0d, 0x06, 0x09, 0x2a, 0x86, 0x48, 0x86, 0xf7, 0x0d, 0x01, 0x01, 0x05, 0x05,
  0x00, 0x03, 0x81, 0x81, 0x00, 0x81, 0xfa, 0x8b, 0x03, 0x59, 0x30, 0xbf, 0xd2, 0x7f, 0x90, 0xd9,
  0x1a, 0xd9, 0xae, 0x1f, 0x3e, 0xc5, 0x45, 0x00, 0x0e, 0x06, 0x6e, 0xbc, 0xb0, 0xac, 0x32, 0xe3,
  0x2c, 0x10, 0x2d, 0x59, 0x51, 0x34, 0x7a, 0xb4, 0x22, 0x1d, 0x0f, 0x85, 0x9d, 0x80, 0x90, 0x3f,
  0x8e, 0x78, 0x2f, 0xfc, 0x12, 0x9e, 0xf2, 0xaa, 0xc9, 0x5d, 0x4a, 0x82, 0xc5, 0x64, 0xc7, 0x5a,
  0x29, 0xcb, 0xc2, 0x59, 0xde, 0xdf, 0xd8, 0x69, 0x51, 0x7a, 0x78, 0x4b, 0x47, 0x15, 0xcd, 0x52,
  0x66, 0xff, 0xb8, 0xf5, 0x16, 0xde, 0xe4, 0x32, 0xc5, 0x40, 0x42, 0xeb, 0xeb, 0x54, 0x63, 0xf7,
  0x82, 0x44, 0x4b, 0x5d, 0x8f, 0x3a, 0x29, 0xdf, 0xbc, 0xe0, 0x21, 0x3d, 0xc2, 0x4a, 0x19, 0x6e,
  0x7c, 0xed, 0xd3, 0x79, 0xac, 0xb0, 0x37, 0xea, 0xfd, 0x60, 0x7f, 0xbe, 0x5b, 0x0b, 0x69, 0x4a,
  0xe3, 0xac, 0xfa, 0x75, 0x0f
  };

//
// Password-protected PEM Key data for RSA Private Key Retrieving (encryption key is "client").
// (Generated by OpenSSL utility).
//
GLOBAL_REMOVE_IF_UNREFERENCED CONST UINT8 TestKeyPem[] = {
  0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x42, 0x45, 0x47, 0x49, 0x4e, 0x20, 0x52, 0x53, 0x41, 0x20, 0x50,
  0x52, 0x49, 0x56, 0x41, 0x54, 0x45, 0x20, 0x4b, 0x45, 0x59, 0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x0a,
  0x50, 0x72, 0x6f, 0x63, 0x2d, 0x54, 0x79, 0x70, 0x65, 0x3a, 0x20, 0x34, 0x2c, 0x45, 0x4e, 0x43,
  0x52, 0x59, 0x50, 0x54, 0x45, 0x44, 0x0a, 0x44, 0x45, 0x4b, 0x2d, 0x49, 0x6e, 0x66, 0x6f, 0x3a,
  0x20, 0x44, 0x45, 0x53, 0x2d, 0x45, 0x44, 0x45, 0x33, 0x2d, 0x43, 0x42, 0x43, 0x2c, 0x44, 0x45,
  0x37, 0x32, 0x46, 0x31, 0x31, 0x30, 0x31, 0x31, 0x37, 0x42, 0x33, 0x36, 0x35, 0x36, 0x0a, 0x0a,
  0x38, 0x73, 0x6e, 0x79, 0x32, 0x59, 0x54, 0x79, 0x37, 0x67, 0x52, 0x5a, 0x38, 0x69, 0x73, 0x77,
  0x68, 0x37, 0x66, 0x46, 0x71, 0x49, 0x31, 0x30, 0x61, 0x39, 0x38, 0x6a, 0x2f, 0x76, 0x45, 0x66,
  0x69, 0x65, 0x47, 0x32, 0x43, 0x34, 0x53, 0x2b, 0x48, 0x70, 0x68, 0x33, 0x63, 0x68, 0x7a, 0x45,
  0x2f, 0x63, 0x58, 0x31, 0x2b, 0x6b, 0x6a, 0x6c, 0x46, 0x6b, 0x71, 0x6b, 0x47, 0x4e, 0x65, 0x4d,
  0x0a, 0x70, 0x72, 0x4b, 0x2b, 0x66, 0x48, 0x5a, 0x50, 0x77, 0x6c, 0x30, 0x63, 0x33, 0x79, 0x76,
  0x59, 0x58, 0x45, 0x7a, 0x4c, 0x45, 0x62, 0x50, 0x6f, 0x38, 0x4c, 0x6e, 0x74, 0x38, 0x36, 0x65,
  0x46, 0x53, 0x6f, 0x66, 0x4d, 0x78, 0x70, 0x33, 0x33, 0x64, 0x48, 0x39, 0x5a, 0x68, 0x6f, 0x57,
  0x66, 0x41, 0x43, 0x59, 0x78, 0x47, 0x44, 0x6f, 0x32, 0x30, 0x53, 0x33, 0x79, 0x42, 0x39, 0x67,
  0x38, 0x0a, 0x4d, 0x75, 0x59, 0x63, 0x74, 0x32, 0x38, 0x62, 0x6f, 0x62, 0x30, 0x65, 0x7a, 0x31,
  0x43, 0x38, 0x51, 0x52, 0x78, 0x58, 0x48, 0x31, 0x66, 0x4a, 0x52, 0x5a, 0x52, 0x50, 0x34, 0x38,
  0x50, 0x42, 0x6f, 0x5a, 0x7a, 0x36, 0x73, 0x6a, 0x4b, 0x36, 0x57, 0x51, 0x58, 0x66, 0x67, 0x4d,
  0x32, 0x70, 0x4c, 0x31, 0x42, 0x6f, 0x51, 0x70, 0x4e, 0x4e, 0x4f, 0x6d, 0x61, 0x79, 0x2b, 0x2b,
  0x72, 0x74, 0x0a, 0x6c, 0x7a, 0x32, 0x4f, 0x63, 0x72, 0x6a, 0x67, 0x2f, 0x72, 0x45, 0x61, 0x79,
  0x63, 0x63, 0x43, 0x55, 0x4d, 0x7a, 0x4e, 0x4f, 0x4a, 0x51, 0x74, 0x4f, 0x47, 0x74, 0x34, 0x7a,
  0x4d, 0x4a, 0x53, 0x73, 0x2f, 0x7a, 0x77, 0x77, 0x77, 0x73, 0x5a, 0x43, 0x4b, 0x74, 0x39, 0x33,
  0x37, 0x30, 0x62, 0x76, 0x74, 0x63, 0x36, 0x45, 0x34, 0x75, 0x42, 0x63, 0x75, 0x41, 0x51, 0x72,
  0x37, 0x73, 0x30, 0x0a, 0x44, 0x76, 0x46, 0x64, 0x4d, 0x6d, 0x6f, 0x71, 0x35, 0x57, 0x6d, 0x69,
  0x48, 0x6d, 0x4e, 0x70, 0x67, 0x54, 0x70, 0x65, 0x54, 0x67, 0x77, 0x62, 0x56, 0x64, 0x76, 0x71,
  0x49, 0x4f, 0x71, 0x31, 0x45, 0x6c, 0x6e, 0x30, 0x35, 0x53, 0x70, 0x76, 0x44, 0x7a, 0x4d, 0x56,
  0x76, 0x67, 0x39, 0x78, 0x62, 0x76, 0x64, 0x6f, 0x6e, 0x67, 0x4f, 0x35, 0x77, 0x49, 0x51, 0x70,
  0x69, 0x73, 0x73, 0x47, 0x0a, 0x75, 0x32, 0x69, 0x63, 0x4e, 0x66, 0x48, 0x48, 0x6d, 0x34, 0x76,
  0x48, 0x2b, 0x6d, 0x6e, 0x72, 0x58, 0x45, 0x57, 0x63, 0x69, 0x6c, 0x30, 0x64, 0x61, 0x36, 0x6b,
  0x54, 0x59, 0x66, 0x71, 0x70, 0x6d, 0x46, 0x37, 0x72, 0x52, 0x4d, 0x56, 0x61, 0x6c, 0x69, 0x30,
  0x43, 0x44, 0x4f, 0x59, 0x7a, 0x37, 0x6e, 0x70, 0x51, 0x64, 0x33, 0x38, 0x6a, 0x43, 0x62, 0x78,
  0x65, 0x59, 0x51, 0x65, 0x6d, 0x0a, 0x33, 0x68, 0x73, 0x61, 0x6f, 0x76, 0x58, 0x72, 0x71, 0x71,
  0x4e, 0x34, 0x71, 0x6b, 0x67, 0x50, 0x48, 0x57, 0x68, 0x41, 0x74, 0x39, 0x5a, 0x4d, 0x4e, 0x37,
  0x58, 0x45, 0x62, 0x56, 0x36, 0x42, 0x31, 0x6c, 0x36, 0x77, 0x4a, 0x71, 0x5a, 0x68, 0x68, 0x66,
  0x33, 0x68, 0x79, 0x7a, 0x6f, 0x38, 0x32, 0x38, 0x47, 0x59, 0x45, 0x37, 0x56, 0x58, 0x45, 0x4e,
  0x49, 0x6d, 0x76, 0x73, 0x35, 0x56, 0x0a, 0x69, 0x52, 0x58, 0x31, 0x6d, 0x61, 0x43, 0x30, 0x56,
  0x6b, 0x72, 0x31, 0x46, 0x32, 0x36, 0x55, 0x63, 0x4b, 0x51, 0x67, 0x34, 0x66, 0x53, 0x39, 0x43,
  0x71, 0x48, 0x31, 0x39, 0x7a, 0x4b, 0x36, 0x6d, 0x6d, 0x71, 0x47, 0x75, 0x67, 0x76, 0x66, 0x66,
  0x2f, 0x74, 0x5a, 0x50, 0x72, 0x67, 0x68, 0x61, 0x4f, 0x62, 0x52, 0x2b, 0x77, 0x76, 0x34, 0x46,
  0x65, 0x4f, 0x32, 0x42, 0x45, 0x44, 0x6d, 0x0a, 0x67, 0x4d, 0x33, 0x71, 0x47, 0x51, 0x4a, 0x44,
  0x35, 0x53, 0x65, 0x77, 0x4f, 0x61, 0x62, 0x41, 0x72, 0x4e, 0x37, 0x4c, 0x6f, 0x30, 0x59, 0x2b,
  0x44, 0x6a, 0x79, 0x39, 0x44, 0x43, 0x4b, 0x6f, 0x47, 0x4e, 0x4a, 0x50, 0x53, 0x4f, 0x58, 0x65,
  0x70, 0x57, 0x48, 0x65, 0x6d, 0x6c, 0x76, 0x72, 0x49, 0x63, 0x39, 0x66, 0x4d, 0x2f, 0x37, 0x57,
  0x6a, 0x4b, 0x4d, 0x6b, 0x72, 0x57, 0x50, 0x6a, 0x0a, 0x56, 0x64, 0x73, 0x61, 0x6e, 0x4b, 0x30,
  0x7a, 0x74, 0x4e, 0x2b, 0x43, 0x49, 0x64, 0x66, 0x38, 0x70, 0x33, 0x55, 0x30, 0x30, 0x57, 0x44,
  0x6d, 0x30, 0x2f, 0x62, 0x4d, 0x43, 0x56, 0x6d, 0x6b, 0x36, 0x6a, 0x76, 0x47, 0x66, 0x2f, 0x63,
  0x55, 0x6c, 0x47, 0x38, 0x79, 0x6d, 0x30, 0x2f, 0x49, 0x67, 0x4a, 0x70, 0x71, 0x35, 0x2b, 0x33,
  0x62, 0x78, 0x38, 0x73, 0x63, 0x54, 0x64, 0x55, 0x4f, 0x0a, 0x41, 0x38, 0x30, 0x41, 0x56, 0x68,
  0x61, 0x53, 0x41, 0x71, 0x44, 0x6d, 0x68, 0x49, 0x6c, 0x59, 0x34, 0x54, 0x6f, 0x78, 0x42, 0x68,
  0x63, 0x46, 0x2b, 0x4b, 0x4d, 0x48, 0x57, 0x33, 0x33, 0x5a, 0x45, 0x79, 0x66, 0x4a, 0x4a, 0x54,
  0x71, 0x55, 0x42, 0x71, 0x4a, 0x6a, 0x4f, 0x69, 0x75, 0x41, 0x78, 0x6a, 0x59, 0x70, 0x71, 0x4f,
  0x4e, 0x45, 0x35, 0x56, 0x4b, 0x33, 0x48, 0x68, 0x6c, 0x45, 0x0a, 0x2f, 0x4a, 0x33, 0x6b, 0x57,
  0x79, 0x4f, 0x39, 0x69, 0x4d, 0x62, 0x33, 0x67, 0x73, 0x44, 0x59, 0x36, 0x41, 0x76, 0x41, 0x76,
  0x5a, 0x39, 0x71, 0x6c, 0x5a, 0x6b, 0x30, 0x52, 0x50, 0x67, 0x49, 0x4c, 0x4a, 0x77, 0x6e, 0x33,
  0x6d, 0x77, 0x67, 0x73, 0x63, 0x55, 0x70, 0x41, 0x30, 0x5a, 0x50, 0x6a, 0x61, 0x55, 0x56, 0x6c,
  0x64, 0x71, 0x70, 0x32, 0x69, 0x71, 0x47, 0x78, 0x71, 0x50, 0x36, 0x0a, 0x45, 0x72, 0x65, 0x38,
  0x38, 0x59, 0x75, 0x41, 0x53, 0x55, 0x4a, 0x5a, 0x4a, 0x62, 0x34, 0x72, 0x53, 0x42, 0x4c, 0x68,
  0x45, 0x55, 0x41, 0x76, 0x63, 0x67, 0x38, 0x33, 0x4d, 0x6b, 0x4d, 0x6c, 0x68, 0x74, 0x6b, 0x34,
  0x62, 0x67, 0x34, 0x5a, 0x35, 0x65, 0x73, 0x44, 0x57, 0x66, 0x4d, 0x67, 0x56, 0x65, 0x6a, 0x4e,
  0x4a, 0x51, 0x3d, 0x3d, 0x0a, 0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x45, 0x4e, 0x44, 0x20, 0x52, 0x53,
  0x41, 0x20, 0x50, 0x52, 0x49, 0x56, 0x41, 0x54, 0x45, 0x20, 0x4b, 0x45, 0x59, 0x2d, 0x2d, 0x2d,
  0x2d, 0x2d
  };

//
// Password for private key retrieving from encrypted PEM ("TestKeyPem").
//
GLOBAL_REMOVE_IF_UNREFERENCED CONST CHAR8 *PemPass = "client";

//
// Message Hash for Signing & Verification Validation.
//
GLOBAL_REMOVE_IF_UNREFERENCED CONST UINT8 MsgHash[] = {
  0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09,
  0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09,
  0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09,
  0x00, 0x01
  };

//
// Payload for PKCS#7 Signing & Verification Validation.
//
GLOBAL_REMOVE_IF_UNREFERENCED CONST CHAR8 *Payload = "Payload Data for PKCS#7 Signing";

/**
  Validate UEFI-OpenSSL RSA Key Retrieving & Signature Interfaces.

  @retval  EFI_SUCCESS  Validation succeeded.
  @retval  EFI_ABORTED  Validation failed.

**/
EFI_STATUS
ValidateCryptRsa2 (
  VOID
  )
{
  BOOLEAN        Status;
  VOID           *RsaPrivKey;
  VOID           *RsaPubKey;
  UINT8          *Signature;
  UINTN          SigSize;
  UINT8          *Subject;
  UINTN          SubjectSize;
  RETURN_STATUS  ReturnStatus;
  CHAR8          CommonName[64];
  // CHAR16         CommonNameUnicode[64];
  UINTN          CommonNameSize;

  Print (L"\nUEFI-OpenSSL RSA Key Retrieving Testing: ");

  //
  // Retrieve RSA private key from encrypted PEM data.
  //
  Print (L"\n- Retrieve RSA Private Key for PEM ...");
  Status = RsaGetPrivateKeyFromPem (TestKeyPem, sizeof (TestKeyPem), PemPass, &RsaPrivKey);
  if (!Status) {
    Print (L"[Fail]");
    return EFI_ABORTED;
  } else {
    Print (L"[Pass]");
  }

  //
  // Retrieve RSA public key from X509 Certificate.
  //
  Print (L"\n- Retrieve RSA Public Key from X509 ... ");
  RsaPubKey = NULL;
  Status    = RsaGetPublicKeyFromX509 (TestCert, sizeof (TestCert), &RsaPubKey);
  if (!Status) {
    Print (L"[Fail]");
    return EFI_ABORTED;
  } else {
    Print (L"[Pass]");
  }

  //
  // Generate RSA PKCS#1 Signature.
  //
  Print (L"\n- PKCS#1 Signature ... ");
  SigSize = 0;
  Status  = RsaPkcs1Sign (RsaPrivKey, MsgHash, SHA256_DIGEST_SIZE, NULL, &SigSize);
  if (Status || SigSize == 0) {
    Print (L"[Fail]");
    return EFI_ABORTED;
  }

  Signature = AllocatePool (SigSize);
  Status    = RsaPkcs1Sign (RsaPrivKey, MsgHash, SHA256_DIGEST_SIZE, Signature, &SigSize);
  if (!Status) {
    Print (L"[Fail]");
    return EFI_ABORTED;
  } else {
    Print (L"[Pass]");
  }

  //
  // Verify RSA PKCS#1-encoded Signature.
  //
  Print (L"\n- PKCS#1 Signature Verification ... ");
  Status = RsaPkcs1Verify (RsaPubKey, MsgHash, SHA256_DIGEST_SIZE, Signature, SigSize);
  if (!Status) {
    Print (L"[Fail]");
    return EFI_ABORTED;
  } else {
    Print (L"[Pass]");
  }

  //
  // X509 Certificate Subject Retrieving.
  //
  Print (L"\n- X509 Certificate Subject Bytes Retrieving ... ");
  SubjectSize = 0;
  Status  = X509GetSubjectName (TestCert, sizeof (TestCert), NULL, &SubjectSize);
  Subject = (UINT8 *)AllocatePool (SubjectSize);
  Status  = X509GetSubjectName (TestCert, sizeof (TestCert), Subject, &SubjectSize);
  if (!Status) {
    Print (L"[Fail]");
    return EFI_ABORTED;
  } else {
    Print (L"[Pass]");
  }

  //
  // Get CommonName from X509 Certificate Subject
  //
  CommonNameSize = 64;
  ZeroMem (CommonName, CommonNameSize);
  ReturnStatus = X509GetCommonName (TestCert, sizeof (TestCert), CommonName, &CommonNameSize);
  if (RETURN_ERROR (ReturnStatus)) {
    Print (L"\n  - Retrieving Common Name - [Fail]");
    return EFI_ABORTED;
  } else {
    // AsciiStrToUnicodeStrS (CommonName, CommonNameUnicode, CommonNameSize);
    // Print (L"\n  - Retrieving Common Name = \"%s\" (Size = %d)", CommonNameUnicode, CommonNameSize);
  }

  //
  // X509 Certificate Verification.
  //
  Print (L"\n- X509 Certificate Verification with Trusted CA ...");
  Status = X509VerifyCert (TestCert, sizeof (TestCert), TestCACert, sizeof (TestCACert));
  if (!Status) {
    Print (L"[Fail]\n");
    return EFI_ABORTED;
  } else {
    Print (L"[Pass]\n");
  }

  //
  // Release Resources.
  //
  RsaFree  (RsaPubKey);
  RsaFree  (RsaPrivKey);
  FreePool (Signature);
  FreePool (Subject);

  return EFI_SUCCESS;
}

/**
  Validate UEFI-OpenSSL PKCS#7 Signing & Verification Interfaces.

  @retval  EFI_SUCCESS  Validation succeeded.
  @retval  EFI_ABORTED  Validation failed.

**/
EFI_STATUS
ValidateCryptPkcs7 (
  VOID
  )
{
  BOOLEAN  Status;
  UINT8    *P7SignedData;
  UINTN    P7SignedDataSize;
  UINT8    *SignCert;

  P7SignedData = NULL;
  SignCert     = NULL;

  Print (L"\nUEFI-OpenSSL PKCS#7 Signing & Verification Testing: ");

  Print (L"\n- Create PKCS#7 signedData ...");

  //
  // Construct Signer Certificate from RAW data.
  //
  Status = X509ConstructCertificate (TestCert, sizeof (TestCert), (UINT8 **) &SignCert);
  if (!Status || SignCert == NULL) {
    Print (L"[Fail]");
    goto _Exit;
  } else {
    Print (L"[Pass]");
  }

  //
  // Create PKCS#7 signedData on Payload.
  // Note: Caller should release P7SignedData manually.
  //
  Status = Pkcs7Sign (
             TestKeyPem,
             sizeof (TestKeyPem),
             (CONST UINT8 *) PemPass,
             (UINT8 *) Payload,
             AsciiStrLen (Payload),
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
             TestCACert,
             sizeof (TestCACert),
             (UINT8 *) Payload,
             AsciiStrLen (Payload)
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