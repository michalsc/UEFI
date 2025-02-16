#ifndef INCLUDE_EFI_PROTOCOLS_LOAD_FILE_H_
#define INCLUDE_EFI_PROTOCOLS_LOAD_FILE_H_

#include <efi/types.h>
#include <efi/memory.h>
#include <efi/protocols/device_path.h>

#define EFI_LOAD_FILE_PROTOCOL_GUID \
    { 0x56EC3091, 0x954C, 0x11d2, \
    { 0x8e, 0x3f, 0x00, 0xa0, 0xc9, 0x69, 0x72, 0x3b }}

#define EFI_LOAD_FILE2_PROTOCOL_GUID \
    { 0x4006c0c1, 0xfcb3, 0x403e, \
    { 0x99, 0x6d, 0x4a, 0x6c, 0x87, 0x24, 0xe0, 0x6d }}

struct _EFI_LOAD_FILE_PROTOCOL;
typedef struct _EFI_LOAD_FILE_PROTOCOL EFI_LOAD_FILE_PROTOCOL;
typedef struct _EFI_LOAD_FILE_PROTOCOL EFI_LOAD_FILE2_PROTOCOL;

typedef EFI_STATUS (EFIAPI *EFI_LOAD_FILE)(
    IN EFI_LOAD_FILE_PROTOCOL *This,
    IN EFI_DEVICE_PATH_PROTOCOL *FilePath,
    IN BOOLEAN BootPolicy,
    IN OUT UINTN *BufferSize,
    IN VOID *Buffer);

typedef struct _EFI_LOAD_FILE_PROTOCOL {
    EFI_LOAD_FILE LoadFile;
} EFI_LOAD_FILE_PROTOCOL;

#endif /* INCLUDE_EFI_PROTOCOLS_LOAD_FILE_H_ */
