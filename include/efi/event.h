#ifndef INCLUDE_EFI_EVENT_H_
#define INCLUDE_EFI_EVENT_H_

#include <efi/types.h>

//*******************************************************
// EFI_EVENT
//*******************************************************
typedef VOID *EFI_EVENT;

//*******************************************************
// Event Types
//*******************************************************
// These types can be “ORed” together as needed – for example,
// EVT_TIMER might be “Ored” with EVT_NOTIFY_WAIT or
// EVT_NOTIFY_SIGNAL.
#define EVT_TIMER 0x80000000
#define EVT_RUNTIME 0x40000000
#define EVT_NOTIFY_WAIT 0x00000100
#define EVT_NOTIFY_SIGNAL 0x00000200
#define EVT_SIGNAL_EXIT_BOOT_SERVICES 0x00000201
#define EVT_SIGNAL_VIRTUAL_ADDRESS_CHANGE 0x60000202

//*******************************************************
// EFI_EVENT_NOTIFY
//*******************************************************
typedef VOID(EFIAPI *EFI_EVENT_NOTIFY)(
    IN EFI_EVENT Event,
    IN VOID *Context);
    
#endif // INCLUDE_EFI_EVENT_H_
