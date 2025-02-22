message("Building virtual boot disk in ${CMAKE_INSTALL_PREFIX}")

set(TARGET_IMAGE ${CMAKE_INSTALL_PREFIX}/efi/boot.img)
set(GPT_IMAGE ${CMAKE_BINARY_DIR}/boot.img)
set(CD_FOLDER ${CMAKE_BINARY_DIR}/temp)
set(FAT_IMAGE ${CD_FOLDER}/image.fat)

#message("-- Creating file system")

#execute_process(
#    COMMAND mkdir ${CD_FOLDER}
#    COMMAND dd if=/dev/zero of=${FAT_IMAGE} bs=1M count=2
#    COMMAND mkfs.vfat -s 2 -M 0xf8 -a -F 12 -n BOOT ${FAT_IMAGE}
#    OUTPUT_QUIET
#    ERROR_QUIET
#)

#message("-- Copying files")

#execute_process(
#    COMMAND mmd -i ${FAT_IMAGE} ::EFI
#    COMMAND mmd -i ${FAT_IMAGE} ::EFI/BOOT
#    COMMAND mcopy -i ${FAT_IMAGE} -s ${CMAKE_BINARY_DIR}/BOOTaa64.EFI ::EFI/BOOT/BOOTaa64.EFI
#    OUTPUT_QUIET
#    ERROR_QUIET
#)

#message("-- Making image")

#execute_process(
#    COMMAND xorrisofs -o ${GPT_IMAGE} 
#        -iso-level 3
#        -full-iso9660-filenames
#        -boot-load-size 4
#        -boot-info-table
#        -eltorito-platform efi
#        --eltorito-catalog image.cat
#        -no-emul-boot -eltorito-boot image.fat ${CD_FOLDER}
#    #${CD_FOLDER}
#    #COMMAND xorrisofs -o ${GPT_IMAGE} -R -J -v -d -N -hide-rr-moved -no-emul-boot -eltorito-platform efi -eltorito-boot image.fat ${CD_FOLDER}
#)

execute_process(
    COMMAND dd if=/dev/zero of=${GPT_IMAGE} bs=1M count=3
#    OUTPUT_QUIET
#    ERROR_QUIET
)

execute_process(
    COMMAND parted -a none ${GPT_IMAGE} mklabel gpt
#    OUTPUT_QUIET
#    ERROR_QUIET
)

execute_process(
    COMMAND parted -a none ${GPT_IMAGE} mkpart ESP fat32 32KiB 2MiB
#    OUTPUT_QUIET
#    ERROR_QUIET
)

execute_process(
    COMMAND parted -a none ${GPT_IMAGE} set 1 boot on
#    OUTPUT_QUIET
#    ERROR_QUIET
)

execute_process(
#    COMMAND mkdir ${CD_FOLDER}
#    COMMAND dd if=/dev/zero of=${FAT_IMAGE} bs=1M count=2
    COMMAND mkfs.vfat -F 12  ${GPT_IMAGE} --offset 64 2048
#    OUTPUT_QUIET
#    ERROR_QUIET
)

#execute_process(
#    COMMAND mmd -i ${GPT_IMAGE} -o 32768 ::EFI
#    COMMAND mmd -i ${GPT_IMAGE} -o 32768 ::EFI/BOOT
#    COMMAND mcopy -i ${GPT_IMAGE} -o 32768 -s ${CMAKE_BINARY_DIR}/BOOTaa64.EFI ::EFI/BOOT/BOOTaa64.EFI
#    OUTPUT_QUIET
#    ERROR_QUIET
#)

#execute_process(
#    COMMAND sgdisk -g -a 1 -n 1:48:+32M -c 1:BOOT\ Partition -t 1:ef00 ${GPT_IMAGE}
#    OUTPUT_QUIET
#    ERROR_QUIET
#)

#execute_process(
#    COMMAND dd if=${FAT_IMAGE} of=${GPT_IMAGE} bs=1k seek=24 conv=notrunc
#    OUTPUT_QUIET
#    ERROR_QUIET
#)

#file(COPY
#    ${GPT_IMAGE} DESTINATION ${CMAKE_INSTALL_PREFIX}/efi/
#)
