#ifndef _GIT_UEFI_INCLUDE_EFI_TYPES_H_
#define _GIT_UEFI_INCLUDE_EFI_TYPES_H_

#include <stdint.h>

typedef uint8_t BOOLEAN;
typedef int64_t INTN;
typedef uint64_t UINTN;
typedef int8_t INT8;
typedef uint8_t UINT8;
typedef int16_t INT16;
typedef uint16_t UINT16;
typedef int32_t INT32;
typedef uint32_t UINT32;
typedef int64_t INT64;
typedef uint64_t UINT64;
typedef uint8_t CHAR8;
typedef uint16_t CHAR16;
typedef void VOID;
typedef __int128 INT128;
typedef unsigned __int128 UINT128;
typedef struct { UINT32 Data1; UINT16 Data2; UINT16 Data3; UINT8 Data4[8]; } EFI_GUID;
typedef UINTN EFI_STATUS;
typedef void *EFI_HANDLE;
typedef void *EFI_EVENT;
typedef UINT64 EFI_LBA;
typedef UINTN EFI_TPL;
typedef struct { uint8_t b[32]; } EFI_MAC_ADDRESS;
typedef struct { uint8_t b[4]; } EFI_IPv4_ADDRESS;
typedef struct { uint8_t b[16]; } EFI_IPv6_ADDRESS;
typedef struct { uint8_t b[16]; } EFI_IP_ADDRESS;

typedef struct
{
    UINT64 Signature;
    UINT32 Revision;
    UINT32 HeaderSize;
    UINT32 CRC32;
    UINT32 Reserved;
} EFI_TABLE_HEADER;

#define EFIAPI /* */
#define IN /* */
#define OUT /* */
#define OPTIONAL /* */
#define CONST const
#define ANYSIZE_ARRAY 0

//*******************************************************
// EFI_INTERFACE_TYPE
//*******************************************************
typedef enum
{
    EFI_NATIVE_INTERFACE
} EFI_INTERFACE_TYPE;

//*******************************************************
// EFI_LOCATE_SEARCH_TYPE
//*******************************************************
typedef enum
{
    AllHandles,
    ByRegisterNotify,
    ByProtocol
} EFI_LOCATE_SEARCH_TYPE;

typedef struct
{
    EFI_HANDLE AgentHandle;
    EFI_HANDLE ControllerHandle;
    UINT32 Attributes;
    UINT32 OpenCount;
} EFI_OPEN_PROTOCOL_INFORMATION_ENTRY;

#define ERROR_MASK (1ULL << (sizeof(UINTN) * 8 - 1))

#define EFI_SUCCESS                 0
#define EFI_LOAD_ERROR              (ERROR_MASK | 1)
#define EFI_INVALID_PARAMETER       (ERROR_MASK | 2)
#define EFI_UNSUPPORTED             (ERROR_MASK | 3)
#define EFI_BAD_BUFFER_SIZE         (ERROR_MASK | 4)
#define EFI_BUFFER_TOO_SMALL        (ERROR_MASK | 5)
#define EFI_NOT_READY               (ERROR_MASK | 6)
#define EFI_DEVICE_ERROR            (ERROR_MASK | 7)
#define EFI_WRITE_PROTECTED         (ERROR_MASK | 8)
#define EFI_OUT_OF_RESOURCES        (ERROR_MASK | 9)
#define EFI_VOLUME_CORRUPTED        (ERROR_MASK | 10)
#define EFI_VOLUME_FULL             (ERROR_MASK | 11)
#define EFI_NO_MEDIA                (ERROR_MASK | 12)
#define EFI_MEDIA_CHANGED           (ERROR_MASK | 13)
#define EFI_NOT_FOUND               (ERROR_MASK | 14)
#define EFI_ACCESS_DENIED           (ERROR_MASK | 15)
#define EFI_NO_RESPONSE             (ERROR_MASK | 16)
#define EFI_NO_MAPPING              (ERROR_MASK | 17)
#define EFI_TIMEOUT                 (ERROR_MASK | 18)
#define EFI_NOT_STARTED             (ERROR_MASK | 19)
#define EFI_ALREADY_STARTED         (ERROR_MASK | 20)
#define EFI_ABORTED                 (ERROR_MASK | 21)
#define EFI_ICMP_ERROR              (ERROR_MASK | 22)
#define EFI_TFTP_ERROR              (ERROR_MASK | 23)
#define EFI_PROTOCOL_ERROR          (ERROR_MASK | 24)
#define EFI_INCOMPATIBLE_VERSION    (ERROR_MASK | 25)
#define EFI_SECURITY_VIOLATION      (ERROR_MASK | 26)
#define EFI_CRC_ERROR               (ERROR_MASK | 27)
#define EFI_END_OF_MEDIA            (ERROR_MASK | 28)
#define EFI_END_OF_FILE             (ERROR_MASK | 31)
#define EFI_INVALID_LANGUAGE        (ERROR_MASK | 32)
#define EFI_COMPROMISED_DATA        (ERROR_MASK | 33)
#define EFI_IP_ADDRESS_CONFLICT     (ERROR_MASK | 34)
#define EFI_HTTP_ERROR              (ERROR_MASK | 35)

#define EFI_WARN_UNKNOWN_GLYPH      1
#define EFI_WARN_DELETE_FAILURE     2
#define EFI_WARN_WRITE_FAILURE      3
#define EFI_WARN_BUFFER_TOO_SMALL   4
#define EFI_WARN_STALE_DATA         5
#define EFI_WARN_FILE_SYSTEM        6
#define EFI_WARN_RESET_REQUIRED     7

#define NULL ((void*)0)

#endif // _GIT_UEFI_INCLUDE_EFI_TYPES_H_
