#include "main.h"

/*
Flash layout:

0x08000000 - 0x0803FFFF sector 0 16k: 16KB bootloader

0x08020000 - 0x0805FFFF sector 5-6 128k: 256KB active application

0x08060000 - 0x0809FFFF sector 7-8 128k: 256KB new firmware update area

*/

constexpr uint32_t application_addr = 0x08020000; // Start address of the application in flash memory
constexpr uint32_t new_image_addr = 0x08060000;   // Start address of the new image in flash memory

uint32_t* image_header = (uint32_t*)new_image_addr;

void unlock_flash(){
    // unlock the flash memory for writing
    if(FLASH->CR & FLASH_CR_LOCK){
        FLASH->KEYR = 0x45670123; // unlock key 1
        FLASH->KEYR = 0xCDEF89AB; // unlock key 2
    }
}

void lock_flash(){
    // lock the flash memory after writing
    FLASH->CR |= FLASH_CR_LOCK;
}

void erase_application_flash() {

    // sector 5
    while(FLASH->SR & FLASH_SR_BSY); // wait for flash to not be busy

    FLASH->CR &= ~FLASH_CR_PG; // clear the programming bit
    FLASH->CR |= FLASH_CR_SER; // set the sector erase bit

    FLASH->CR &= ~FLASH_CR_SNB; // clear the sector number bits
    FLASH->CR |= (5 << FLASH_CR_SNB_Pos); // set the sector number to 5

    FLASH->CR |= FLASH_CR_STRT; // start the erase operation

    while(FLASH->SR & FLASH_SR_BSY); // wait for flash to finish erasing
    FLASH->CR &= ~FLASH_CR_SER; // clear the sector erase bit

    // sector 6
    while(FLASH->SR & FLASH_SR_BSY); // wait for flash to not be busy

    FLASH->CR &= ~FLASH_CR_PG; // clear the programming bit
    FLASH->CR |= FLASH_CR_SER; // set the sector erase bit

    FLASH->CR &= ~FLASH_CR_SNB; // clear the sector number bits
    FLASH->CR |= (6 << FLASH_CR_SNB_Pos); // set the sector number to 5

    FLASH->CR |= FLASH_CR_STRT; // start the erase operation

    while(FLASH->SR & FLASH_SR_BSY); // wait for flash to finish erasing
    FLASH->CR &= ~FLASH_CR_SER; // clear the sector erase bit
}

void erase_update_flash() {

    // sector 7
    while(FLASH->SR & FLASH_SR_BSY); // wait for flash to not be busy

    FLASH->CR &= ~FLASH_CR_PG; // clear the programming bit
    FLASH->CR |= FLASH_CR_SER; // set the sector erase bit

    FLASH->CR &= ~FLASH_CR_SNB; // clear the sector number bits
    FLASH->CR |= (7 << FLASH_CR_SNB_Pos); // set the sector number to 5

    FLASH->CR |= FLASH_CR_STRT; // start the erase operation

    while(FLASH->SR & FLASH_SR_BSY); // wait for flash to finish erasing
    FLASH->CR &= ~FLASH_CR_SER; // clear the sector erase bit

    // sector 8
    while(FLASH->SR & FLASH_SR_BSY); // wait for flash to not be busy

    FLASH->CR &= ~FLASH_CR_PG; // clear the programming bit
    FLASH->CR |= FLASH_CR_SER; // set the sector erase bit

    FLASH->CR &= ~FLASH_CR_SNB; // clear the sector number bits
    FLASH->CR |= (8 << FLASH_CR_SNB_Pos); // set the sector number to 5

    FLASH->CR |= FLASH_CR_STRT; // start the erase operation

    while(FLASH->SR & FLASH_SR_BSY); // wait for flash to finish erasing
    FLASH->CR &= ~FLASH_CR_SER; // clear the sector erase bit
}

void write_flash(uint32_t addr, void* src, uint32_t word_count) {
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

void reset() {
	SCB->AIRCR = (1 << SCB_AIRCR_SYSRESETREQ_Pos) | (0x5FA << SCB_AIRCR_VECTKEY_Pos); // reset the device
}

bool check_crc(void* src, uint32_t len) {
    // TODO
    return true;
}

void chainload(uint32_t offset) {
    SCB->VTOR = offset; // Set the vector table offset register to the new application address
	
    // Set the stack pointer to the new application
    asm volatile("ldr r1, [%0]; mov sp, r1; ldr %0, [%0, #4]; bx %0" :: "r" (offset) : "r1");

    while (true);   // should never reach here
}

int main(void) {
    // Jump to application if there's no new image.
    if (image_header[0] == 0xffffffff) {
        chainload(application_addr);
    }

    unlock_flash();

    // set flash programming size to x32 (max for our configuration)
    FLASH->CR &= ~FLASH_CR_PSIZE; // clear PSIZE bits
    FLASH->CR |= (0b10 << FLASH_CR_PSIZE_Pos); // set PSIZE to x32


    // Validate image.
    if(check_crc(image_header + 8, *image_header)) {
        erase_application_flash(); // erase the application flash sectors

        uint32_t image_size = *image_header; // get the image size from the header
        image_size += image_size % 4; // make sure the image size is a multiple of 4

        write_flash(application_addr, (uint8_t*)image_header + 8, image_size/4); // write the new firmware to application flash memory
    }

    erase_update_flash();
    reset();
}