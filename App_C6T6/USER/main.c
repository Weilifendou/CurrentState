#include "led.h"
#include "delay.h"
#include "sys.h"
#include "uart.h"
#include "str.h"
#include "oled.h"
#include "cncode.h"
#include "dht11.h"
#include "pwm.h"
#include "timer.h"
#include "iwdg.h"


/*********************移植时务必注意修改************************/
//务必注意修改 FLASH_APP_ADDR 0x08002800 //第一个应用程序起始地址(存放在FLASH)
//务必注意修改 STM32_FLASH_SIZE 32 //所选STM32的FLASH容量大小(单位为K)
/***************************************************************/

extern u8 FunCode;


int main(void)
{
    char text[20] = {0};
    u8 delayer = 30;
    NVIC_SetVectorTable(0x08000000, 0x2800);
    delay_init(); //必须要调用，否则延时不准确
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
    UART_Init(115200); 
    LED_Init(); //LED初始化
    DHT11_Init(); //温湿度传感器初始化
    OLED_Init(); //屏幕初始化
    OLED_Fill(0x00);
    ClearStrBuff();
    ClearStr(text);
    AddStr(text, "CURRENT STATE");
    OLED_InsertStr(0, 1, text, 1);
    OLED_ThreeLineCenterValidate();
    ClearStrBuff();
    delay_ms(1000);
    delay_ms(1000);
    delay_ms(1000);
//    TIM3_PWM_Init(1999, 719); //PWM波初始化，周期为20ms
//    TIM2_Int_Init(5999, 7199); //定时器2初始化定时1s
    Iwdg_Init();
    
    while(1)
    {
        ClearStrBuff();
        //显示当前温度
        ClearStr(text);
        AddStr(text, "T: ");
        AddStr(text, FloatToString(Temperature / 10.0, 1));
        AddStr(text, "deg     ");
        OLED_InsertStr(2, 0, text, 0);
        
        //显示当前湿度
        ClearStr(text);
        AddStr(text, "H: ");
        AddStr(text, FloatToString(Humidty / 10.0, 1));
        AddStr(text, "%     ");
        OLED_InsertStr(2, 1, text, 0);
        
        //显示当前角度
//        ClearStr(text);
//        AddStr(text, IntToString(white));
//        AddStr(text, " ");
//        AddStr(text, IntToString(yellow));
//        AddStr(text, " ");
//        AddStr(text, IntToString(steer));
//        OLED_InsertStr(2, 2, text, 0);
        
        OLED_ThreeLineCenterValidate();
        
        if (delayer >= 30) {
            delayer = 0;
            DHT11_ReadData(&Temperature, &Humidty);
        }
        if (FunCode == 1 || FunCode == 3) {
            FunCode = 0;
            NVIC_SystemReset();
        }
        IWDG_ReloadCounter();
        delay_ms(200);
        delayer++;
    }
} 

