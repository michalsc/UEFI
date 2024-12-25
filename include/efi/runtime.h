#ifndef INCLUDE_EFI_RUNTIME_H_
#define INCLUDE_EFI_RUNTIME_H_

#include <efi/types.h>
#include <efi/time.h>
#include <efi/memory.h>

//*******************************************************
// EFI_RESET_TYPE
//*******************************************************
typedef enum
{
    EfiResetCold,
    EfiResetWarm,
    EfiResetShutdown,
    EfiResetPlatformSpecific
} EFI_RESET_TYPE;

#define EFI_RUNTIME_SERVICES_SIGNATURE 0x56524553544e5552
#define EFI_RUNTIME_SERVICES_REVISION EFI_SPECIFICATION_VERSION

typedef EFI_STATUS(EFIAPI *EFI_GET_TIME)(
    OUT EFI_TIME *Time,
    OUT EFI_TIME_CAPABILITIES *Capabilities OPTIONAL);

typedef EFI_STATUS(EFIAPI *EFI_SET_TIME)(
    IN EFI_TIME *Time);

typedef EFI_STATUS(EFIAPI *EFI_GET_WAKEUP_TIME)(
    OUT BOOLEAN *Enabled,
    OUT BOOLEAN *Pending,
    OUT EFI_TIME *Time);

typedef EFI_STATUS(EFIAPI *EFI_SET_WAKEUP_TIME)(
    IN BOOLEAN Enable,
    IN EFI_TIME *Time OPTIONAL);

typedef EFI_STATUS(EFIAPI *EFI_SET_VIRTUAL_ADDRESS_MAP)(
    IN UINTN MemoryMapSize,
    IN UINTN DescriptorSize,
    IN UINT32 DescriptorVersion,
    IN EFI_MEMORY_DESCRIPTOR *VirtualMap);

typedef EFI_STATUS(EFIAPI *EFI_CONVERT_POINTER)(
    IN UINTN DebugDisposition,
    IN VOID **Address);

typedef EFI_STATUS(EFIAPI *EFI_GET_VARIABLE)(
    IN CHAR16 *VariableName,
    IN EFI_GUID *VendorGuid,
    OUT UINT32 *Attributes OPTIONAL,
    IN OUT UINTN *DataSize,
    OUT VOID *Data OPTIONAL);

typedef EFI_STATUS(EFIAPI *EFI_GET_NEXT_VARIABLE_NAME)(
    IN OUT UINTN *VariableNameSize,
    IN OUT CHAR16 *VariableName,
    IN OUT EFI_GUID *VendorGuid);

typedef EFI_STATUS(EFIAPI *EFI_SET_VARIABLE)(
    IN CHAR16 *VariableName,
    IN EFI_GUID *VendorGuid,
    IN UINT32 Attributes,
    IN UINTN DataSize,
    IN VOID *Data);

typedef VOID(EFIAPI *EFI_RESET_SYSTEM)(
    IN EFI_RESET_TYPE ResetType,
    IN EFI_STATUS ResetStatus,
    IN UINTN DataSize,
    IN VOID *ResetData OPTIONAL);

typedef EFI_STATUS(EFIAPI *EFI_GET_NEXT_HIGH_MONO_COUNT)(
    OUT UINT32 *HighCount);

typedef EFI_STATUS(EFIAPI *EFI_UPDATE_CAPSULE)(
    IN EFI_CAPSULE_HEADER **CapsuleHeaderArray,
    IN UINTN CapsuleCount,
    IN EFI_PHYSICAL_ADDRESS ScatterGatherList OPTIONAL);

typedef EFI_STATUS(EFIAPI *EFI_QUERY_CAPSULE_CAPABILITIES)(
    IN EFI_CAPSULE_HEADER **CapsuleHeaderArray,
    IN UINTN CapsuleCount,
    OUT UINT64 *MaximumCapsuleSize,
    OUT EFI_RESET_TYPE *ResetType);

typedef EFI_STATUS(EFIAPI *EFI_QUERY_VARIABLE_INFO)(
    IN UINT32 Attributes,
    OUT UINT64 *MaximumVariableStorageSize,
    OUT UINT64 *RemainingVariableStorageSize,
    OUT UINT64 *MaximumVariableSize);

typedef struct
{
    EFI_TABLE_HEADER Hdr;
    
    //
    // Time Services
    //
    EFI_GET_TIME GetTime;
    EFI_SET_TIME SetTime;
    EFI_GET_WAKEUP_TIME GetWakeupTime;
    EFI_SET_WAKEUP_TIME SetWakeupTime;
    
    //
    // Virtual Memory Services
    //
    EFI_SET_VIRTUAL_ADDRESS_MAP SetVirtualAddressMap;
    EFI_CONVERT_POINTER ConvertPointer;
    
    //
    // Variable Services
    //
    EFI_GET_VARIABLE GetVariable;
    EFI_GET_NEXT_VARIABLE_NAME GetNextVariableName;
    EFI_SET_VARIABLE SetVariable;
    
    //
    // Miscellaneous Services
    //
    EFI_GET_NEXT_HIGH_MONO_COUNT GetNextHighMonotonicCount;
    EFI_RESET_SYSTEM ResetSystem;
    
    //
    // UEFI 2.0 Capsule Services
    //
    EFI_UPDATE_CAPSULE UpdateCapsule;
    EFI_QUERY_CAPSULE_CAPABILITIES QueryCapsuleCapabilities;
    
    //
    // Miscellaneous UEFI 2.0 Service
    //
    EFI_QUERY_VARIABLE_INFO QueryVariableInfo;
} EFI_RUNTIME_SERVICES;

#endif // INCLUDE_EFI_RUNTIME_H_
