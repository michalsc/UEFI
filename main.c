#include <efi/system.h>
#include <support.h>

EFI_RUNTIME_SERVICES *gRS;
EFI_SYSTEM_TABLE *gST;
EFI_BOOT_SERVICES *gBS;

EFI_STATUS efi_main(EFI_HANDLE imageHandle, EFI_SYSTEM_TABLE *system_table)
{
    gST = system_table;
    gBS = gST->BootServices;
    gRS = gST->RuntimeServices;

    kprintf(u"Hello, World!\r\n");
    kprintf(u"efi_main address: %p\r\n", &efi_main);

    gBS->Stall(10000000);

    return 0;
}
