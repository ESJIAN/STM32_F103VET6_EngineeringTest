#include"Voice.h"


/**
 * ������ģ�鷢������
 * 
 * ������ͨ��USART2���ڽӿڣ�������ģ�鷢��һ������
 * ����ȴ�ÿ���ֽڱ�������Ϻ�ż���������һ���ֽ�
 * 
 * @param data Ҫ���͵���������
 * @param length Ҫ���͵����ݳ���
 */
void SendData_to_Voice_modle(uint8_t *data, uint8_t length) {
    for (uint8_t i = 0; i < length; i++) {
        // �ȴ� USART ���ݼĴ����գ�׼������
        while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);

        // ����һ���ֽ�����
        USART_SendData(USART2, data[i]);
    }
}




/*******************************************************************************
* ������         : Voice_INit
* ����           : ����ģ���ʼ��,����ʱ��ʹ��,GPIO����,USART����,USART�ж�ʹ��,NVIC����,ʹ��USART
* ����           : ����ģ��ͨ�Ų�����
* ���           : ��
* ����           : ��
*******************************************************************************/
void Voice_modle_Init(u32 Voice_SerialBaudRate)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    // 1. ���� USART2 �� GPIO ʱ��
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);  // USART2 ʹ�õ��� APB1 ʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);   // GPIOA ��Ȼ��Ҫ

    // 2. ���� GPIO
    // TX: PA2 (USART2)
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // RX: PA3 (USART2)
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // 3. USART2 ����
    USART_InitStructure.USART_BaudRate = Voice_SerialBaudRate;  // ����Ҫ�滻Ϊ���Լ��Ĳ����ʱ���
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
    USART_Init(USART2, &USART_InitStructure);

    // 4. ʹ�� USART2 �����ж�
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);

    // 5. NVIC ����
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;  // USART1 ����Ϊ USART2
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    // 6. ʹ�� USART2
    USART_Cmd(USART2, ENABLE);



}



