#include "ota.h"


ota_state_t ota_state = OTA_STATE_IDLE;

uint8_t header_buf[4];
uint32_t header_received = 0;
uint8_t crc_buf[4];
uint32_t crc_received = 0;
uint8_t ota_block[OTA_BLOCK_SIZE];
uint8_t* ota_data_ptr = ota_block;
uint32_t block_idx = 0;
uint32_t total_received = 0;
uint32_t expected_len = 0;
uint32_t expected_crc = 0;
uint32_t actual_crc = 0;

volatile bool ota_done = false;
volatile uint32_t ota_last_tick = 0;
volatile uint8_t time_flag = 0;

uint8_t ring_buffer[RING_BUFFER_SIZE];
volatile uint32_t ring_head = 0;
volatile uint32_t ring_tail = 0;

uint32_t i = 0;
uint32_t pages = 0;
uint32_t data_len  =0;

uint32_t ota_calc_crc32(uint32_t flash_addr, uint32_t len) {
    crc_data_register_reset();
    for (i = 0; i < len; i += 4) {
        uint32_t data = *(uint32_t*)(flash_addr + i);
        crc_single_data_calculate(data);
    }
    return crc_data_register_read();
}

void flash_erase(uint32_t addr, uint32_t size) {
    pages = (size + 0xFFF) / 0x1000;
    fmc_unlock();
    for ( i = 0; i < pages; i++) {
        fmc_page_erase(addr + i * 0x1000);
    }
    fmc_lock();
}

void flash_write(uint32_t addr, uint8_t *data, uint32_t len) {
    fmc_unlock();
    for ( i = 0; i < len; i += 4) {
        uint32_t word = 0xFFFFFFFF;
        memcpy(&word, data + i, (len - i) >= 4 ? 4 : (len - i));
        fmc_word_program(addr + i, word);
    }
    fmc_lock();
}

void set_ota_flag(void) {
    uint32_t temp_buffer[FLASH_PAGE_SIZE / 4];
    uint32_t offset = 0; 
    uint32_t page_addr = OTA_FLAG_ADDR;

    for (i = 0; i < FLASH_PAGE_SIZE / 4; i++) {
        temp_buffer[i] = *(uint32_t *)(page_addr + i * 4);
    }

    temp_buffer[offset] = OTA_FLAG_VALUE;

    fmc_unlock();
    fmc_page_erase(page_addr);
		
    for (i = 0; i < FLASH_PAGE_SIZE / 4; i++) {
        fmc_word_program(page_addr + i * 4, temp_buffer[i]);
    }
    fmc_lock();
}

uint32_t ring_buffer_read(uint8_t* dest, uint32_t max_len) {
    uint32_t count = 0;
    while (ring_tail != ring_head && count < max_len) {
        dest[count++] = ring_buffer[ring_tail];
        ring_tail = (ring_tail + 1) % RING_BUFFER_SIZE;
    }
    return count;
}

void ota_reset_state(void) {
    ota_state = OTA_STATE_HEADER;
	time_flag = 0;
    header_received = 0;
    crc_received = 0;
    expected_len = 0;
    expected_crc = 0;
    actual_crc = 0;
    total_received = 0;
    block_idx = 0;
    ota_data_ptr = ota_block;
    ota_done = false;
    memset(header_buf, 0, sizeof(header_buf));
    memset(crc_buf, 0, sizeof(crc_buf));
    memset(ota_block, 0xFF, sizeof(ota_block));
}
