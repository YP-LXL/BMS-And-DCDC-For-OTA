#ifndef TEST_TASK_H
#define	TEST_TASK_H

#include <stdio.h>
#include <stdint.h>
#include <FreeRTOS.h>
#include <task.h>
#include "sh367309.h"
#include <gd32f4xx_can.h>
#include "project_config.h"
// #include "gateway_communicate.h"
#include "board.h" 
#include "parameter.h"
// #include "period_task.h"
void test_task(void *pvParameters);
void ota_task(void *pvParameters);
void counttest_task(void* pvParameters);
uint8_t spi_sector_test(void);
uint8_t spi_test(void);
uint8_t test_block_operation();
uint8_t test_block_full_operation(uint32_t block_num);
uint8_t Hash_test(void);
uint8_t test_chip_full_operation(void); 
void generate_test_data(uint8_t *buffer, uint32_t block_num, uint32_t len);
#endif
