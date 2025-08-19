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
#define OTA_TIMEOUT_MS      3000
#define OTA_WRITE_BUF_SIZE 8

#define CAN_ERR_FLASH_ERASE     0x00            /* FLASH 擦除出错 */
#define CAN_ERR_FLASH_WRITE     0x01            /* FLASH 写出错 */
#define CAN_ERR_UNEXPECTED_PKT  0x03            /* 包号不匹配 */
#define CAN_ERR_LEN_INVALID     0x04            /* 固件长度不合适 */
#define CAN_ERR_CRC_MISMATCH    0x05            /* CRC校验码不正确 */
#define CAN_ERR_OVER_TIME       0x06            /* 接收数据超时 */

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


/* 串口ota  */
// typedef enum {
//     OTA_STATE_IDLE = 0,
//     OTA_STATE_HEADER,
//     OTA_STATE_RECEIVING,
//     OTA_STATE_CRC,
//     OTA_STATE_DONE,
//     OTA_STATE_ERROR
// } ota_state_t;


/* can ota*/
typedef enum {
    OTA_STATE_IDLE,        // 空闲（等待信息包）
    OTA_STATE_RECEIVING,   // 接收中（等待数据包）
    OTA_STATE_FINISHED,     // 接收完成（等待 CRC 校验）
    OTA_STATE_ERROR
} OtaState;

typedef struct {
    OtaState   state;          // 当前状态
    uint16_t   current_packet; // 期望的下一个数据包号（大端）
    uint32_t   firmware_len;   // 固件总长度（来自信息包）
    uint16_t   firmware_crc16; // 固件 CRC16（来自信息包）
    uint32_t   flash_addr;     // Flash 写入基地址（如 BACKUP_ADDR）
    uint32_t   received_len;   // 已接收的固件数据长度
    uint8_t write_buffer[OTA_WRITE_BUF_SIZE];
    uint8_t buffer_len;
    uint32_t last_recv_tick;     // 上次接收时间
    uint8_t retry_count;       // 当前包的重传计数
    uint8_t max_retries;        // 最大重传次数
    uint16_t current_tx_packet;
    uint32_t tx_offset;
    uint16_t current_chunk_len;
    uint16_t current_sending_packet; // 当前正在发送的包号（info包为0x0000）
} OtaContext;

// Modbus协议常量
#define MODBUS_DEVICE_ADDR   0x01
#define MODBUS_FUNC_CODE     0x40  // 可选 0x40 / 0x41 / 0x42

extern OtaContext ota_ctx;

// extern ota_state_t ota_state;
uint32_t ring_buffer_read(uint8_t* dest, uint32_t max_len);
void ota_reset_state(void);
void set_ota_flag(void) ;
uint8_t flash_write(uint32_t addr, uint8_t *data, uint32_t len);
uint8_t flash_erase(uint32_t addr, uint32_t size);
uint32_t ota_calc_crc32(uint32_t flash_addr, uint32_t len);
uint16_t modbus_crc16(uint8_t *data, uint16_t length);
#endif
