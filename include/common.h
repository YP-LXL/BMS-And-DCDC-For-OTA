#ifndef COMMON_H
#define COMMON_H


/* Includes ------------------------------------------------------------------*/
#include "gd32f4xx.h"

#include <sys/unistd.h>
#include <stdint.h>
#include <stdio.h>   
#include <string.h>
#include <gd32f4xx_misc.h>
#include <gd32f4xx_usart.h>
#include "gd32f4x.h"
#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#include <errno.h>
#include <math.h>
#include "sh367309.h"
#include "gd25q32esig.h"
#include "atcommand.h"
// #include "dcdc_communicate.h"

// #include "board.h"

// #include "sh367309.h"
// #include "parameter.h"
// #include "at8563.h" 
// #include "gd25q32esig.h"
// // #include "battery_parallel_communicate.h"
// // #include "dcdc_communicate.h"
// #include "gateway_communicate.h"
// // #include "ota.h"
// // #include "pc_communicate.h"


// #include "atcommand.h"
// #include "system_gd32f4xx.h"


/* Define ------------------------------------------------------------------*/
/**
 * 关于共享内存数据多层次操作
 */

/* Unsigned 32-bit 数据 */
typedef union _UINT32
{
    uint32_t  lword;
    struct _WORDS
    {
        uint16_t LWORD:16;
        struct _SBYTES1
        {  
            uint8_t LSB;                               
            uint8_t MSB;
        }bytes_L;
        uint16_t MWORD:16;
        struct _SBYTES2
        {  
            uint8_t LSB;                               
            uint8_t MSB;
        }bytes_H;
    }WORDS;
}UINT_32;

/* Unsigned 16-bit 数据 */
typedef union _UINT16
{
    uint16_t W;
    struct _SBYTES
    {  
        uint8_t LSB;                               
        uint8_t MSB;
    }bytes;
}UINT_16;

/* 字节操作定义 */
typedef union
{
    uint8_t Byte;
    struct
    {
        uint8_t Bit0 :1;
        uint8_t Bit1 :1;
        uint8_t Bit2 :1;
        uint8_t Bit3 :1;
        uint8_t Bit4 :1;
        uint8_t Bit5 :1;
        uint8_t Bit6 :1;
        uint8_t Bit7 :1;
    }Bits;
}uByteBits;

typedef union _WORD
{
    unsigned short int W;
    struct _BIT
    {
        union _BITS_L
        {
            unsigned char L;
            struct _BITS_L_
            {
                uint8_t b0:1;
                uint8_t b1:1;
                uint8_t b2:1;
                uint8_t b3:1;
                uint8_t b4:1;
                uint8_t b5:1;
                uint8_t b6:1;
                uint8_t b7:1;
            }bit;
        }lbyte;    

        union _BITS_H
        {
            unsigned char H;
            struct _BITS_H_
            {
                uint8_t b8:1;
                uint8_t b9:1;
                uint8_t bA:1;
                uint8_t bB:1;
                uint8_t bC:1;
                uint8_t bD:1;
                uint8_t bE:1;
                uint8_t bF:1;
            }bit;
        }hbyte;
    }bits;
}uWordBits;


#define TX_DATA_BUFF_QUANTITY         							(500u)




/*定义不清，根据文档命名暂时先放到这*/
typedef struct
{
   uint8_t modbus_addr;
   uint8_t function_code;
   uint32_t register_add;
   uint32_t register_num;
   uint16_t byte_num;
   uint8_t register_data[TX_DATA_BUFF_QUANTITY];
   uint8_t modubus_crc[2];
}COMM_PARAM_TYPEDEF;




/* 逻辑右移取Z位 */
#define LOGICAL_RIGHT(x,y,z) (((x)>>(y))&z)	
/* 逻辑左移取W位 */				
#define LOGICAL_LEFT(u,v,w)                 (((u)<<(v))&w)		
			
#define LSR_GETX(x,y,z)			 		    (((x)>>(y))&z)					//逻辑右移
#define SHL_GETX(u,v,w)                     (((u)<<(v))&w)					//逻辑左移取x位

/* 字符串拼接数值 */
#define STRINGIFY(x)    #x
#define TOSTRING(x)     STRINGIFY(x)
#define CONCAT_STR_NUM(str, num)   str TOSTRING(num) //str "_" TOSTRING(num)

/* 16位数据共用体 */
typedef union
{
    uint16_t* p_u16_data;   // 指向无符号16位
    int16_t*  p_i16_data;   // 指向有符号16位
    uint8_t*  p_u8_data;    // 指向8位无符号数据 按字节访问,如： 0x1234 大端u8_data[0]= 0x12 u8_data[1]= 0x34,小端u8_data[0]= 0x34 u8_data[1]= 0x12,
}U_MODBUS_DATA_POINTER_TYPDEF;
/* DCDC-通用数据定义************************** */
/* -- DCDC-默认数据  */
#define DCDC_SOFT_VERSION 5  /* DCDC软件版本号 */
#define DCDC_DEV_ADDRESS  0  /* DCDC设备地址 */


/* -- DCDC-通信数据  */
/* 报警共用体定义 */
typedef union
{
    uint16_t value;  /* 报警的整体值 */
    struct
    {
        uint16_t bus_overvoltage : 1;         /* bit0:母线高压过压 */
        uint16_t bus_undervoltage : 1;        /* bit1:母线高压欠压 */
        uint16_t bus_overcurrent : 1;         /* bit2:母线电流过流 */
        uint16_t low_voltage_overvoltage : 1; /* bit3:低压电压过压 */
        uint16_t low_voltage_undervoltage : 1;/* bit4:低压电压欠压 */
        uint16_t low_current_overcurrent : 1; /* bit5:低压电电流过流 */
        uint16_t heating_film_fault : 1;      /* bit6:加热膜异常 */
        uint16_t fan_fault : 1;               /* bit7:风扇异常 */
        uint16_t reserved : 8;                /* 其他保留 */
    } bits;
} U_ALARM_TYPDEF;

/* 保护共用体定义 */
typedef union
{
    uint16_t value;  /* 保护的整体值 */
    struct
    {
        uint16_t bus_overvoltage : 1;         /* bit0:母线高压过压 */
        uint16_t bus_undervoltage : 1;        /* bit1:母线高压欠压 */
        uint16_t bus_overcurrent : 1;         /* bit2:母线电流过流 */
        uint16_t low_voltage_overvoltage : 1; /* bit3:低压电压过压 */
        uint16_t low_voltage_undervoltage : 1;/* bit4:低压电压欠压 */
        uint16_t low_current_overcurrent : 1; /* bit5:低压电电流过流 */
        uint16_t heating_film_fault : 1;      /* bit6:加热膜异常 */
        uint16_t fan_fault : 1;               /* bit7:风扇异常 */
        uint16_t reserved : 8;                /* 其他保留 */
    } bits;
} U_PROTECTION_TYPDEF;

/* DCDC-结构体变量 宏定义 */
#pragma pack(1)
typedef struct
{
    /* modbus 3区（30001~39999） 只读 */
    /* ========================= DCDC设备信息 ========================= */
    uint16_t dcdc_device_address;        /* 设备地址 (只读) */
    uint16_t dcdc_sw_version;            /* 软件版本号 (示例：0x0102表示V1.2) (只读) */

    /* ========================= DCDC状态监测 ========================= */
    uint16_t            status;                  /* 状态 */
    U_ALARM_TYPDEF      u_alarm;                 /* 报警（共用体 位操作） */
    U_PROTECTION_TYPDEF u_protection;            /* 保护（共用体 位操作） */

    /* ========================= 电气参数监测 ========================= */
    uint16_t dcdc_bus_voltage;           /* 母线电压 (单位：0.1V) 实际值=raw*0.1V */
    int16_t dcdc_bus_current;            /* 母线电流 (单位：mA) 支持正负值表示方向 */
    uint16_t dcdc_lv_voltage;            /* 低压侧电压 (电池端，单位：0.01V) 实际值=raw*0.01V */
    int16_t dcdc_lv_current;             /* 低压侧电流 (电池端，单位：mA) 支持正负值 */
    int16_t dcdc_temperature;            /* 温度 (单位：0.01℃) 实际值=raw*0.01℃ */
    uint16_t dcdc_power;                 /* 当前功率值 (单位：W) */

    /* modbus 4区（40001~49999） 寄存器 读写 */
    uint16_t            sleep_status;            /* 睡眠 */
    uint16_t            power_status;            /* 开/关机 */
    uint16_t            charge_discharge_control;/* 充、放电控制 */
    /* ======================== 2025.06.10->dcdc用 ======================== */
    uint16_t current_calibration_preparation;   /* 电流标定-准备 */
    uint16_t current_calibration_value;         /* 电流标定-标定(A) */
    uint16_t voltage_calibration_preparation;   /* 电压标定-准备 */
    uint16_t voltage_calibration_value;         /* 电压标定-标定(V) */
    uint16_t temp_calibration_preparation;      /* 温度标定-准备 */
    uint16_t temp_calibration_value;            /* 温度标定-标定(0.01℃) */
    uint16_t freq_calibration_preparation;      /* 频率标定-准备 */
    uint16_t freq_calibration_value;            /* 频率标定-标定(hz) */
    uint16_t usart_receive_num;                 /* 串口指令接收计数 */
    uint16_t usart_send_num;                    /* 串口指令应答计数 */
    uint16_t erase_usart_num_clt;               /* 清串口计数值 */          

} T_DCDC_DATA_TYPDEF;
#pragma pack()
extern T_DCDC_DATA_TYPDEF t_dcdc_data;
#endif
 