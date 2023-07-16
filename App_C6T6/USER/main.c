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


/*********************��ֲʱ���ע���޸�************************/
//���ע���޸� FLASH_APP_ADDR 0x08002800 //��һ��Ӧ�ó�����ʼ��ַ(�����FLASH)
//���ע���޸� STM32_FLASH_SIZE 32 //��ѡSTM32��FLASH������С(��λΪK)
/***************************************************************/

extern u8 FunCode;


int main(void)
{
    char text[20] = {0};
    u8 delayer = 30;
    NVIC_SetVectorTable(0x08000000, 0x2800);
    delay_init(); //����Ҫ���ã�������ʱ��׼ȷ
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
    UART_Init(115200); 
    LED_Init(); //LED��ʼ��
    DHT11_Init(); //��ʪ�ȴ�������ʼ��
    OLED_Init(); //��Ļ��ʼ��
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
//    TIM3_PWM_Init(1999, 719); //PWM����ʼ��������Ϊ20ms
//    TIM2_Int_Init(5999, 7199); //��ʱ��2��ʼ����ʱ1s
    Iwdg_Init();
    
    while(1)
    {
        ClearStrBuff();
        //��ʾ��ǰ�¶�
        ClearStr(text);
        AddStr(text, "T: ");
        AddStr(text, FloatToString(Temperature / 10.0, 1));
        AddStr(text, "deg     ");
        OLED_InsertStr(2, 0, text, 0);
        
        //��ʾ��ǰʪ��
        ClearStr(text);
        AddStr(text, "H: ");
        AddStr(text, FloatToString(Humidty / 10.0, 1));
        AddStr(text, "%     ");
        OLED_InsertStr(2, 1, text, 0);
        
        //��ʾ��ǰ�Ƕ�
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

