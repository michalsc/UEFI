#include <efi/system.h>
#include <support.h>

static const CHAR8 __attribute__((used,section(".text"))) id[] = IDSTRING;

EFI_STATUS efi_main(EFI_HANDLE imageHandle, EFI_SYSTEM_TABLE *systemTable)
{
    EfiSetup(imageHandle, systemTable);
    
    kprintf(u"Starting %S\r\n", &id[6]);
    kprintf(u"efi_main address: %p\r\n", &efi_main);

    for (int i=0; i < 100; i++) {
        kprintf(u"Waiting %d.%d\r", i / 10, i % 10);
        gBS->Stall(100000);
    }
    kprintf(u"\nNothing more here...\r\n");

    return 0;
}
