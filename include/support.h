#ifndef INCLUDE_SUPPORT_H_
#define INCLUDE_SUPPORT_H_

#include <stdarg.h>
#include <efi/types.h>
#include <efi/system.h>

#define GPT_AMIGA_RIGID_DISK_BLOCK \
    { 0x3F82EEBC, 0x87C9, 0x4097, \
    { 0x81, 0x65, 0x89, 0xD6, 0x54, 0x05, 0x57, 0xC0 }}

typedef void (*putc_func)(void *data, CHAR16 c);
void vkprintf_pc(putc_func putc_f, void *putc_data, const CHAR16 *format, va_list args);
void kprintf_pc(putc_func putc_f, void *putc_data, const CHAR16 *format, ...);
void vkprintf(const CHAR16 *format, va_list args);
void kprintf(const CHAR16 *format, ...);

void EfiSetup(EFI_HANDLE imageHandle, EFI_SYSTEM_TABLE *system);

#ifndef IDSTRING
#define IDSTRING "idstring"
#endif

#endif // INCLUDE_SUPPORT_H_
