#include "board.h"

/**
 * @函数名称: red_led_switch_on_or_off()
 * @功能描述: red_led灯开关或关闭
 * @输入: action：打开或关闭，取值："ON"或"OFF"
 * @输出: 无
 * @返回值: 0:正确。1：错误
 */
static uint8_t red_led_switch_on_or_off(char* action)
{
    uint8_t return_value = 0;
    char *action_name[2] = {"ON","OFF"};
    if(action != NULL) {
        if(strcmp(action,action_name[0]) == 0) {
            gd32f4x_timer2_led_j49_6_set_duty(101);
        } else if(strcmp(action,action_name[1]) == 0) {
            gd32f4x_timer2_led_j49_6_set_duty(0);
        } else {
            return_value = 1;
        }
    } else {
        return_value = 1;
    }
    return return_value;    
}

/**
 * @函数名称: white_red_led_switch_on_or_off()
 * @功能描述: white_led灯开关或关闭
 * @输入: action：打开或关闭，取值："ON"或"OFF"
 * @输出: 无
 * @返回值: 0:正确。1：错误
 */
static uint8_t white_red_led_switch_on_or_off(char* action)
{
    uint8_t return_value = 0;
    char *action_name[2] = {"ON","OFF"};
    if(action != NULL) {
        if(strcmp(action,action_name[0]) == 0) {
            gd32f4x_timer0_led_j49_7_set_duty(101); 
        } else if(strcmp(action,action_name[1]) == 0) {
            gd32f4x_timer0_led_j49_7_set_duty(0);
        } else {
            return_value = 1;
        }
    } else {
        return_value = 1;
    }
    return return_value;    
}

/**
 * @函数名称: pcba_switch_vbat_enable_on_or_off()
 * @功能描述: EN_SW1置高，Vbat开始给板卡通电，辅电工作
 * @输入: action：打开或关闭，取值："ON"或"OFF"
 * @输出: 无
 * @返回值: 0:正确。1：错误
 */
static uint8_t pcba_switch_vbat_enable_on_or_off(char* action)
{
    uint8_t return_value = 0;
    char *action_name[2] = {"ON","OFF"};
    if(action != NULL) {
        if(strcmp(action,action_name[0]) == 0) {
            EN_SW1_APS_VBT_ON();
        } else if(strcmp(action,action_name[1]) == 0) {
            EN_SW1_APS_VBT_OFF();
        } else {
            return_value = 1;
        }
    } else {
        return_value = 1;
    }
    return return_value;    
}

/**
 * @函数名称: pcba_switch_vbat_enable_on_or_off()
 * @功能描述: EN_SW2置高，Vbat开始防反接电路工作
 * @输入: action：打开或关闭，取值："ON"或"OFF"
 * @输出: 无
 * @返回值: 0:正确。1：错误
 */
static uint8_t pcba_reverse_protection_vbat_enable_on_or_off(char* action)
{
    uint8_t return_value = 0;
    char *action_name[2] = {"ON","OFF"};
    if(action != NULL) {
        if(strcmp(action,action_name[0]) == 0) {
            EN_SW2_ARC_VBT_ON();
        } else if(strcmp(action,action_name[1]) == 0) {
            EN_SW2_ARC_VBT_OFF();
        } else {
            return_value = 1;
        }
    } else {
        return_value = 1;
    }
    return return_value;    
}

/**
 * @函数名称: communication_vbat_enable_on_or_off()
 * @功能描述: 485/CAN用的隔离芯片供电使能控制开关
 * @输入: action：打开或关闭，取值："ON"或"OFF"
 * @输出: 无
 * @返回值: 0:正确。1：错误
 */
static uint8_t communication_vbat_enable_on_or_off(char* action)
{
    uint8_t return_value = 0;
    char *action_name[2] = {"ON","OFF"};
    if(action != NULL) {
        if(strcmp(action,action_name[0]) == 0) {
            PERIPHERAL_POWER_VDD_ON();
        } else if(strcmp(action,action_name[1]) == 0) {
            PERIPHERAL_POWER_VDD_OFF();
        } else {
            return_value = 1;
        }
    } else {
        return_value = 1;
    }
    return return_value;    
}

/**
 * @函数名称: low_vbat_drive_enable_on_or_off()
 * @功能描述: 低压Vbat驱动端工作控制开关
 * @输入: action：打开或关闭，取值："ON"或"OFF"
 * @输出: 无
 * @返回值: 0:正确。1：错误
 */
static uint8_t low_vbat_drive_enable_on_or_off(char* action)
{
    uint8_t return_value = 0;
    char *action_name[2] = {"ON","OFF"};
    if(action != NULL) {
        if(strcmp(action,action_name[0]) == 0) {
            LOW_VOLTAGE_DRIVE_EN_ON();
        } else if(strcmp(action,action_name[1]) == 0) {
            LOW_VOLTAGE_DRIVE_EN_OFF();
        } else {
            return_value = 1;
        }
    } else {
        return_value = 1;
    }
    return return_value;    
}

/**
 * @函数名称: communication_receive_enable_on_or_off()
 * @功能描述: RS485(/RE)收发器的接收使能控制开关
 * @输入: action：打开或关闭，取值："ON"或"OFF"
 * @输出: 无
 * @返回值: 0:正确。1：错误
 */
static uint8_t communication_receive_enable_on_or_off(char* action)
{
    uint8_t return_value = 0;
    char *action_name[2] = {"ON","OFF"};
    if(action != NULL) {
        if(strcmp(action,action_name[0]) == 0) {
            PERIPHERAL_RS485_EN_ON();
        } else if(strcmp(action,action_name[1]) == 0) {
            PERIPHERAL_RS485_EN_OFF();
        } else {
            return_value = 1;
        }
    } else {
        return_value = 1;
    }
    return return_value;    
}

/**
 * @函数名称: afe_switch_power_vpro_on_or_off()
 * @功能描述: VPRO的供电使能，常低，拉高延时1s供电。
 * @输入: action：打开或关闭，取值："ON"或"OFF"
 * @输出: 无
 * @返回值: 0:正确。1：错误 
 */
static uint8_t afe_switch_power_vpro_on_or_off(char* action)
{
    uint8_t return_value = 0;
    char *action_name[2] = {"ON","OFF"};
    if(action != NULL) {
        if(strcmp(action,action_name[0]) == 0) {
            VPRO12V8V_CONTROL_ON();
        } else if(strcmp(action,action_name[1]) == 0) {
            VPRO12V8V_CONTROL_OFF();
        } else {
            return_value = 1;
        }
    } else {
        return_value = 1;
    }
    return return_value;        
}

/**
 * @函数名称: dry_contact_relay_switch_on_or_off()
 * @功能描述: 控制干接点继电器供电电源控制开关
 * @输入: action：打开或关闭，取值："ON"或"OFF"
 * @输出: 无
 * @返回值: 0:正确。1：错误 
 */
static uint8_t dry_contact_relay_switch_on_or_off(char* action)
{
    uint8_t return_value = 0;
    char *action_name[2] = {"ON","OFF"};
    if(action != NULL) {
        if(strcmp(action,action_name[0]) == 0) {
            // DRY_CONTACT_RELAY_EN_ON();
        } else if(strcmp(action,action_name[1]) == 0) {
            // DRY_CONTACT_RELAY_EN_OFF();
        } else {
            return_value = 1;
        }
    } else {
        return_value = 1;
    }
    return return_value;        
}

/**
 * @函数名称: afe_switch_mos_on_or_off()
 * @功能描述: MOS开关关闭或打开
 * @输入: action：打开或关闭，取值："ON"或"OFF"
 * @输出: 无
 * @返回值: 0:正确。1：错误
 */
static uint8_t afe_switch_mos_on_or_off(char* action)
{
    uint8_t return_value = 0;
    char *action_name[2] = {"ON","OFF"};
    if(action != NULL) {
        if(strcmp(action,action_name[0]) == 0) {
            AFESH367309MOS_ON();
        } else if(strcmp(action,action_name[1]) == 0) {
            AFESH367309MOS_OFF();
        } else {
            return_value = 1;
        }
    } else {
        return_value = 1;
    }
    return return_value;    
}

/**
 * @函数名称: afe_switch_ship_on_or_off()
 * @功能描述: 输出，SHIP，常低，拉高SHIP拉高，退出仓运。
 * @输入: action：打开或关闭，取值："ON"或"OFF"
 * @输出: 无
 * @返回值: 0:正确。1：错误
 */
static uint8_t afe_switch_ship_on_or_off(char* action)
{
    uint8_t return_value = 0;
    char *action_name[2] = {"ON","OFF"};
    if(action != NULL) {
        if(strcmp(action,action_name[0]) == 0) {
            WAREHOUSE_STAT_CONTR_IN();
        } else if(strcmp(action,action_name[1]) == 0) {
            WAREHOUSE_STAT_CONTR_OUT();
        } else {
            return_value = 1;
        }
    } else {
        return_value = 1;
    }
    return return_value;    
}

/**
 * @函数名称: self_inspection_out()
 * @功能描述: 自检状态输出
 * @输入: action：打开或关闭，取值："ON"或"OFF"
 * @输出: 无
 * @返回值: 0:正确。1：错误
 */
static uint8_t self_inspection_out(char* action)
{
    uint8_t return_value = 0;
    char *action_name[2] = {"ON","OFF"};
    if(action != NULL) {
        if(strcmp(action,action_name[0]) == 0) {
            EN_PWR_ON();
        } else if(strcmp(action,action_name[1]) == 0) {
            EN_PWR_OFF();
        } else {
            return_value = 1;
        }
    } else {
        return_value = 1;
    }
    return return_value;    
}

/**
 * @函数名称: high_voltage_drive_on_or_off()
 * @功能描述: 高压Vbat驱动端工作控制开关
 * @输入: action：打开或关闭，取值："ON"或"OFF"
 * @输出: 无
 * @返回值: 0:正确。1：错误
 */
static uint8_t high_voltage_drive_on_or_off(char* action)
{
    uint8_t return_value = 0;
    char *action_name[2] = {"ON","OFF"};
    if(action != NULL) {
        if(strcmp(action,action_name[0]) == 0) {
            HIGH_VOLTAGE_DRIVE_EN_ON();
        } else if(strcmp(action,action_name[1]) == 0) {
            // HIGH_VOLTAGE_DRIVE_EN_OFF();
        } else {
            return_value = 1;
        }
    } else {
        return_value = 1;
    }
    return return_value;    
}

/**
 * @函数名称: board_switch_on_or_off()
 * @功能描述: 开关功能打开或关闭
 * @输入: switch_name:开关量名称 action：打开或关闭，取值："ON"或"OFF"
 * @输出: 无
 * @返回值: 0:正确。1：错误
 */
uint8_t board_switch_on_or_off(SWITCH_TYPE switch_name,char* action)
{
    uint8_t return_value = 0;
   /* 检查参数合法性 */
    if ((switch_name >= SWITCH_TYPE_MAX) || (action == NULL)) {
        /* 返回错误值 */
        return_value = 1; 
        return return_value;
    }
    switch(switch_name) {
        case RED_LED_RUN: {
            return_value = red_led_switch_on_or_off(action);
        }break;
        case WHITE_LED_RUN: {
            return_value = white_red_led_switch_on_or_off(action);
        }break;
        case EN_SW1: {
            return_value = pcba_switch_vbat_enable_on_or_off(action);
        }break;
        case EN_SW2: {
            return_value = pcba_reverse_protection_vbat_enable_on_or_off(action);
        }break;
        case CAN_RS485_POWER: {
            return_value = communication_vbat_enable_on_or_off(action);
        }break;
        case LOW_VBAT_DRIVE: {
            return_value = low_vbat_drive_enable_on_or_off(action);
        }break;
        case RS485_RE: {
            return_value = communication_receive_enable_on_or_off(action);
        }break;
        case POWER_VPRO: {
            return_value = afe_switch_power_vpro_on_or_off(action);
        }break;
        case RELAY_POWER: {
            return_value = dry_contact_relay_switch_on_or_off(action);
        }break;
        case AFE_MOS: {
            return_value = afe_switch_mos_on_or_off(action);
        }break;
        case SHIP: {
            return_value = afe_switch_ship_on_or_off(action);
        }break;
        case SELF_OUT: {
            return_value = self_inspection_out(action);
        }break;
        case HIGH_VBAT_DRIVE: {
            return_value = high_voltage_drive_on_or_off(action);
        }break;
        default:break;
    }   
    return return_value;
}




/*输入检测*********************************************************************** */
/**
 * @函数名称: board_reverse_connect_detection()
 * @功能描述: 获取是否存在母线高压输出
 * @输入: 无
 * @输出: 无
 * @返回值: 0:未存在。1：存在
 */
uint8_t board_bus_high_voltage_detection(void) 
{
    uint8_t pin_state = BUS_HIGH_VOLTAGE_STATE();

    if (pin_state == SET) {
        return 1; 
    } else {
        return 0; 
    }
}


/* 功能键************************************************************************** */
#define EVENT_CB(ev)       if (handle->cb[ev]) handle->cb[ev]((void*)handle)
#define PRESS_REPEAT_MAX_NUM 15 /* 重复计数器的最大值 */

static BUTTON_TYPDEF* head_handle = NULL;

static void board_button_handler(BUTTON_TYPDEF* handle)
{
    /* 获取当前电平 */
    static uint8_t level_laste = 0;
    uint8_t        level_cur   = handle->hal_button_Level(handle->button_id);

    /* 消抖 */
    if (level_laste != level_cur) {
        if (handle->debounce_cnt < DEBOUNCE_TICKS) {
            handle->debounce_cnt++;
            return;
        }
        else {
            handle->debounce_cnt = 0;
            level_laste          = level_cur;
        }
    }

    /* 按键有效 */
    if (level_cur == handle->active_level) {
        /* 按下判断 */
        if (handle->state == 0) {
            handle->state        = 1;
            handle->debounce_cnt = 0;
            handle->ticks        = 0;
            EVENT_CB(PRESS_DOWN);
        }
        /* 长按判断 */
        if (handle->ticks == LONG_TICKS) {
            if (handle->event != LONG_PRESS_HOLD) {
                handle->event = LONG_PRESS_START;
                EVENT_CB(LONG_PRESS_START);
                // handle->ticks = 0;
            }
        }
        else if (handle->ticks > LONG_TICKS) {
            EVENT_CB(LONG_PRESS_HOLD);
        }
    }
    else {
        /* 按键无效 */
        if (handle->state == 1) {
            if (handle->ticks < SHORT_TICKS) {
                if (handle->repeat < PRESS_REPEAT_MAX_NUM) {
                    handle->repeat++;
                }
            }

            switch (handle->repeat) {
                case 1:
                    handle->event = SINGLE_CLICK;
                    EVENT_CB(SINGLE_CLICK);
                    break;
                case 2:
                    handle->event = DOUBLE_CLICK;
                    EVENT_CB(DOUBLE_CLICK);
                    break;

                default: break;
            }

            handle->state = 0;
            EVENT_CB(PRESS_UP);
        }
        else if ((handle->repeat > 0) && (handle->ticks > LONG_TICKS)) {
            handle->repeat = 0;
        }
    }

    if (handle->ticks < 0xFFFF) {
        handle->ticks++;
    }
}

/**
 * @函数名称: button_init()
 * @功能描述: 初始化按键结构体
 * @输入: handle 按键结构体
 * @输入: button_init 按键读取
 * @输入: active_level 按键触发电平
 * @输出: button_id 按键ID
 * @返回值: 无
 */
void board_button_init(BUTTON_TYPDEF* handle, uint8_t (*pin_level)(uint8_t), uint8_t active_level, uint8_t button_id)
{
    memset(handle, 0, sizeof(BUTTON_TYPDEF));
    handle->event            = (uint8_t)NONE_PRESS;
    handle->hal_button_Level = pin_level;
    handle->button_level     = !active_level;
    handle->active_level     = active_level;
    handle->button_id        = button_id;
}

/**
 * @函数名称: button_attach()
 * @功能描述: 关联回调函数
 * @输入: handle 目标函数结构体
 * @输入: event 关联事件
 * @输入: cb 回调处理
 * @输出: 无
 * @返回值: 无
 */
void board_button_attach(BUTTON_TYPDEF* handle, PRESS_EVENT_TYPDEF event, button_call_back cb)
{
    if (NULL == handle) {
        return;
    }

    if (event < NUMBER_OF_EVENT) {
        handle->cb[event] = cb;
    }
}

/**
 * @函数名称: get_button_event()
 * @功能描述: 查询时间是否发生
 * @输入: handle 目标函数结构体
 * @输出: 无
 * @返回值: 按键事件
 */
PRESS_EVENT_TYPDEF get_button_event(BUTTON_TYPDEF* handle)
{
    return (PRESS_EVENT_TYPDEF)(handle->event);
}

/**
 * @函数名称: board_button_start()
 * @功能描述: 开始按键工作，添加函数到工作链表
 * @输入: handle 目标函数结构体
 * @输出: 无
 * @返回值: 0 成功 -1失败
 */
int board_button_start(BUTTON_TYPDEF* handle)
{
    if (NULL == handle) {
        return -1;
    }

    BUTTON_TYPDEF* target = head_handle;
    while (target) {
        if (target == handle) {
            return -1;  
        } 
        target = target->next;
    }
    handle->next = head_handle;
    head_handle  = handle;
    return 0;
}

/**
 * @函数名称: button_stop()
 * @功能描述: 停止按键工作，
 * @输入: handle 目标函数结构体
 * @输出: 无
 * @返回值: 无
 */
void board_button_stop(BUTTON_TYPDEF* handle)
{
    if (NULL == handle) {
        return;
    }

    BUTTON_TYPDEF** curr;
    for (curr = &head_handle; *curr;) {
        BUTTON_TYPDEF* entry = *curr;
        if (entry == handle) {
            *curr = entry->next;
            //			free(entry);
            return;
        }
        else {
            curr = &entry->next;
        }
    }
}

/**
 * @函数名称: button_ticks()
 * @功能描述: 按键计时，周期调用函数，默认10ms
 * @输入: 无
 * @输出: 无
 * @返回值: 无
 */
void board_button_ticks(void)
{
    BUTTON_TYPDEF* target;
    for (target = head_handle; target; target = target->next) {
        board_button_handler(target);
    }
}


/* ------用户-私有实现部分-开始------ */
/* 用户按键id-用户自定义修改 */
enum Button_IDs {
	btn1_id,
};

/* 用户按键-用户自定义修改 */
BUTTON_TYPDEF key1 = {0};

/**
 * @函数名称: read_button_GPIO()
 * @功能描述: 按键读取，用户自定义修改
 * @输入: 无
 * @输出: 无
 * @返回值: 无
 */
static uint8_t read_button_GPIO(uint8_t button_id)
{
	switch(button_id)
	{
		case btn1_id:
			return GPIO_KEY1_READ();
		default:
			return 0;
	}
}

/**
 * @函数名称: boart_buttons_atach()
 * @功能描述: 按键事件触发回调函数连接，用户自定义修改
 * @输入: event 事件
 * @输出: cb 回调函数
 * @返回值: 无
 */
void boart_buttons_atach(PRESS_EVENT_TYPDEF event, button_call_back cb)
{
    board_button_attach(&key1, event, cb);
}

/**
 * @函数名称: boart_buttons_init()
 * @功能描述: 用户按键初始化，用户自定义修改
 * @输入: 无
 * @输出: 无
 * @返回值: 无
 */
void boart_buttons_init(void)
{
    board_button_init(&key1, read_button_GPIO, 0, btn1_id);
    board_button_start(&key1);
}

/**
 * @函数名称: boart_buttons_process()
 * @功能描述: 周期调用 默认10ms
 * @输入: 无
 * @输出: 无
 * @返回值: 无
 */
void boart_buttons_process(void)
{
    board_button_handler(&key1);
}








/* 低功耗睡眠************************************************************************** */
static uint32_t sleep_flag = 0;

/**
 * @函数名称: board_sleep_init()
 * @功能描述: 板卡睡眠初始化
 * @输入: 无
 * @输出: 无
 * @返回值: 无
 */
void board_sleep_init(void)
{
    sleep_flag = 0;
}

/**
 * @函数名称: board_sleep_enter()
 * @功能描述: 进入睡眠模式
 * @输入: 睡眠模式：sleep_mode 唤醒模式：wakeup_mode
 * @输出: 无
 * @返回值: 睡眠模式错误：-1 唤醒模式错误：-2  成功： 0
 */
int8_t board_sleep_enter(uint8_t sleep_mode, uint8_t wakeup_mode)
{
    switch (wakeup_mode) {
    case BOARD_WAKUP_MODE_CAN0:
        gd32f4x_can0_wakeup_enable();
        break;
    case BOARD_WAKUP_MODE_KEY:
        gd32f4x_key_wakeup_init();
        gd32f4x_key_wakeup_enable();
        break;
    default:
        return -2;
    }
    switch (sleep_mode) {
    case BOARD_MODE_SLEEP:
        gd32f4x_mcu_sleep_enter(MCU_MODE_SLEEP);
        break;
    case BOARD_MODE_DEEP_SLEEP:
        gd32f4x_mcu_sleep_enter(MCU_MODE_DEEP_SLEEP);
        break;
    case BOARD_MODE_STANDBY:
        gd32f4x_mcu_sleep_enter(MCU_MODE_STANDBY);
        break;
    default:
        return -1;
    }

    return 0;
}

/**
 * @函数名称: board_sleep_exit()
 * @功能描述: 退出睡眠模式
 * @输入: 无
 * @输出: 无
 * @返回值: 无
 */
void board_sleep_exit(void)
{
    sleep_flag = 0;
    gd32f4x_after_sleep_init_func();
}

/**
 * @函数名称: board_sleep_set_flag()
 * @功能描述: 设置睡眠标志位
 * @输入: 睡眠标志位： flag_value
 * @输出: 无
 * @返回值: 超长：-1 成功 ：0
 */
int8_t board_sleep_set_flag(uint32_t flag_value)
{
    /* 传入的值不能超长 */
    if ((sleep_flag | flag_value) < sleep_flag) {
        return -1;
    }
    sleep_flag |= flag_value;
    return 0;
}

/**
 * @函数名称: board_sleep_period_check()
 * @功能描述: 睡眠标志位检测
 * @输入: 无
 * @输出: 无
 * @返回值: uint8_t 返回睡眠标志位 SLEEP_FLAG_MANUAL SLEEP_FLAG_UNDERVOLTAGE SLEEP_FLAG_TIMEOUT_DEACTIVATED
 */
uint8_t board_sleep_flag_check(void)
{
    return sleep_flag;
}

/* 看门狗************************************************************************** */
static uint32_t fwdgt_flag = 0;
/**
 * @函数名称: board_soft_watchdog_init()
 * @功能描述: 喂狗初始化
 * @输入: 时间ms：times
 * @输出: 无
 * @返回值: 无
 */
void board_soft_watchdog_init(uint32_t times)
{
    gd32f4x_fwdgt_init_ms(times);
}

/**
 * @函数名称: board_soft_watchdog_feed()
 * @功能描述: 喂狗初始化
 * @输入: 无
 * @输出: 无
 * @返回值: 无
 */
void board_soft_watchdog_feed(void)
{
    gd32f4x_fwdgt_feed();
    fwdgt_flag = 0;
}

/**
 * @函数名称: board_soft_watchdog_set_flag()
 * @功能描述: 看门狗flag位设置
 * @输入: flag位
 * @输出: 无
 * @返回值: 无
 */
int8_t board_soft_watchdog_set_flag(uint32_t flag_value)
{
    /* 传入的值不能超长 */
    if ((fwdgt_flag | flag_value) < fwdgt_flag) {
        return -1;
    }
    fwdgt_flag |= flag_value;
    return 0;
}

/**
 * @函数名称: board_soft_watchdog_check()
 * @功能描述: 看门狗flag位设置
 * @输入: flag位
 * @输出: 无
 * @返回值: 0 无需喂狗，1 喂狗,
 */
int8_t board_soft_watchdog_check(void)
{
    if (WATCHDOG_FLAG_ALL == (WATCHDOG_FLAG_ALL & fwdgt_flag )) {
        board_soft_watchdog_feed();
        return 1;
    }

    return 0;
}

/* 时间RTC************************************************************************** */

rtc_alarm_struct set_time;

/**
 * @函数名称: board_rtc_bcd_To_byte()
 * @功能描述: BCD转二进制
 * @输入: value：转换值
 * @输出: 无
 * @返回值: 转换后的值
 */
static uint8_t board_rtc_bcd_To_byte(uint8_t value)
{
    uint8_t tmp;
    tmp = ((value & 0xF0u) >> 4u) * 10u;
    return (tmp + (value & 0x0Fu));
}

/**
 * @函数名称: board_rtc_byte_To_bcd()
 * @功能描述: 二进制转BCD
 * @输入: value：转换值
 * @输出: 无
 * @返回值: 转换后的值
 */
static uint8_t board_rtc_byte_To_bcd(uint8_t value)
{
    uint32_t bcd_high = 0u;
    uint8_t param = value;

    while(param >= 10u) {
        bcd_high++;
        param -= 10u;
    }
    return  ((uint8_t)(bcd_high << 4u) | param);
}

/**
 * @函数名称: board_datatime_set()
 * @功能描述: 设置RTC时间
 * @输入: RTC_DATA_TYPEDEF结构体
 * @输出: 无
 * @返回值: 0：设置成功 1：设置失败
 */
uint8_t board_datatime_set(RTC_DATA_TYPEDEF *rtc_data)
{
    uint8_t return_value = 0;

    if (rtc_data == NULL) {
        /* 如果传入的指针为空，返回错误 */
        return 1; 
    }
    rtc_time_structure.year     = board_rtc_byte_To_bcd(rtc_data->year);
    rtc_time_structure.month    = board_rtc_byte_To_bcd(rtc_data->month);
    rtc_time_structure.date     = board_rtc_byte_To_bcd(rtc_data->day);
    rtc_time_structure.hour     = board_rtc_byte_To_bcd(rtc_data->hour);
    rtc_time_structure.minute   = board_rtc_byte_To_bcd(rtc_data->minute);
    rtc_time_structure.second   = board_rtc_byte_To_bcd(rtc_data->second);
    rtc_time_structure.display_format = RTC_24HOUR;
    rtc_time_structure.am_pm = RTC_AM;
    rtc_time_structure.day_of_week = rtc_data->week;

    if (rtc_init(&rtc_time_structure) != SUCCESS) {
        /* RTC 初始化失败时返回错误标志 */
        return_value = 1;
    }
    return return_value;
}

/**
 * @函数名称: board_datatime_initial_set
 * @功能描述: 配置 RTC 初始时间
 * @输入: RTC_DATA_TYPEDEF结构体
 * @输出: 无
 * @返回值: 0：成功 1: 时间已经被初始化 -1：设置时间失败
 */
int board_datatime_initial_set(RTC_DATA_TYPEDEF *rtc_data)
{
    /* 检查是否已经配置过时间，避免重复设置 */
    if (RTC_BKP0 == BKP_VALUE) {
        /* 时间已经初始化 */
        return 1;
    }
    /* 配置时间 */
    if (board_datatime_set(rtc_data) != 0) {
        /* 设置时间失败 */
        return -1; 
    }
    /* 写入备份寄存器，防止复位后重复设置时间 */
    RTC_BKP0 = BKP_VALUE;
    return 0;
}

/**
 * @函数名称: board_datatime_get()
 * @功能描述: 读取RTC时间
 * @输入: RTC_DATA_TYPEDEF读取结构体
 * @输出: 无
 * @返回值: 无
 */
void board_datatime_get(RTC_DATA_TYPEDEF *rtc_data)
{
    /* 获取当前时间 */
    rtc_current_time_get(&rtc_time_structure);       
    rtc_data->year = board_rtc_bcd_To_byte(rtc_time_structure.year);
    rtc_data->month = board_rtc_bcd_To_byte(rtc_time_structure.month);
    rtc_data->day = board_rtc_bcd_To_byte(rtc_time_structure.date);
    rtc_data->hour = board_rtc_bcd_To_byte(rtc_time_structure.hour);
    rtc_data->minute = board_rtc_bcd_To_byte(rtc_time_structure.minute);
    rtc_data->second = board_rtc_bcd_To_byte(rtc_time_structure.second);
    rtc_data->week = board_rtc_bcd_To_byte(rtc_time_structure.day_of_week);
}

/**
 * @函数名称: board_alarmtime_set()
 * @功能描述: 设置闹钟时间
 * @输入: day：日 hour：时 min：分 sec：秒
 * @输出: 无
 * @返回值: 无
 */
void board_alarmtime_set(uint8_t day, uint8_t hour, uint8_t min, uint8_t sec)
{
    rtc_alarm_disable(RTC_ALARM0);
    /* 日期掩码，只判断时分秒，忽略日期 */
    set_time.alarm_mask = RTC_ALARM_DATE_MASK;
    set_time.weekday_or_date = RTC_ALARM_DATE_SELECTED;
    set_time.alarm_day = board_rtc_byte_To_bcd(day);
    set_time.alarm_hour = board_rtc_byte_To_bcd(hour);
    set_time.alarm_minute = board_rtc_byte_To_bcd(min);
    set_time.alarm_second = board_rtc_byte_To_bcd(sec);
    set_time.am_pm = RTC_AM;
    /* 配置RTC闹钟 */
    rtc_alarm_config(RTC_ALARM0, &set_time); 
    rtc_alarm_enable(RTC_ALARM0);
}

/**
 * @函数名称: board_alarmtime_get()
 * @功能描述: 读取闹钟时间
 * @输入: 无
 * @输出: 无
 * @返回值: 读取闹钟时间结构体
 */
rtc_alarm_struct board_alarmtime_get(void)
{
    /* 获取闹钟时间 */
    rtc_alarm_get(RTC_ALARM0,&set_time);
    return set_time;
}

/* 箱体检测变量 */
ENCLOSURE_TYPDEF enclosure = {0};

/**
 * @函数名称: board_enclosure_detect_process()
 * @功能描述: 箱体检测周期调用 默认10ms
 * @输入: 无
 * @输出: 无
 * @返回值: 无
 */
void board_enclosure_detect_process(void)
{
    ENCLOSURE_STATUS_TYPEDEF status  = ENCLOSURE_OFF;
    if ( enclosure.poweron_time_count < ENCLOSURE_POWER_TIME ) {
        enclosure.poweron_time_count += ENCLOSURE_INTERVAL;

        if (MS0_DET_READ_STATE() == 1) {
            status = ENCLOSURE_ON;
        }
        else {
            status = ENCLOSURE_OFF;
        }
        if (status != enclosure.status) {
            enclosure.detect_time_count += ENCLOSURE_INTERVAL;
            if (enclosure.detect_time_count > ENCLOSURE_DETECT_TIME) {
                enclosure.status = status;
                if(  enclosure.status == ENCLOSURE_ON ) {
                    // printf( "IN SERT!!!!!!!!!!!!!!!\r\n");
                    MS0_OUT_EN_ON(); /* 输出高电平 */
                }
            }
        }
        else {
            enclosure.detect_time_count = 0;
        }
    }
    else {
        return;
    }
}

/**
 * @函数名称: board_enclosure_detect_read()
 * @功能描述: 箱体检测结果读取
 * @输入: out 接收输出箱体输出结果地址
 * @输出: 无
 * @返回值: 0成功 1失败
 */
uint8_t board_enclosure_detect_read(uint16_t* out)
{
    if (NULL == out) {
        return 1;
    }

    *out = enclosure.status;

    if (ENCLOSURE_OFF == enclosure.status) {
        return 1;
    }
    return 0;
}






