#ifndef INCLUDE_EFI_MEMORY_H_
#define INCLUDE_EFI_MEMORY_H_

#include <efi/types.h>

//*******************************************************
// EFI_ALLOCATE_TYPE
//*******************************************************
// These types are discussed in the “Description” section below.
typedef enum
{
    AllocateAnyPages,
    AllocateMaxAddress,
    AllocateAddress,
    MaxAllocateType
} EFI_ALLOCATE_TYPE;

//*******************************************************
//EFI_MEMORY_TYPE
//*******************************************************
// These type values are discussed in Table 29 and Table 30.
typedef enum
{
    EfiReservedMemoryType,
    EfiLoaderCode,
    EfiLoaderData,
    EfiBootServicesCode,
    EfiBootServicesData,
    EfiRuntimeServicesCode,
    EfiRuntimeServicesData,
    EfiConventionalMemory,
    EfiUnusableMemory,
    EfiACPIReclaimMemory,
    EfiACPIMemoryNVS,
    EfiMemoryMappedIO,
    EfiMemoryMappedIOPortSpace,
    EfiPalCode,
    EfiPersistentMemory,
    EfiMaxMemoryType
} EFI_MEMORY_TYPE;

//*******************************************************
// EFI_PHYSICAL_ADDRESS
//*******************************************************
typedef UINT64 EFI_PHYSICAL_ADDRESS;

typedef struct
{
    UINT64 Length;
    union
    {
        EFI_PHYSICAL_ADDRESS DataBlock;
        EFI_PHYSICAL_ADDRESS ContinuationPointer;
    } Union;
} EFI_CAPSULE_BLOCK_DESCRIPTOR;

typedef struct
{
    EFI_GUID CapsuleGuid;
    UINT32 HeaderSize;
    UINT32 Flags;
    UINT32 CapsuleImageSize;
} EFI_CAPSULE_HEADER;

#define CAPSULE_FLAGS_PERSIST_ACROSS_RESET 0x00010000
#define CAPSULE_FLAGS_POPULATE_SYSTEM_TABLE 0x00020000
#define CAPSULE_FLAGS_INITIATE_RESET 0x00040000

typedef struct
{
    UINT32 CapsuleArrayNumber;
    VOID *CapsulePtr[1];
} EFI_CAPSULE_TABLE;

typedef struct
{
    EFI_PHYSICAL_ADDRESS Address;
    UINT64 Length;
}
EFI_MEMORY_RANGE;

typedef struct
{
    EFI_CAPSULE_HEADER Header;
    UINT32 OsRequestedMemoryType;
    UINT64 NumberOfMemoryRanges;
    EFI_MEMORY_RANGE MemoryRanges[];
} EFI_MEMORY_RANGE_CAPSULE;

//*******************************************************
// EFI_OPTIONAL_PTR
//*******************************************************
#define EFI_OPTIONAL_PTR 0x00000001

#define EFI_MEMORY_ATTRIBUTES_TABLE_GUID \
    { 0xdcfa911d, 0x26eb, 0x469f, \
    { 0xa2, 0x20, 0x38, 0xb7, 0xdc, 0x46, 0x12, 0x20 }}

//*******************************************************
// EFI_MEMORY_ATTRIBUTES_TABLE
//*******************************************************
typedef struct {
    UINT32 Version ;
    UINT32 NumberOfEntries ;
    UINT32 DescriptorSize ;
    UINT32 Reserved;
    // EFI_MEMORY_DESCRIPTOR Entry [1];
} EFI_MEMORY_ATTRIBUTES_TABLE;

//*******************************************************
// EFI_VIRTUAL_ADDRESS
//*******************************************************
typedef UINT64 EFI_VIRTUAL_ADDRESS;
//*******************************************************
// Memory Descriptor Version Number
//*******************************************************
#define EFI_MEMORY_DESCRIPTOR_VERSION 1

//*******************************************************
// EFI_MEMORY_DESCRIPTOR
//*******************************************************
typedef struct
{
    UINT32 Type;
    EFI_PHYSICAL_ADDRESS PhysicalStart;
    EFI_VIRTUAL_ADDRESS VirtualStart;
    UINT64 NumberOfPages;
    UINT64 Attribute;
} EFI_MEMORY_DESCRIPTOR;

//*******************************************************
// Memory Attribute Definitions
//*******************************************************
// These types can be “ORed” together as needed.
#define EFI_MEMORY_UC 0x0000000000000001
#define EFI_MEMORY_WC 0x0000000000000002
#define EFI_MEMORY_WT 0x0000000000000004
#define EFI_MEMORY_WB 0x0000000000000008
#define EFI_MEMORY_UCE 0x0000000000000010
#define EFI_MEMORY_WP 0x0000000000001000
#define EFI_MEMORY_RP 0x0000000000002000
#define EFI_MEMORY_XP 0x0000000000004000
#define EFI_MEMORY_NV 0x0000000000008000
#define EFI_MEMORY_MORE_RELIABLE 0x0000000000010000
#define EFI_MEMORY_RO 0x0000000000020000
#define EFI_MEMORY_SP 0x0000000000040000
#define EFI_MEMORY_CPU_CRYPTO 0x0000000000080000
#define EFI_MEMORY_RUNTIME 0x8000000000000000


#endif // INCLUDE_EFI_MEMORY_H_
