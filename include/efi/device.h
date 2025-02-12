#ifndef INCLUDE_EFI_DEVICE_H_
#define INCLUDE_EFI_DEVICE_H_

#include <efi/types.h>

//*******************************************************
// EFI_DEVICE_PATH_PROTOCOL
//*******************************************************
typedef struct _EFI_DEVICE_PATH_PROTOCOL
{
    UINT8 Type;
    UINT8 SubType;
    UINT8 Length[2];
} EFI_DEVICE_PATH_PROTOCOL;

#endif // INCLUDE_EFI_DEVICE_H_
