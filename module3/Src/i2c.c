#include "i2c.h"

#define BUFFER_SIZE 20

uint8_t TWI_Rx_Buffer[BUFFER_SIZE];
const uint8_t* TWI_Tx_Buffer;
uint8_t counter=0;
uint8_t sla;
uint32_t data_size;

void TWI_Init()
{
    PORTC |= 1<<4|1<<5;
    DDRC &= ~(1<<4|1<<5);

    TWBR = 0x48;

    // адрес slave 0x4b
    TWAR = 0x4B << 1;
}

uint8_t TWI_Receive(uint8_t slave_address, uint8_t* data, uint32_t size){
    if (size > BUFFER_SIZE || size <= 0)
        return 1;
    sla = slave_address | 0x01;
    data_size = size;
    counter=0;
    TWCR = (1<<TWINT) | (1<<TWSTA) | (0<<TWSTO) | (1<<TWEN) | (1<<TWIE);
    while (TWCR & (1<<TWIE));
    for (uint32_t i=0; i<size; i++)
        data[i] = TWI_Rx_Buffer[i];
    return 0;
}

uint8_t TWI_Transmit(uint8_t slave_address, const uint8_t* data, uint32_t size){
    if (size > BUFFER_SIZE || size <= 0)
        return 1;
    sla = slave_address;
    data_size = size;

    TWI_Tx_Buffer = data;

    counter=0;
    // Enable TWI and interrupts
    TWCR = (1<<TWINT) | (1<<TWSTA) | (0<<TWSTO) | (1<<TWEN) | (1<<TWIE);
    return 0;
}


uint8_t TWI_Busy(){
    return (TWCR & (1<<TWIE));
}

ISR(TWI_vect){
    switch (TWSR & TW_STATUS_MASK) {
    case TW_REP_START:
    case TW_START:
        // После отправки START или REPEATED_START отправляю SLA+W or SLA+R
        TWDR = sla;
        TWCR = (1<<TWINT) | (0<<TWSTA) | (0<<TWSTO) | (1<<TWEN) | (1<<TWIE);
        break;

    case TW_MT_SLA_ACK:
        // Получаю подтверждение адреса, отправляю первый байт
        TWDR = TWI_Tx_Buffer[counter++];
        TWCR = (1<<TWINT) | (0<<TWSTA) | (0<<TWSTO) | (1<<TWEN) | (1<<TWIE);
        break;

    case TW_MT_DATA_ACK:
        // Получаю подтверждение данных,повторяется до отправки последнего байта
        if (counter<data_size){
            TWDR = TWI_Tx_Buffer[counter++];
            TWCR = (1<<TWINT) | (0<<TWSTA) | (0<<TWSTO) | (1<<TWEN) | (1<<TWIE);
        }
        else{
            TWCR = (1<<TWINT) | (0<<TWSTA) | (1<<TWSTO) | (1<<TWEN) | (0<<TWIE);
            //            state = TWI_Transmitted;
        }
        break;

    case TW_MT_SLA_NACK:
        // не получил подтверждение адреса, заново отправляю START
        TWCR = (1<<TWINT) | (1<<TWSTA) | (0<<TWSTO) | (1<<TWEN) | (1<<TWIE);
        break;

    case TW_MT_ARB_LOST:
        // Arbitration lost
        TWCR = (1<<TWINT) | (1<<TWSTA) | (0<<TWSTO) | (1<<TWEN) | (1<<TWIE);
        break;

    case TW_MT_DATA_NACK:
        // не получил подтверждение данных, заново отправляю эти данные
        counter--;
        TWDR = TWI_Tx_Buffer[counter++];
        TWCR = (1<<TWINT) | (0<<TWSTA) | (0<<TWSTO) | (1<<TWEN) | (1<<TWIE);
        break;


        // MR - Master receiver mode
    case TW_MR_SLA_ACK:
        // SLA+R has been transmitted;
        // ACK has been received
        if (data_size==1)
            TWCR = (1<<TWINT) | (0<<TWEA) | (0<<TWSTA) | (0<<TWSTO) | (1<<TWEN) | (1<<TWIE);
        else
            TWCR = (1<<TWINT) | (1<<TWEA) | (0<<TWSTA) | (0<<TWSTO) | (1<<TWEN) | (1<<TWIE);
        break;

    case TW_MR_SLA_NACK:
        TWCR = (1<<TWINT) | (1<<TWSTA) | (0<<TWSTO) | (1<<TWEN) | (1<<TWIE);
        break;

    case TW_MR_DATA_NACK:
        TWI_Rx_Buffer[counter] = TWDR;
        TWCR = (1<<TWINT) | (0<<TWSTA) | (1<<TWSTO) | (1<<TWEN) | (0<<TWIE);
        break;

    case TW_MR_DATA_ACK:
        TWI_Rx_Buffer[counter++] = TWDR;
        if (counter < (data_size-1)){
            TWCR = (1<<TWINT) | (1<<TWEA) | (0<<TWSTA) | (0<<TWSTO) | (1<<TWEN) | (1<<TWIE);
        }else
            TWCR = (1<<TWINT) | (0<<TWEA) | (0<<TWSTA) | (0<<TWSTO) | (1<<TWEN) | (1<<TWIE);
        break;

    default:
        TWCR = (1<<TWINT) | (0<<TWSTA) | (0<<TWSTO) | (1<<TWEN) | (1<<TWIE);

    }
}
