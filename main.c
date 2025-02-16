#include <efi/system.h>
#include <efi/protocols/graphics.h>
#include <efi/protocols/loaded_image.h>
#include <efi/protocols/simple_file_system.h>
#include <efi/protocols/file.h>
#include <efi/protocols/load_file.h>
#include <support.h>
#include <libdeflate.h>

#include <logo.h>

static const CHAR8 __attribute__((used,section(".text"))) id[] = IDSTRING;

static inline EFI_GRAPHICS_OUTPUT_BLT_PIXEL getColor(int k, char mode)
{
    EFI_GRAPHICS_OUTPUT_BLT_PIXEL pix;

    if (mode == 'b')
    {
        int g = ((0x90 - k) * 5) / 4;
        if (g < 0)
            g = 0;
        if (g > 255)
            g = 255;

        pix.Blue = g;
        pix.Red = g;
        pix.Green = g;
    }
    else if (mode == 'p')
    {
        k = 264 - k;
        int r = -330, g = -343, b = -91;
        r += (k * 848) >> 8;
        g += (k * 768) >> 8;
        b += (k * 341) >> 8;

        if (r < 0)
            r = 0;
        if (g < 0)
            g = 0;
        if (b < 0)
            b = 0;
        if (r > 255)
            r = 255;
        if (g > 255)
            g = 255;
        if (b > 255)
            b = 255;

        pix.Red = r;
        pix.Green = g;
        pix.Blue = b;
    }
    else
    {
        pix.Blue = k;
        pix.Red = k;
        pix.Green = k;
    }

    return pix;
}

EFI_STATUS init_graphics(EFI_HANDLE imageHandle)
{
    EFI_GUID gop = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
    UINTN noHandles = 0;
    EFI_HANDLE *handleBuffer = NULL;
    EFI_GRAPHICS_OUTPUT_PROTOCOL *graphics;

    // Let EFI allocate all handles supporting GOP protocol
    EFI_STATUS stat = EfiLocateHandleBuffer(ByProtocol, &gop, NULL, &noHandles, &handleBuffer);

    kprintf(u"Number of GOP handles: %d\r\n", noHandles);
    kprintf(u"Buffer for handles: %p\r\n", handleBuffer);

    kprintf(u"Status: %016lx\r\n", stat);

    EFI_GRAPHICS_OUTPUT_BLT_PIXEL *logo;

    int pos = 4;
    int logo_width = logo_rle[0] | (logo_rle[1] << 8);
    int logo_height = logo_rle[2] | (logo_rle[3] << 8);

    EfiAllocatePool(EfiLoaderData, logo_width * logo_height * sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL), (VOID**)&logo);
    
    char color_mode = 0; // 'b'lack, 'p'urple;

    int outpos = 0;
    while(pos < logo_rle_len) {
        int count = 0;
        int diff = 0;
        unsigned char c = logo_rle[pos++];
        if (c & 0x40) diff = 1;
        count = c & 0x3f;
        while (c & 0x80) {
            count <<= 7;
            c = logo_rle[pos++];
            count |= c & 0x7f;
        }
        if (diff) {
            for (int i = 0; i < count; i++) {
                logo[outpos] = getColor(logo_rle[pos++], color_mode);
                outpos++;
            }
        } else {
            EFI_GRAPHICS_OUTPUT_BLT_PIXEL pix = getColor(logo_rle[pos++], color_mode);

            for (int i=0; i < count; i++) {
                logo[outpos] = pix;
                outpos++;
            }
        }
    }

    noHandles = 1;
    if (stat == EFI_SUCCESS) {
        for (int i=0; i < noHandles; i++) {
            stat = EfiOpenProtocol(handleBuffer[i], &gop, (void**)&graphics, imageHandle, NULL, EFI_OPEN_PROTOCOL_GET_PROTOCOL);
            kprintf(u"EfiOpenProtocol for handle %d (%p) returned %016lx, graphics %p\r\n", i, handleBuffer[i], stat, graphics);

            if (stat == EFI_SUCCESS) {
                kprintf(u"  MaxMode number: %d, Current mode: %d\r\n", graphics->Mode->MaxMode, graphics->Mode->Mode);
                kprintf(u"  Resolution: %dx%d\r\n", graphics->Mode->Info->HorizontalResolution, graphics->Mode->Info->VerticalResolution);
                kprintf(u"  Framebuffer at %p, size %d, pitch %d\r\n", graphics->Mode->FrameBufferBase, graphics->Mode->FrameBufferSize,
                    graphics->Mode->Info->PixelsPerScanLine);
                
                kprintf(u"  Mode List:\r\n");
                for (int i=0; i < graphics->Mode->MaxMode; i++) {
                    EFI_GRAPHICS_OUTPUT_MODE_INFORMATION *info;
                    UINTN infoSize;
                    graphics->QueryMode(graphics, i, &infoSize, &info);
                    kprintf(u"    %2d: %dx%d\r\n", i, info->HorizontalResolution, info->VerticalResolution);

                }

                stat = graphics->SetMode(graphics, graphics->Mode->Mode);
                
                if (stat != EFI_SUCCESS)
                    continue;

                kprintf(u"  MaxMode number: %d, Current mode: %d\r\n", graphics->Mode->MaxMode, graphics->Mode->Mode);
                kprintf(u"  Resolution: %dx%d\r\n", graphics->Mode->Info->HorizontalResolution, graphics->Mode->Info->VerticalResolution);
                kprintf(u"  Framebuffer at %p, size %d, pitch %d\r\n", graphics->Mode->FrameBufferBase, graphics->Mode->FrameBufferSize,
                        graphics->Mode->Info->PixelsPerScanLine);

                graphics->Blt(graphics, logo, EfiBltVideoFill, 0, 0, 0, 0, graphics->Mode->Info->HorizontalResolution,
                    graphics->Mode->Info->VerticalResolution, 0);
                UINTN x = (graphics->Mode->Info->HorizontalResolution - logo_width) / 2;
                UINTN y = (graphics->Mode->Info->VerticalResolution - logo_height) / 2;
                graphics->Blt(graphics, logo, EfiBltBufferToVideo, 0, 0, x, y, logo_width, logo_height, 0);

                break;
            }
        }
        EfiFreePool(handleBuffer);
    }

    return stat;
}

static void *my_malloc(uint64_t size) {
    void *buffer = NULL;
    if (EfiAllocatePool(EfiLoaderData, size, &buffer) == EFI_SUCCESS) {
        return buffer;
    } else {
        return NULL;
    }
}

static void my_free(void * buffer) {
    EfiFreePool(buffer);
}

EFI_STATUS checkBigEndian(void)
{
    uint64_t tmp, reg;

    __asm__ volatile(
        "mrs %0, SCTLR_EL1                      \n\t"
        "orr %1, %0, #(1 << 24)                 \n\t"
        "msr SCTLR_EL1, %1                      \n\t"
        "mrs %1, SCTLR_EL1                      \n\t"
        "msr SCTLR_EL1, %1                      \n\t"
        :"=r"(tmp), "=r"(reg)
    );

    if ((reg & (((1 << 24)))) == (1 << 24))
        return EFI_SUCCESS;
    else
        return EFI_UNSUPPORTED;
}

EFI_STATUS efi_main(EFI_HANDLE imageHandle, EFI_SYSTEM_TABLE *systemTable)
{
    EfiSetup(imageHandle, systemTable);
    EFI_GUID lip = EFI_LOADED_IMAGE_PROTOCOL_GUID;
    EFI_GUID sfs = EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID;
    uint64_t reg;
    EFI_LOADED_IMAGE_PROTOCOL *loadedImageProtocol = NULL;
    EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *simpleFileSystemProtocol = NULL;
    EFI_FILE_PROTOCOL *fileProtocol = NULL;
    EFI_HANDLE bootDevice;

    kprintf(u"Starting %S\r\n", &id[6]);

    __asm__ volatile("mrs %0, CurrentEL":"=r"(reg));

    kprintf(u"Current level: EL%d\r\n", (reg >> 2) & 3);

    kprintf(u"Checking BigEndian support: ");
    if (checkBigEndian() == EFI_SUCCESS)
        kprintf(u"supported\r\n");
    else
        kprintf(u"unsupported\r\n");

    EfiOpenProtocol(imageHandle, &lip, (void**)&loadedImageProtocol, imageHandle, NULL, EFI_OPEN_PROTOCOL_BY_HANDLE_PROTOCOL);

    kprintf(u"Boot Device handle: %016lx\r\n", loadedImageProtocol->DeviceHandle);
    bootDevice = loadedImageProtocol->DeviceHandle;
    
    EfiOpenProtocol(bootDevice, &sfs, (void**)&simpleFileSystemProtocol, imageHandle, NULL, EFI_OPEN_PROTOCOL_BY_HANDLE_PROTOCOL);
    if (simpleFileSystemProtocol != NULL) {
        kprintf(u"SFS protocol: %016lx\r\n", simpleFileSystemProtocol);

        simpleFileSystemProtocol->OpenVolume(simpleFileSystemProtocol, &fileProtocol);
    
        kprintf(u"File protocol: %016lx\r\n", fileProtocol);    
    }
    else
    {
        EFI_GUID lfg = EFI_LOAD_FILE_PROTOCOL_GUID;
        EFI_LOAD_FILE_PROTOCOL *loadFile;

        kprintf(u"Boot device without simple file system protocol, trying LOAD_FILE_PROTOCOL instead \r\n");

        EfiOpenProtocol(bootDevice, &lfg, (void**)&loadFile, imageHandle, NULL, EFI_OPEN_PROTOCOL_BY_HANDLE_PROTOCOL);

        kprintf(u"Load File protocol: %016lx\r\n", loadFile);
    }

    EfiStall(5000000);

    libdeflate_set_memory_allocator(my_malloc, my_free);

    init_graphics(imageHandle);

    for (int i=0; i <= 50; i++) {
        kprintf(u"Waiting %d.%d\r", i / 10, i % 10);
        EfiStall(100000);
    }
    kprintf(u"\nNothing more here...\r\n");

    return 0;
}
