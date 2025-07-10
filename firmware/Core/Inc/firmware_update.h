#include "stm32f413xx.h"
#include "stdint.h"
#include "device_descriptor.h"

/*
Flash layout:

0x08000000 - 0x0803FFFF sector 0 16k: 16KB bootloader

0x08020000 - 0x0805FFFF sector 5-6 128k: 256KB active application

0x08060000 - 0x0809FFFF sector 7-8 128k: 256KB new firmware update area

*/

class firmware_update{
    public:
        void init();

        uint32_t write_words(uint32_t addr, uint64_t* data, uint8_t size); // write data to flash memory
        void reset_to_new_firmware(); // reset the device to the new firmware

    private:

        const uint32_t flash_start_address = 0x08060000; // start address of the flash memory for the new firmware
        uint64_t image_size = 0; // size of the new firmware image in bytes

        void unlock_flash(); // unlock the flash memory for writing
        void lock_flash(); // lock the flash memory after writing
        void erase_flash(); // erase the flash sectors where the new firmware will be written (if they are not already erased)
        void write_flash(uint32_t addr, void* src, uint32_t word_count); // write the new firmware to flash memory

};