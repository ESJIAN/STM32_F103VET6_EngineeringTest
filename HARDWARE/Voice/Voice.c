#include"Voice.h"


/**
 * 向语音模块发送数据
 * 
 * 本函数通过USART2串口接口，向语音模块发送一组数据
 * 它会等待每个字节被发送完毕后才继续发送下一个字节
 * 
 * @param data 要发送的数据数组
 * @param length 要发送的数据长度
 */
void SendData_to_Voice_modle(uint8_t *data, uint8_t length) {
    for (uint8_t i = 0; i < length; i++) {
        // 等待 USART 数据寄存器空，准备发送
        while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);

        // 发送一个字节数据
        USART_SendData(USART2, data[i]);
    }
}




/*******************************************************************************
* 函数名         : Voice_INit
* 描述           : 语音模块初始化,包括时钟使能,GPIO配置,USART配置,USART中断使能,NVIC配置,使能USART
* 输入           : 语音模块通信波特率
* 输出           : 无
* 返回           : 无
*******************************************************************************/
void Voice_modle_Init(u32 Voice_SerialBaudRate)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    // 1. 开启 USART2 和 GPIO 时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);  // USART2 使用的是 APB1 时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);   // GPIOA 仍然需要

    // 2. 配置 GPIO
    // TX: PA2 (USART2)
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // RX: PA3 (USART2)
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // 3. USART2 配置
    USART_InitStructure.USART_BaudRate = Voice_SerialBaudRate;  // 你需要替换为你自己的波特率变量
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
    USART_Init(USART2, &USART_InitStructure);

    // 4. 使能 USART2 接收中断
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);

    // 5. NVIC 配置
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;  // USART1 更改为 USART2
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    // 6. 使能 USART2
    USART_Cmd(USART2, ENABLE);



}



