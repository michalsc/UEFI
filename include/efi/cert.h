#ifndef INCLUDE_EFI_CERT_H_
#define INCLUDE_EFI_CERT_H_

#include <efi/types.h>

#define EFI_CERT_TYPE_RSA2048_SHA256_GUID \
    { 0xa7717414, 0xc616, 0x4977, \
    { 0x94, 0x20, 0x84, 0x47, 0x12, 0xa7, 0x35, 0xbf }}

#define EFI_CERT_TYPE_PKCS7_GUID \
    { 0x4aafd29d, 0x68df, 0x49ee, \
    { 0x8a, 0xa9, 0x34, 0x7d, 0x37, 0x56, 0x65, 0xa7 }}

typedef struct _WIN_CERTIFICATE
{
    UINT32 dwLength;
    UINT16 wRevision;
    UINT16 wCertificateType;
    // UINT8 bCertificate[ANYSIZE_ARRAY];
} WIN_CERTIFICATE;

#define WIN_CERT_TYPE_PKCS_SIGNED_DATA 0x0002
#define WIN_CERT_TYPE_EFI_PKCS115 0x0EF0
#define WIN_CERT_TYPE_EFI_GUID 0x0EF1

typedef struct _EFI_CERT_BLOCK_RSA_2048_SHA256 {
    EFI_GUID HashType;
    UINT8 PublicKey[256];
    UINT8 Signature[256];
} EFI_CERT_BLOCK_RSA_2048_SHA256;

typedef struct _WIN_CERTIFICATE_UEFI_GUID
{
    WIN_CERTIFICATE Hdr;
    EFI_GUID CertType;
    UINT8 CertData[ANYSIZE_ARRAY];
} WIN_CERTIFICATE_UEFI_GUID;

#endif // INCLUDE_EFI_CERT_H_
