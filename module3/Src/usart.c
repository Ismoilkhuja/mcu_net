#include "usart.h"

void USART0_Init(){
    // 115200 бод
    UBRR0L = 16;
    // U2X0 - Double the USART Transmission Speed
    UCSR0A |= (1<<U2X0);
    // Передатчик и приемник USART, //также прерывание по получение//
    UCSR0B = (1<<RXEN0)|(1<<TXEN0)/*|(1<<RXCIE0)*/;
    // 8-битная передача
    UCSR0C = (1<<UCSZ01)|(1<<UCSZ00);
}

void USART0_Transmit(uint8_t data){
    while (!(UCSR0A & (1 << UDRE0)));
    UDR0 = data;
}

void USART0_Transmit_Buffer(const char *buffer){
    while (*buffer) USART0_Transmit(*buffer++);
}

uint8_t USART_Receive() {
    while (!(UCSR0A & (1<<RXC0)));
    return UDR0;
}

void USART_Receive_Buffer(uint8_t* buffer, uint32_t size){
    for (uint32_t i =0; i<size; i++)
        buffer[i] = USART_Receive();
}
