#include "firmware_update.h"
#include "memory"

void firmware_update::unlock_flash(){
    // unlock the flash memory for writing
    if(FLASH->CR & FLASH_CR_LOCK){
        FLASH->KEYR = 0x45670123; // unlock key 1
        FLASH->KEYR = 0xCDEF89AB; // unlock key 2
    }
}

void firmware_update::lock_flash(){
    // lock the flash memory after writing
    FLASH->CR |= FLASH_CR_LOCK;
}

void firmware_update::erase_flash() {

    // sector 7
    for(uint32_t i=0x08060000; i<0x0807FFFF; i+=4) {
        if(*(uint32_t*)i != 0xFFFFFFFF) {
            // non-erased data found, erase the sector

            while(FLASH->SR & FLASH_SR_BSY); // wait for flash to not be busy

            FLASH->CR &= ~FLASH_CR_PG; // clear the programming bit
            FLASH->CR |= FLASH_CR_SER; // set the sector erase bit

            FLASH->CR &= ~FLASH_CR_SNB; // clear the sector number bits
            FLASH->CR |= (7 << FLASH_CR_SNB_Pos); // set the sector number to 5

            FLASH->CR |= FLASH_CR_STRT; // start the erase operation

            while(FLASH->SR & FLASH_SR_BSY); // wait for flash to finish erasing
            FLASH->CR &= ~FLASH_CR_SER; // clear the sector erase bit

            break; // no need to check other addresses in the sector after erasing
        }
    }

    // sector 8
    for(uint32_t i=0x08080000; i<0x0809FFFF; i+=4) {
        if(*(uint32_t*)i != 0xFFFFFFFF) {
            // non-erased data found, erase the sector

            while(FLASH->SR & FLASH_SR_BSY); // wait for flash to not be busy

            FLASH->CR &= ~FLASH_CR_PG; // clear the programming bit
            FLASH->CR |= FLASH_CR_SER; // set the sector erase bit

            FLASH->CR &= ~FLASH_CR_SNB; // clear the sector number bits
            FLASH->CR |= (8 << FLASH_CR_SNB_Pos); // set the sector number to 5

            FLASH->CR |= FLASH_CR_STRT; // start the erase operation

            while(FLASH->SR & FLASH_SR_BSY); // wait for flash to finish erasing
            FLASH->CR &= ~FLASH_CR_SER; // clear the sector erase bit

            break; // no need to check other addresses in the sector after erasing
        }
    } 
}

void firmware_update::write_flash(uint32_t addr, void* src, uint32_t word_count) {
    uint32_t* srcp = (uint32_t*)src;
    uint32_t* dstp = (uint32_t*)addr;

    while(FLASH->SR & FLASH_SR_BSY); // wait for flash to not be busy

    FLASH->CR |= FLASH_CR_PG; // set the programming bit
    FLASH->CR &= ~FLASH_CR_SER; // clear the sector erase bit

    for (uint32_t i = 0; i < word_count; i++) {

        ARM_MPU_OrderedMemcpy(dstp+i, srcp+i, 1); // write the data to flash memory

        while(FLASH->SR & FLASH_SR_BSY); // wait for the write to complete
    }

    FLASH->CR &= ~FLASH_CR_PG; // clear the programming bit
}

void firmware_update::init() {
    // initialize the firmware update module

    while(FLASH->SR & FLASH_SR_BSY); // wait for flash to not be busy

    unlock_flash();

    // set flash programming size to x32 (max for our configuration)
    FLASH->CR &= ~FLASH_CR_PSIZE; // clear PSIZE bits
    FLASH->CR |= (0b10 << FLASH_CR_PSIZE_Pos); // set PSIZE to x32

    erase_flash();
    lock_flash();
}

uint32_t firmware_update::write_words(uint32_t addr, uint64_t* data, uint8_t size) {
    // size is in bytes, must be a multiple of 4 (32 bits)

    if(size & 0b11){ // check if size is a multiple of 4
        return 1; // error
    }

    if(addr+size > image_size){
        image_size = addr+size; // update the image size
    }

    unlock_flash();

    write_flash(addr + 2*4 + flash_start_address, data, size / 4);

    lock_flash();

    return 0;
}

void firmware_update::reset_to_new_firmware() {

    if(image_size == 0 || image_size > 0x40000) {
        // invalid image size
        image_size = 0;
        unlock_flash();
        erase_flash(); // erase so we don't end up booting into it
        lock_flash();
        return;
    }

    // write image size to the first 4 bytes of the new firmware
    unlock_flash();
    write_flash(flash_start_address, &image_size, 2); // write the image size to the first 8 bytes of the new firmware
    lock_flash();

    // reset the device
    SCB->AIRCR = (1 << SCB_AIRCR_SYSRESETREQ_Pos) | (0x5FA << SCB_AIRCR_VECTKEY_Pos); // reset the device
}