#ifndef OTA_H
#define OTA_H

#include "gd32f4xx.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "gd32f4x.h"


#define BACKUP_ADDR         0x08041000
#define OTA_FLAG_ADDR       0x0807D000
#define OTA_FLAG_VALUE      0xA5A5A5A5
#define OTA_BLOCK_SIZE      4096
#define FLASH_PAGE_SIZE     0x1000
#define RING_BUFFER_SIZE    8192
#define OTA_TIMEOUT_MS      10000


extern uint8_t header_buf[4];
extern uint32_t header_received;
extern uint8_t crc_buf[4];
extern uint32_t crc_received;
extern uint8_t ota_block[OTA_BLOCK_SIZE];
extern uint8_t* ota_data_ptr;
extern uint32_t block_idx;
extern uint32_t total_received;
extern uint32_t expected_len;
extern uint32_t expected_crc;
extern uint32_t actual_crc;

extern volatile bool ota_done;
extern volatile uint32_t ota_last_tick;
extern volatile uint8_t time_flag;

extern uint8_t ring_buffer[RING_BUFFER_SIZE];
extern volatile uint32_t ring_head;
extern volatile uint32_t ring_tail;

extern uint32_t data_len ;
extern uint32_t i;



typedef enum {
    OTA_STATE_IDLE = 0,
    OTA_STATE_HEADER,
    OTA_STATE_RECEIVING,
    OTA_STATE_CRC,
    OTA_STATE_DONE,
    OTA_STATE_ERROR
} ota_state_t;
extern ota_state_t ota_state;
uint32_t ring_buffer_read(uint8_t* dest, uint32_t max_len);
void ota_reset_state(void);
void set_ota_flag(void) ;
void flash_write(uint32_t addr, uint8_t *data, uint32_t len);
void flash_erase(uint32_t addr, uint32_t size);
uint32_t ota_calc_crc32(uint32_t flash_addr, uint32_t len);

#endif
