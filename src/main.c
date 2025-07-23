#include <stdio.h>
#include <FreeRTOS.h>
#include <task.h>

#include "common.h"
#include "project_config.h"
#include "gd32f4xx.h"
#include "logger.h"
#include "atcommand.h"
#include "test_task.h"
// #include "collect_task.h"
// #include "communicate_task.h"
// #include "protect_task.h"
/* 测试 */
#define         test_task_stack_words                                   256 /* 占用空间字为单位 */
#define         test_task_priority                                      7   /* 优先级 */
StaticTask_t    test_task_buffer;
StackType_t     test_task_stack[test_task_stack_words];
TaskHandle_t    test_task_handle;

#define         counttest_task_stack_words                              256 /* 占用空间字为单位 */
#define         counttest_task_priority                                  2   /* 优先级 */
StaticTask_t    counttest_task_buffer;
StackType_t     counttest_task_stack[counttest_task_stack_words];
TaskHandle_t    counttest_task_handle;

#define TEST_LOW_POWER      0   /* pcba  使能 */
/* 采集 */
#define         collect_afe_task_stack_words                    512 /* 占用空间字为单位 */
#define         collect_afe_task_priority                       3   /* 优先级 */
StaticTask_t    collect_afe_task_buffer;
StackType_t     collect_afe_task_stack[collect_afe_task_stack_words];
TaskHandle_t    collect_afe_task_handle;

/* 采集 */
#define         collect_mcu_task_stack_words                    512 /* 占用空间字为单位 */
#define         collect_mcu_task_priority                       4   /* 优先级 */
StaticTask_t    collect_mcu_task_buffer;
StackType_t     collect_mcu_task_stack[collect_mcu_task_stack_words];
TaskHandle_t    collect_mcu_task_handle;

/* 采集 */
#define         collect_soc_soh_task_stack_words                    512 /* 占用空间字为单位 */
#define         collect_soc_soh_task_priority                       5   /* 优先级 */
StaticTask_t    collect_soc_soh_task_buffer;
StackType_t     collect_soc_soh_task_stack[collect_soc_soh_task_stack_words];
TaskHandle_t    collect_soc_soh_task_handle;

/* DCDC通信 */
#define         dcdc_communicate_task_stack_words                       512 /* 占用空间字为单位 */
#define         dcdc_communicate_task_priority                          2   /* 优先级 */
StaticTask_t    dcdc_communicate_task_buffer;
StackType_t     dcdc_communicate_task_stack[dcdc_communicate_task_stack_words];
TaskHandle_t    dcdc_communicate_task_handle;

/* CAN通信 */
#define         gateway_communicate_task_size                        512
#define         gateway_communicate_task_priority                    2
TaskHandle_t    gateway_communicate_task_handle;
StackType_t     gateway_communicate_task_stack[gateway_communicate_task_size];
StaticTask_t    gateway_communicate_task_buffer;

#define             protect_nonactivated_task_size                   256
#define             protect_nonactivated_task_priority               4
TaskHandle_t        protect_nonactivated_task_handle;
StackType_t         protect_nonactivated_task_stack[protect_nonactivated_task_size];
StaticTask_t        protect_nonactivated_task_buffer;

#define             protect_voltage_task_size                   256
#define             protect_voltage_task_priority               8
TaskHandle_t        protect_voltage_task_handle;
StackType_t         protect_voltage_task_stack[protect_voltage_task_size];
StaticTask_t        protect_voltage_task_buffer;

#define             protect_current_task_size                   256
#define             protect_current_task_priority               8
TaskHandle_t        protect_current_task_handle;
StackType_t         protect_current_task_stack[protect_current_task_size];
StaticTask_t        protect_current_task_buffer;

#define             protect_temperature_task_size                   256
#define             protect_temperature_task_priority               8
TaskHandle_t        protect_temperature_task_handle;
StackType_t         protect_temperature_task_stack[protect_temperature_task_size];
StaticTask_t        protect_temperature_task_buffer;

#define             protect_dealt_task_size                   512
#define             protect_dealt_task_priority               9
TaskHandle_t        protect_dealt_task_handle;
StackType_t         protect_dealt_task_stack[protect_dealt_task_size];
StaticTask_t        protect_dealt_task_buffer;

#define             protect_other_task_size                   256
#define             protect_other_task_priority               8
TaskHandle_t        protect_other_task_handle;
StackType_t         protect_other_task_stack[protect_other_task_size];
StaticTask_t        protect_other_task_buffer;

#define             period_key_task_size                   256
#define             period_key_task_priority               8
TaskHandle_t period_key_task_handle;
StackType_t period_key_task_stack[period_key_task_size];
StaticTask_t period_key_task_buffer;

#define             period_box_task_size                   256
#define             period_box_task_priority               8
TaskHandle_t period_box_task_handle;
StackType_t period_box_task_stack[period_box_task_size];
StaticTask_t period_box_task_buffer;

#define             period_alarm_task_size                   256
#define             period_alarm_task_priority               7
TaskHandle_t period_alarm_task_handle;
StackType_t  period_alarm_task_stack[period_alarm_task_size];
StaticTask_t period_alarm_task_buffer;

#define             ota_task_size                   4096
#define             ota_task_priority               1
TaskHandle_t ota_task_handle;
StackType_t  ota_task_stack[ota_task_size];
StaticTask_t ota_task_buffer;

/**
 * @函数名称: all_init()
 * @功能描述: 所有硬件接口层初始化
 * @输入: 无
 * @输出: 无
 * @返回值: 无
 */
static void all_init(void)
{
    // uint8_t val = 0;
    SCB->VTOR = 0x08005000;
    //SCB->VTOR = FLASH_BASE | 0x5000;
    gd32f4x_system_clock_init();
    parameter_ram_init();
    /* GPIO初始化 */
	gd32f4x_gpio_init();
    /* 串口初始化 */
    // gd32f4x_usart_init();
    gd_eval_com_init();

    printf("***bms_start Tver=%s ***\r\n",CONCAT_STR_NUM(SOFTWARE_NAME_V,SOFTWARE_V_NUM));
    //usart_data_transmit(USART2,(uint8_t)1);
    /* DMA初始化 */
    gd32f4x_dma_init();   

    rcu_periph_clock_enable(RCU_CRC);

    uint8_t ret = gd32f4x_usart2_set_receive_callback(usart2_data_process);
    /*定时器初始化*/
    gd32f4x_timer_init();  
    /*ADC初始化*/
    gd32f4x_adc_init();    
    /*CAN0初始化*/
    gd32f4x_can0_init(BAUD_500K);           

    /*CAN1初始化*/
    gd32f4x_can1_init(BAUD_500K);  

    /* afe初始化--暂时放这里 */
    gd32f4x_i2c1_init(500000);


    gd32f4x_rtc_init(); 

    gd32f4x_spi_init(); 


    ATcmd_init();
    

    
    // /* 日志打印管理 */
    // log_set_enable(1);
    // log_set_level(LOG_LEVEL_DEBUG);

    // /* 所有参数的注册和初始化包括AFE和MCU的初始化,在这里添加偶现阻塞问题，目前添加在任务中 */
    // parameter_register_and_init();



    // val = dcdc_communicate_init(dcdc_communication_param);

    log_set_enable(LOG_ENABLE);
    log_set_level(LOG_LEVEL_DEBUG);

    param_ram.uninitialized_flag &= ~TASK_UNINITIALIZED_FLAG_HARDWARE; /* 清除未初始化标志位 */
}




int main(void)
{
    all_init();
    printf("123\r\n");


#if TEST_LOW_POWER
    gd32f4x_delay_ms(10000);
    gd32f4x_mcu_sleep_enter(MCU_MODE_STANDBY);
#endif

    /* 创建不使用任何动态内存分配的任务:(任务-执行任务的函数 、任务的文本名称、数据索引的数量buff大小、任务中传入的参数、 任务创建的优先级、作为任务栈的数组、保存任务数据结构的变量)*/
    /* 测试任务*/
    test_task_handle = xTaskCreateStatic(test_task, 
                                        "test_task", 
                                         test_task_stack_words, 
                                         (void *)0, 
                                         test_task_priority, 
                                         test_task_stack, 
                                        &test_task_buffer);
    
    ota_task_handle = xTaskCreateStatic(
                                        /* 执行任务的函数 */
                                        ota_task,
                                        /* 任务的文本名称 */
                                        "ota_task",
                                        /* 数据索引的数量 */
                                        ota_task_size,
                                        /* 任务中传入的参数 */
                                        (void*)0,
                                        /* 任务创建的优先级 */
                                        ota_task_priority,
                                        /* 作为任务栈的数组 */
                                        ota_task_stack,
                                        /* 保存任务数据结构的变量 */
                                        &ota_task_buffer);
                                        
    // counttest_task_handle = xTaskCreateStatic(counttest_task, 
    //                                     "counttest_task", 
    //                                      counttest_task_stack_words, 
    //                                      (void *)0, 
    //                                      counttest_task_priority,   
    //                                      counttest_task_stack, 
    //                                     &counttest_task_buffer);
    // /* 采集任务*/
    // collect_afe_task_handle = xTaskCreateStatic(collect_afe_task, 
    //                                     "collect_afe_task", 
    //                                      collect_afe_task_stack_words, 
    //                                     (void *)0,
    //                                      collect_afe_task_priority, 
    //                                      collect_afe_task_stack, 
    //                                     &collect_afe_task_buffer);

    // /* 采集任务*/
    // collect_mcu_task_handle = xTaskCreateStatic(collect_mcu_task, 
    //                                     "collect_mcu_task", 
    //                                      collect_mcu_task_stack_words, 
    //                                     (void *)0,
    //                                      collect_mcu_task_priority, 
    //                                      collect_mcu_task_stack, 
    //                                     &collect_mcu_task_buffer);

    // /* 采集任务*/
    // collect_soc_soh_task_handle = xTaskCreateStatic(collect_soc_soh_task, 
    //                                     "collect_soc_soh_task", 
    //                                      collect_soc_soh_task_stack_words, 
    //                                     (void *)0, 
    //                                      collect_soc_soh_task_priority, 
    //                                      collect_soc_soh_task_stack, 
    //                                     &collect_soc_soh_task_buffer);

    // /* DCDC通信任务*/
    // dcdc_communicate_task_handle = xTaskCreateStatic(dcdc_communicate_task, 
    //                                     "dcdc_communicate_task", 
    //                                      dcdc_communicate_task_stack_words, 
    //                                     (void *)0, 
    //                                      dcdc_communicate_task_priority, 
    //                                      dcdc_communicate_task_stack, 
    //                                     &dcdc_communicate_task_buffer);

    // /* 通信 */
    // gateway_communicate_task_handle = xTaskCreateStatic(
    //                                     /* 执行任务的函数 */
    //                                     gateway_communicate_task,
    //                                     /* 任务的文本名称 */
    //                                     "gateway_communicate_task",
    //                                     /* 数据索引的数量 */
    //                                     gateway_communicate_task_size,
    //                                     /* 任务中传入的参数 */
    //                                     (void*)0,
    //                                     /* 任务创建的优先级 */
    //                                     gateway_communicate_task_priority,
    //                                     /* 作为任务栈的数组 */
    //                                     gateway_communicate_task_stack,
    //                                     /* 保存任务数据结构的变量 */
    //                                     &gateway_communicate_task_buffer);

    // // protect_nonactivated_task_handle = xTaskCreateStatic(
    // //                                     /* 执行任务的函数 */
    // //                                     protect_nonactivated_task,
    // //                                     /* 任务的文本名称 */
    // //                                     "protect_nonactivated_task",
    // //                                     /* 数据索引的数量 */
    // //                                     protect_nonactivated_task_size,
    // //                                     /* 任务中传入的参数 */
    // //                                     (void *)0,
    // //                                     /* 任务创建的优先级 */
    // //                                     protect_nonactivated_task_priority,
    // //                                     /* 作为任务栈的数组 */
    // //                                     protect_nonactivated_task_stack,
    // //                                     /* 保存任务数据结构的变量 */
    // //                                     &protect_nonactivated_task_buffer);

    // protect_voltage_task_handle = xTaskCreateStatic(
    //                                     /* 执行任务的函数 */
    //                                     protect_voltage_task,
    //                                     /* 任务的文本名称 */
    //                                     "protect_voltage_task",
    //                                     /* 数据索引的数量 */
    //                                     protect_voltage_task_size,
    //                                     /* 任务中传入的参数 */
    //                                     (void *)0,
    //                                     /* 任务创建的优先级 */
    //                                     protect_voltage_task_priority,
    //                                     /* 作为任务栈的数组 */
    //                                     protect_voltage_task_stack,
    //                                     /* 保存任务数据结构的变量 */
    //                                     &protect_voltage_task_buffer);

    // protect_current_task_handle = xTaskCreateStatic(
    //                                     /* 执行任务的函数 */
    //                                     protect_current_task,
    //                                     /* 任务的文本名称 */
    //                                     "protect_current_task",
    //                                     /* 数据索引的数量 */
    //                                     protect_current_task_size,
    //                                     /* 任务中传入的参数 */
    //                                     (void *)0,
    //                                     /* 任务创建的优先级 */
    //                                     protect_current_task_priority,
    //                                     /* 作为任务栈的数组 */
    //                                     protect_current_task_stack,
    //                                     /* 保存任务数据结构的变量 */
    //                                     &protect_current_task_buffer);

    // protect_temperature_task_handle = xTaskCreateStatic(
    //                                     /* 执行任务的函数 */
    //                                     protect_temperature_task,
    //                                     /* 任务的文本名称 */
    //                                     "protect_temperature_task",
    //                                     /* 数据索引的数量 */
    //                                     protect_temperature_task_size,
    //                                     /* 任务中传入的参数 */
    //                                     (void *)0,
    //                                     /* 任务创建的优先级 */
    //                                     protect_temperature_task_priority,
    //                                     /* 作为任务栈的数组 */
    //                                     protect_temperature_task_stack,
    //                                     /* 保存任务数据结构的变量 */
    //                                     &protect_temperature_task_buffer);

    // protect_dealt_task_handle = xTaskCreateStatic(
    //                                     /* 执行任务的函数 */
    //                                     protect_dealt_task,
    //                                     /* 任务的文本名称 */
    //                                     "protect_dealt_task",
    //                                     /* 数据索引的数量 */
    //                                     protect_dealt_task_size,
    //                                     /* 任务中传入的参数 */
    //                                     (void *)0,
    //                                     /* 任务创建的优先级 */
    //                                     protect_dealt_task_priority,
    //                                     /* 作为任务栈的数组 */
    //                                     protect_dealt_task_stack,
    //                                     /* 保存任务数据结构的变量 */
    //                                     &protect_dealt_task_buffer);




    // // protect_other_task_handle = xTaskCreateStatic(
    // //                                     /* 执行任务的函数 */
    // //                                     protect_other_task,
    // //                                     /* 任务的文本名称 */
    // //                                     "protect_other_task",
    // //                                     /* 数据索引的数量 */
    // //                                     protect_other_task_size,
    // //                                     /* 任务中传入的参数 */
    // //                                     (void *)0,
    // //                                     /* 任务创建的优先级 */
    // //                                     protect_other_task_priority,
    // //                                     /* 作为任务栈的数组 */
    // //                                     protect_other_task_stack,
    // //                                     /* 保存任务数据结构的变量 */
    // //                                     &protect_other_task_buffer);

    // /* 创建不使用任何动态内存分配的任务 */
    // period_box_task_handle = xTaskCreateStatic(
    //                                     /* 执行任务的函数 */
    //                                     period_box_task,
    //                                     /* 任务的文本名称 */
    //                                     "period_box_task",
    //                                     /* 数据索引的数量 */
    //                                     period_box_task_size,
    //                                     /* 任务中传入的参数 */
    //                                     (void*)0,
    //                                     /* 任务创建的优先级 */
    //                                     period_box_task_priority,
    //                                     /* 作为任务栈的数组 */
    //                                     period_box_task_stack,
    //                                     /* 保存任务数据结构的变量 */
    //                                     &period_box_task_buffer);

    // /* 创建不使用任何动态内存分配的任务 */
    // period_key_task_handle = xTaskCreateStatic(
    //                                     /* 执行任务的函数 */
    //                                     period_key_task,
    //                                     /* 任务的文本名称 */
    //                                     "period_key_task",
    //                                     /* 数据索引的数量 */
    //                                     period_key_task_size,
    //                                     /* 任务中传入的参数 */
    //                                     (void*)0,
    //                                     /* 任务创建的优先级 */
    //                                     period_key_task_priority,
    //                                     /* 作为任务栈的数组 */
    //                                     period_key_task_stack,
    //                                     /* 保存任务数据结构的变量 */
    //                                     &period_key_task_buffer);

    // /* 创建不使用任何动态内存分配的任务 */
    // period_alarm_task_handle = xTaskCreateStatic(
    //                                     /* 执行任务的函数 */
    //                                     period_alarm_task,
    //                                     /* 任务的文本名称 */
    //                                     "period_alarm_task",
    //                                     /* 数据索引的数量 */
    //                                     period_alarm_task_size,
    //                                     /* 任务中传入的参数 */
    //                                     (void*)0,
    //                                     /* 任务创建的优先级 */
    //                                     period_alarm_task_priority,
    //                                     /* 作为任务栈的数组 */
    //                                     period_alarm_task_stack,
    //                                     /* 保存任务数据结构的变量 */
    //                                     &period_alarm_task_buffer);


    vTaskStartScheduler();

    return 0;
}




