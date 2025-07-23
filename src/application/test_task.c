#include "test_task.h"

#define TEST_PCBA       1   /* pcba  使能 */

#if TEST_PCBA
#include "gd32f4x.h"
#include "atcommand.h"
#include "gd25q32esig.h"

/*  单个地址测试  */
#define TEST_ADDRESS 0x00001000  // 测试地址，确保该地址未被使用
#define BUFFER_SIZE 1

/*  扇区测试    */

#define TEST_SECTOR_NUM 0        // 要测试的扇区号
#define SECTOR_SIZE     4096     // 扇区大小
#define TEST_DATA_PATTERN 0xAA   // 测试数据

/* 块测试 */
#define BLOCK_SIZE 0x10000

/* 整片测试 */
#define TOTAL_SIZE 0x400000  // 总容量4MB（4,194,304字节）
#define BLOCK_SIZE 0x10000   // 块大小64KB（65536字节）
#define TOTAL_BLOCKS 64      // 总块数（4MB / 64KB = 64块）

#endif

/* 调试接口接收 */
uint8_t debug_read_buf[1024] = {0};
uint8_t debug_read_flag  = 0;
uint8_t spi_flag = 1;

/*  SPI 单地址调试接口  */
uint8_t write_data[BUFFER_SIZE] = {0x01};  // 待写入的数据：0x01
uint8_t read_data[BUFFER_SIZE] = {0};     // 用于存储读取的数据
uint8_t status;

/*  SPI 扇区测试接口*/
// uint8_t write_buf[SECTOR_SIZE];  // 扇区大小的写入缓冲区
// uint8_t read_buf[SECTOR_SIZE];   // 扇区大小的读取缓冲区
// uint32_t sector_addr;            // 扇区起始地址
// uint8_t ret;
// uint32_t i;
// bool verify_ok = true;


void debug_recive(uint8_t* p_data, uint32_t len) 
{
    debug_read_flag = 1;
    memcpy(debug_read_buf, p_data, len);
}

/**
 * @函数名称: test_task()
 * @功能描述: 测试任务
 * @输入: pvParameters
 * @输出: 无
 * @返回值: 无
 */
void test_task(void* pvParameters)
{
    printf("2222222\r\n");
    while (1) 
    {
        /* code */
        
        if (debug_read_flag) {
            debug_read_flag = 0;
            ATcmd_server();
        }
        if (spi_flag) {
            //spi_test();
            // spi_sector_test();
            //test_single_page_write();
            //Hash_test();
            // test_block_operation(); 
            // uint32_t test_block_num = 0;  // 选择块1进行测试
            // if (test_block_full_operation(test_block_num) == 0) {
            //     printf("pass\r\n");
            // } else {
            //     printf("go home\r\n");
            // }
            //test_chip_full_operation();
            // printf("Hash1: %d\n", hash_string("cell_ov1_alarm"));  
            // printf("Hash2: %d\n", hash_string("cell_iv1_alars"));  
            spi_flag = 0;
        }
        // gpio_bit_set(LED_J49_6_GPIO,LED_J49_6_PIN);
        //printf("test_task running...\r\n");
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

/**
 * @函数名称: ota_task()
 * @功能描述: ota升级任务
 * @输入: pvParameters
 * @输出: 无
 * @返回值: 无
 */
void ota_task(void *pvParameters) {
    static uint8_t temp_buf[512];

    printf("[OTA] Task started\r\n");
    ota_reset_state();

    while (1) {
        uint32_t len = ring_buffer_read(temp_buf, sizeof(temp_buf));
        if (len > 0) {
            usart2_data_process(temp_buf, len);
            ota_last_tick = xTaskGetTickCount();
        }
        
        if (ota_done && ota_state == OTA_STATE_DONE) {
            actual_crc = ota_calc_crc32(BACKUP_ADDR, expected_len);
            printf("[OTA] CRC check: expected=%08X, actual=%08X\r\n", expected_crc, actual_crc);
            if (expected_crc == actual_crc) {
                set_ota_flag();
                vTaskDelay(500);
                NVIC_SystemReset();
            } else {
                printf("[OTA] CRC error. Resetting state.\r\n");
                ota_reset_state();
            }
        }

        if (time_flag && ota_state != OTA_STATE_IDLE && ota_state < OTA_STATE_DONE) {
            if ((xTaskGetTickCount() - ota_last_tick) * portTICK_PERIOD_MS >= OTA_TIMEOUT_MS) {
                printf("[OTA] Timeout. Resetting state.\r\n");
                ota_reset_state();
            }
        }

        vTaskDelay(pdMS_TO_TICKS(10));  // Reduce CPU load
    }
}

/**
 * @函数名称: counttest_task()
 * @功能描述: 测试任务
 * @输入: pvParameters
 * @输出: 无
 * @返回值: 无
 */
void counttest_task(void* pvParameters)
{
    gpio_bit_reset(LED_J49_6_GPIO,LED_J49_6_PIN);
    gpio_bit_reset(LED_J49_7_GPIO,LED_J49_7_PIN);
    vTaskDelay(3000);
    while (1) 
    {
        if(param_ram.bms_key_active_flag == 1) {
            /* code */
            #if 0 /* 测试用 */
            printf("HIGH_VOLTAGE =: %d\r\n", HIGH_VOLTAGE_DRIVE_ENSTATE());
            printf("LOW_VOLTAGE =: %d\r\n",  LOW_VOLTAGE_DRIVE_EN_STATE());
            printf("EN_SW1 =: %d\r\n", EN_SW1_APS_VBT_STATE());
            printf("EN_SW2 =: %d\r\n", EN_SW2_ARC_VBT_STATE());
            printf("EN_PWR =: %d\r\n", EN_PWR_DRIVE_STATE());
            #endif
            /* 6.6 测试的task*/
            gpio_bit_set(LED_J49_6_GPIO,LED_J49_6_PIN);
		    gpio_bit_set(LED_J49_7_GPIO,LED_J49_7_PIN);
            vTaskDelay(500);
		    gpio_bit_reset(LED_J49_6_GPIO,LED_J49_6_PIN);
		    gpio_bit_reset(LED_J49_7_GPIO,LED_J49_7_PIN);
        
        }
        vTaskDelay(500);
    }
}

/**
 * @函数名称: Hash_test()
 * @功能描述: 哈希表映射flash接口测试函数
 * @输入: 无
 * @输出: 无
 * @返回值: 无
 */
uint8_t Hash_test(void)
{
    
    if (parameter_rom_register(&rom_parameter)) {
        printf("successful\r\n");
    } else {
        printf("Error");
    }
    if (0 != parameter_rom_init(&rom_parameter)) {
        printf("Error\r\n");
    } else {
        printf("successful");
    }
    return 0;
    
}

/**
 * @函数名称: spi_test()
 * @功能描述: 一个地址测试函数
 * @输入: 无
 * @输出: 无
 * @返回值: 无
 */
// uint8_t spi_test(void)
// {
//      // 擦除测试扇区（可选，确保数据可写入）
//     spi_flash_erase_sector(TEST_ADDRESS / 4096);           
//     // 写入数据到指定地址
//     status = spi_flash_gd25q32esig_write(TEST_ADDRESS, write_data, BUFFER_SIZE);
//     if (status != 0) {
//         printf("Error: Write operation failed!\n");
//     }

//     printf("Data 0x%02X written to address 0x%08X successfully.\n", write_data[0], TEST_ADDRESS);
//     vTaskDelay(500);

//     status = spi_flash_gd25q32esig_read(TEST_ADDRESS, read_data, BUFFER_SIZE);
//     if (status != 0) {
//         printf("Error: Read operation failed!\n");
//         return -1;
//     }
    
//     // 打印读取结果
//     printf("Data read from address 0x%08X: 0x%02X\n", TEST_ADDRESS, read_data[0]);

//         // 验证数据是否一致
//     if (read_data[0] == write_data[0]) {
//         printf("Verification PASSED: Data matches!\n");
//     } else {
//         printf("Verification FAILED: Data mismatch!\n");
//     }
    
//     printf("SPI Flash Test Complete.\n");
//     return 0;
// }


/**
 * @函数名称: spi_sector_test()
 * @功能描述: 整片扇区地址测试函数
 * @输入: 无
 * @输出: 无
 * @返回值: 无
 */
// uint8_t spi_sector_test(void)
// {
//     uint32_t sector_addr;
//     uint8_t ret;
//     uint32_t i;
//     bool verify_ok = true;  // 初始化验证标志
//     int j = 0;
//     // 声明缓冲区（如果没有在其他地方声明）
//     static uint8_t write_buf[SECTOR_SIZE];
//     static uint8_t read_buf[SECTOR_SIZE];

//     sector_addr = TEST_SECTOR_NUM * SECTOR_SIZE;
//     printf("Starting sector read-write test...\r\n");
//     printf("Test sector: %d, Start address: 0x%06X, Size: %d bytes\r\n", 
//            TEST_SECTOR_NUM, sector_addr, SECTOR_SIZE);
           
//     // 擦除目标扇区（必须步骤，否则写入可能失败）
//     ret = spi_flash_erase_sector(TEST_SECTOR_NUM);
//     if (ret != 0) {
//         printf("Sector erasure failed!\r\n");
//         return;
//     }
//     printf("Sector %d erasure completed\r\n", TEST_SECTOR_NUM);

//     // 填充测试数据（每个字节使用相同pattern，也可设计为递增/随机模式）
//     for (i = 0; i < SECTOR_SIZE; i++) {
//         write_buf[i] = TEST_DATA_PATTERN;  // 填充测试数据
//     }

//     // 向扇区写入数据（使用缓冲区写入函数，自动处理页边界）
//     printf("Writing data to sector...\r\n");
//     ret = spi_flash_buffer_write(sector_addr, write_buf, SECTOR_SIZE);
//     if (ret != SPI_OK) {
//         printf("Sector data write failed (error code: %d)!\r\n", ret);
//         return;
//     }
//     printf("Data written to sector %d (Data value: 0x%02X)\r\n", 
//            TEST_SECTOR_NUM, TEST_DATA_PATTERN);

//     // 使用vTaskDelay延时（确保FreeRTOS已正确初始化）
//     printf("Waiting for write completion...\r\n");
//     vTaskDelay(100);
//     printf("Delay completed, continuing...\r\n");
//     // 从扇区读取数据
//     printf("Reading data from sector...\r\n");
//     ret = spi_flash_gd25q32esig_read(sector_addr, read_buf, SECTOR_SIZE);
//     if (ret != 0) {
//         printf("Sector data read failed (error code: %d)!\r\n", ret);
//         return;
//     }
//     printf("Sector %d data read completed\r\n", TEST_SECTOR_NUM);

//     // 验证读写数据是否一致
//     printf("Verifying data...\r\n");
//     verify_ok = true;  // 重置验证标志
//     for (i = 0; i < SECTOR_SIZE; i++) {
//         if (read_buf[i] != write_buf[i]) {
//             printf("Verification failed! Address 0x%06X: Written 0x%02X, Read 0x%02X\r\n", 
//                    sector_addr + i, write_buf[i], read_buf[i]);
//             verify_ok = false;
//         }
//         else {
//             j++;
//         }
//     }

//     // 输出最终验证结果
//     if (verify_ok && j == 4096) {
//         printf("Sector %d read-write verification successful! All data consistent\r\n", 
//                TEST_SECTOR_NUM);
//     } else {
//         printf("Sector %d read-write verification failed!\r\n", TEST_SECTOR_NUM);
//     }
//     return 0;
// }

/**
 * @函数名称: test_single_page_write()
 * @功能描述: 一页地址测试函数
 * @输入: 无
 * @输出: 无
 * @返回值: 无
 */
// 测试单页写入是否正常
// uint8_t test_single_page_write(void)
// {
//     uint8_t write_buf[256];
//     uint8_t read_buf[256];
//     // 填充测试数据
//     for (int i=0; i<256; i++) write_buf[i] = i;
//     // 擦除0号扇区
//     spi_flash_erase_sector(0);
//     printf("Erased sector 0\r\n");
//     // 写入0号页（地址0x000000）
//     printf("Writing single page...\r\n");
//     spi_flash_gd25q32esig_write(0x000000, write_buf, 256);
//     printf("Single page write done\r\n");
//     // 读取验证
//     spi_flash_gd25q32esig_read(0x000000, read_buf, 256);
//     printf("Read back first 4 bytes: 0x%02X,0x%02X,0x%02X,0x%02X\r\n",
//            read_buf[0], read_buf[1], read_buf[2], read_buf[255]);
//      return 0;
// }



/**
 * @函数名称: est_block_operation()
 * @功能描述: 一块地址中个别地址测试函数
 * @输入: 无
 * @输出: 无
 * @返回值: 无
 */
// uint8_t test_block_operation() {
//     uint32_t block_num = 1;          // 块编号1
//     uint32_t offset_in_block = 0xFFF1;  // 块内偏移地址
//     uint8_t write_data[10] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A};
//     uint8_t read_data[10] = {0};

//     // 1. 向块1的偏移0x100处写入10字节数据
//     uint8_t write_result = spi_flash_write_block(block_num, offset_in_block, write_data, 10);
//     if (write_result == 0) {
//         printf("successful\r\n");
//     } else {
//         printf("error\r\n");
//         return 0;
//     }
//     // spi_flash_erase_chip();
//     // 2. 从块1的偏移0x100处读取10字节数据
//     uint8_t read_result = spi_flash_read_block(block_num, offset_in_block, read_data, 10);
//     if (read_result == 0) {
//         printf("successful\r\n");
//         for (int i = 0; i < 10; i++) {
//             printf("%02X ", read_data[i]);
//         }
//         printf("\r\n");
//     } else {
//         printf("error\r\n");
//     }
//     return 0;
// }

/**
 * 测试整个块的读写操作
 * @param block_num：要操作的块编号（0~63，GD25Q32共64个64KB块）
 * @return 0=成功，1=失败
 */
// uint8_t test_block_full_operation(uint32_t block_num) {
//     uint8_t *write_data = malloc(BLOCK_SIZE);  // 分配64KB缓冲区
//     uint8_t *read_data = malloc(BLOCK_SIZE);   // 分配64KB读取缓冲区
//     if (write_data == NULL || read_data == NULL) {
//         return 1;
//     }

//     // 填充测试数据（例如：递增序列，便于验证）
//     for (uint32_t i = 0; i < BLOCK_SIZE; i++) {
//         write_data[i] = i % 256;  // 0x00, 0x01, ..., 0xFF, 0x00, ...
//     }


//     if (spi_flash_erase_block(block_num) != 0) {
//         free(write_data);
//         free(read_data);
//         return 1;
//     }

//     if (spi_flash_erase_and_write_block(block_num, write_data, BLOCK_SIZE) != 0) {
//         free(write_data);
//         free(read_data);
//         return 1;
//     }

//     uint32_t block_start_addr = block_num * BLOCK_SIZE;  // 块起始地址
//     if (spi_flash_gd25q32esig_read(block_start_addr, read_data, BLOCK_SIZE) != 0) {
//         free(write_data);
//         free(read_data);
//         return 1;
//     }

//     uint32_t error_count = 0;
//     for (uint32_t i = 0; i < BLOCK_SIZE; i++) {
//         if (read_data[i] != write_data[i]) {
//             error_count++;
//             // 打印前10处错误位置（避免刷屏）
//             if (error_count <= 10) {
//                 printf("10101010....0x%08X,0x%02X,0x%02X\r\n", 
//                        block_start_addr + i, write_data[i], read_data[i]);
//             }
//         }
//     }

//     if (error_count == 0) {
//         printf(" %d successful\r\n", block_num);
//     } else {
//         printf(" %d error%d\r\n", block_num, error_count);
//     }

//     // 释放内存
//     free(write_data);
//     free(read_data);
//     return (error_count == 0) ? 0 : 1;
// }

/**
 * 生成测试数据（递增序列，便于验证）
 * @param buffer：数据缓冲区
 * @param block_num：块编号（用于生成不同块的独特数据）
 * @param len：数据长度（BLOCK_SIZE）
 */
void generate_test_data(uint8_t *buffer, uint32_t block_num, uint32_t len) {
    for (uint32_t i = 0; i < len; i++) {
        // 数据 = 块编号 * 256 + 偏移量（确保不同块数据不同）
        buffer[i] = (block_num % 256) + (i % 256);
        buffer[i] %= 256;  // 确保在0~255范围内
    }
}

/**
 * 整片读写测试主函数
 * @return 0=成功，1=失败
 */
// uint8_t test_chip_full_operation(void) {
//     uint8_t *write_buffer = malloc(BLOCK_SIZE);  // 64KB块缓冲区（写）
//     uint8_t *read_buffer = malloc(BLOCK_SIZE);   // 64KB块缓冲区（读）
//     if (write_buffer == NULL || read_buffer == NULL) {

//         return 1;
//     }
//     if (spi_flash_erase_chip() != 0) {
//         free(write_buffer);
//         free(read_buffer);
//         return 1;
//     }
//     // 分块写入整片数据（64块×64KB=4MB）
//     for (uint32_t block = 0; block < TOTAL_BLOCKS; block++) {
//         uint32_t block_addr = block * BLOCK_SIZE;  // 当前块起始地址
//         // 生成当前块的测试数据
//         generate_test_data(write_buffer, block, BLOCK_SIZE);
//         // 写入当前块（使用现有块写入函数）
//         if (spi_flash_erase_and_write_block(block, write_buffer, BLOCK_SIZE) != 0) {
//             free(write_buffer);
//             free(read_buffer);
//             return 1;
//         }
//         // 每完成8块打印一次进度
//         if ((block + 1) % 8 == 0) {
//             printf("write%d/%dblock(%d%%)\r\n", block + 1, TOTAL_BLOCKS, (block + 1) * 100 / TOTAL_BLOCKS);
//         }
//     }
//     // 分块读取整片数据并验证
//     uint32_t total_error = 0;
//     for (uint32_t block = 0; block < TOTAL_BLOCKS; block++) {
//         uint32_t block_addr = block * BLOCK_SIZE;

//         // 读取当前块数据
//         if (spi_flash_gd25q32esig_read(block_addr, read_buffer, BLOCK_SIZE) != 0) {
//             free(write_buffer);
//             free(read_buffer);
//             return 1;
//         }

//         // 生成对应块的原始数据（用于对比）
//         generate_test_data(write_buffer, block, BLOCK_SIZE);

//         // 验证当前块数据
//         uint32_t block_error = 0;
//         for (uint32_t i = 0; i < BLOCK_SIZE; i++) {
//             if (read_buffer[i] != write_buffer[i]) {
//                 block_error++;
//                 // 只打印前3处错误（避免刷屏）
//                 if (block_error <= 3) {
//                     printf("block%derrot:addr0x%08X,yuqi0x%02X,sj0x%02X\r\n",
//                            block, block_addr + i, write_buffer[i], read_buffer[i]);
//                 }
//             }
//         }

//         total_error += block_error;
//         // 每完成8块打印一次进度
//         if ((block + 1) % 8 == 0) {
//             printf("%d/%dblock,error:%d\r\n", block + 1, TOTAL_BLOCKS, total_error);
//         }
//     }

//     //  输出测试结果
//     if (total_error == 0) {
//         printf("successful\r\n");
//     } else {
//         printf("error,data:%d\r\n", total_error);
//     }

//     // 释放内存
//     free(write_buffer);
//     free(read_buffer);
//     return (total_error == 0) ? 0 : 1;
// }
