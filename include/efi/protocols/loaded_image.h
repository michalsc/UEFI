#ifndef INCLUDE_EFI_PROTOCOLS_LOADED_IMAGE_H_
#define INCLUDE_EFI_PROTOCOLS_LOADED_IMAGE_H_

#include <efi/system.h>
#include <efi/types.h>
#include <efi/memory.h>

#define EFI_LOADED_IMAGE_PROTOCOL_GUID \
    { 0x5B1B31A1, 0x9562, 0x11d2, \
    { 0x8E, 0x3F, 0x00, 0xA0, 0xC9, 0x69, 0x72, 0x3B }}

#define EFI_LOADED_IMAGE_PROTOCOL_REVISION 0x1000

typedef EFI_STATUS(EFIAPI *EFI_IMAGE_UNLOAD)(
    IN EFI_HANDLE ImageHandle);

typedef struct {
    UINT32 Revision;
    EFI_HANDLE ParentHandle;
    EFI_SYSTEM_TABLE *SystemTable;
    // Source location of the image
    EFI_HANDLE DeviceHandle;
    EFI_DEVICE_PATH_PROTOCOL *FilePath;
    VOID *Reserved;
    // Image’s load options
    UINT32 LoadOptionsSize;
    VOID *LoadOptions;
    // Location where image was loaded
    VOID *ImageBase;
    UINT64 ImageSize;
    EFI_MEMORY_TYPE ImageCodeType;
    EFI_MEMORY_TYPE ImageDataType;
    EFI_IMAGE_UNLOAD Unload;
} EFI_LOADED_IMAGE_PROTOCOL;

#endif /* INCLUDE_EFI_PROTOCOLS_LOADED_IMAGE_H_ */
