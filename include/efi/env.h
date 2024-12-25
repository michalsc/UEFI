#ifndef INCLUDE_EFI_ENV_H_
#define INCLUDE_EFI_ENV_H_

#include <efi/types.h>
#include <efi/time.h>
#include <efi/cert.h>

//*******************************************************
// Variable Attributes
//*******************************************************
#define EFI_VARIABLE_NON_VOLATILE 0x00000001
#define EFI_VARIABLE_BOOTSERVICE_ACCESS 0x00000002
#define EFI_VARIABLE_RUNTIME_ACCESS 0x00000004
#define EFI_VARIABLE_HARDWARE_ERROR_RECORD 0x00000008 // This attribute is identified by the mnemonic 'HR' elsewhere
                                                      // in this specification.
#define EFI_VARIABLE_AUTHENTICATED_WRITE_ACCESS 0x00000010
// NOTE: EFI_VARIABLE_AUTHENTICATED_WRITE_ACCESS is deprecated
// and should be considered reserved.
#define EFI_VARIABLE_TIME_BASED_AUTHENTICATED_WRITE_ACCESS \ 0x00000020
#define EFI_VARIABLE_APPEND_WRITE 0x00000040
#define EFI_VARIABLE_ENHANCED_AUTHENTICATED_ACCESS 0x00000080
// This attribute indicates that the variable payload begins
// with an EFI_VARIABLE_AUTHENTICATION_3 structure, and
// potentially more structures as indicated by fields of this
// structure. See definition below and in SetVariable().

//
// EFI_VARIABLE_AUTHENTICATION_3_CERT_ID descriptor
//
// An extensible structure to identify a unique x509 cert
// associated with a given variable
//
#define EFI_VARIABLE_AUTHENTICATION_3_CERT_ID_SHA256 1
typedef struct
{
    UINT8 Type;
    UINT32 IdSize;
    // UINT8 Id[IdSize];
} EFI_VARIABLE_AUTHENTICATION_3_CERT_ID;

//*******************************************************
// Variable Attributes
//*******************************************************
// NOTE: This interface is deprecated and should no longer be used!
//
// EFI_VARIABLE_AUTHENTICATION descriptor
//
// A counter-based authentication method descriptor template
//
typedef struct
{
    UINT64 MonotonicCount;
    WIN_CERTIFICATE_UEFI_GUID AuthInfo;
} EFI_VARIABLE_AUTHENTICATION;

//
// EFI_VARIABLE_AUTHENTICATION_2 descriptor
//
// A time-based authentication method descriptor template
//
typedef struct
{
    EFI_TIME TimeStamp;
    WIN_CERTIFICATE_UEFI_GUID AuthInfo;
} EFI_VARIABLE_AUTHENTICATION_2;

//
// EFI_VARIABLE_AUTHENTICATION_3 descriptor
//
// An extensible implementation of the Variable Authentication
// structure.
//
#define EFI_VARIABLE_AUTHENTICATION_3_TIMESTAMP_TYPE 1
#define EFI_VARIABLE_AUTHENTICATION_3_NONCE_TYPE 2
typedef struct
{
    UINT8 Version;
    UINT8 Type;
    UINT32 MetadataSize;
    UINT32 Flags;
} EFI_VARIABLE_AUTHENTICATION_3;

#endif // INCLUDE_EFI_ENV_H_
