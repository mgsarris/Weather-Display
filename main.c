#include<avr/io.h>
#define F_CPU 1000000
#include<util/delay.h>
#define UBRR 12 //U2X0 Must be set to 1!
#include<util/twi.h>
// #include"font5x7.h"

#define SSD1306_WRITE 0b01111000
#define SSD1306_READ 0b01111001

#define CHAR_PER_LINE 25

struct inputUSART{
    char inputData[CHAR_PER_LINE];
}input;
/***************
This is a font array I pulled from the internet.
It was intended for use in c++, but I modified it into a multi-dimensional array
It is ascii compatible... as long as you subtract the character by 20
This only supports ascii characters 0x20-0x7F
****************/
static uint8_t GLCDFONTDECL[][5] = {
//   0x0, 0x0, // size of zero indicates fixed width font,
//   0x05, // width
//   0x07, // height
//   0x20, // first char
//   0x60, // char count
  {0x00, 0x00, 0x00, 0x00, 0x00}, // SPACE
  {0x00, 0x00, 0x5F, 0x00, 0x00}, // !
  {0x00, 0x03, 0x00, 0x03, 0x00}, // "
  {0x14, 0x3E, 0x14, 0x3E, 0x14}, // #
  {0x24, 0x2A, 0x7F, 0x2A, 0x12}, // $
  {0x43, 0x33, 0x08, 0x66, 0x61}, // %
  {0x36, 0x49, 0x55, 0x22, 0x50}, // &
  {0x00, 0x05, 0x03, 0x00, 0x00}, // '
  {0x00, 0x1C, 0x22, 0x41, 0x00}, // (
  {0x00, 0x41, 0x22, 0x1C, 0x00}, // )
  {0x14, 0x08, 0x3E, 0x08, 0x14}, // *
  {0x08, 0x08, 0x3E, 0x08, 0x08}, // +
  {0x00, 0x50, 0x30, 0x00, 0x00}, // ,
  {0x08, 0x08, 0x08, 0x08, 0x08}, // -
  {0x00, 0x60, 0x60, 0x00, 0x00}, // .
  {0x20, 0x10, 0x08, 0x04, 0x02}, // /
  {0x3E, 0x51, 0x49, 0x45, 0x3E}, // 0
  {0x00, 0x04, 0x02, 0x7F, 0x00}, // 1
  {0x42, 0x61, 0x51, 0x49, 0x46}, // 2
  {0x22, 0x41, 0x49, 0x49, 0x36}, // 3
  {0x18, 0x14, 0x12, 0x7F, 0x10}, // 4
  {0x27, 0x45, 0x45, 0x45, 0x39}, // 5
  {0x3E, 0x49, 0x49, 0x49, 0x32}, // 6
  {0x01, 0x01, 0x71, 0x09, 0x07}, // 7
  {0x36, 0x49, 0x49, 0x49, 0x36}, // 8
  {0x26, 0x49, 0x49, 0x49, 0x3E}, // 9
  {0x00, 0x36, 0x36, 0x00, 0x00}, // :
  {0x00, 0x56, 0x36, 0x00, 0x00}, // ;
  {0x08, 0x14, 0x22, 0x41, 0x00}, // <
  {0x14, 0x14, 0x14, 0x14, 0x14}, // =
  {0x00, 0x41, 0x22, 0x14, 0x08}, // >
  {0x02, 0x01, 0x51, 0x09, 0x06}, // ?
  {0x3E, 0x41, 0x59, 0x55, 0x5E}, // @
  {0x7E, 0x09, 0x09, 0x09, 0x7E}, // A
  {0x7F, 0x49, 0x49, 0x49, 0x36}, // B
  {0x3E, 0x41, 0x41, 0x41, 0x22}, // C
  {0x7F, 0x41, 0x41, 0x41, 0x3E}, // D
  {0x7F, 0x49, 0x49, 0x49, 0x41}, // E
  {0x7F, 0x09, 0x09, 0x09, 0x01}, // F
  {0x3E, 0x41, 0x41, 0x49, 0x3A}, // G
  {0x7F, 0x08, 0x08, 0x08, 0x7F}, // H
  {0x00, 0x41, 0x7F, 0x41, 0x00}, // I
  {0x30, 0x40, 0x40, 0x40, 0x3F}, // J
  {0x7F, 0x08, 0x14, 0x22, 0x41}, // K
  {0x7F, 0x40, 0x40, 0x40, 0x40}, // L
  {0x7F, 0x02, 0x0C, 0x02, 0x7F}, // M
  {0x7F, 0x02, 0x04, 0x08, 0x7F}, // N
  {0x3E, 0x41, 0x41, 0x41, 0x3E}, // O
  {0x7F, 0x09, 0x09, 0x09, 0x06}, // P
  {0x1E, 0x21, 0x21, 0x21, 0x5E}, // Q
  {0x7F, 0x09, 0x09, 0x09, 0x76}, // R
  {0x26, 0x49, 0x49, 0x49, 0x32}, // S
  {0x01, 0x01, 0x7F, 0x01, 0x01}, // T
  {0x3F, 0x40, 0x40, 0x40, 0x3F}, // U
  {0x1F, 0x20, 0x40, 0x20, 0x1F}, // V
  {0x7F, 0x20, 0x10, 0x20, 0x7F}, // W
  {0x41, 0x22, 0x1C, 0x22, 0x41}, // X
  {0x07, 0x08, 0x70, 0x08, 0x07}, // Y
  {0x61, 0x51, 0x49, 0x45, 0x43}, // Z
  {0x00, 0x7F, 0x41, 0x00, 0x00}, // [
  {0x02, 0x04, 0x08, 0x10, 0x20}, // backslash
  {0x00, 0x00, 0x41, 0x7F, 0x00}, // ]
  {0x04, 0x02, 0x01, 0x02, 0x04}, // ^
  {0x40, 0x40, 0x40, 0x40, 0x40}, // _
  {0x00, 0x01, 0x02, 0x04, 0x00}, // `
  {0x20, 0x54, 0x54, 0x54, 0x78}, // a
  {0x7F, 0x44, 0x44, 0x44, 0x38}, // b
  {0x38, 0x44, 0x44, 0x44, 0x44}, // c
  {0x38, 0x44, 0x44, 0x44, 0x7F}, // d
  {0x38, 0x54, 0x54, 0x54, 0x18}, // e
  {0x04, 0x04, 0x7E, 0x05, 0x05}, // f
  {0x08, 0x54, 0x54, 0x54, 0x3C}, // g
  {0x7F, 0x08, 0x04, 0x04, 0x78}, // h
  {0x00, 0x44, 0x7D, 0x40, 0x00}, // i
  {0x20, 0x40, 0x44, 0x3D, 0x00}, // j
  {0x7F, 0x10, 0x28, 0x44, 0x00}, // k
  {0x00, 0x41, 0x7F, 0x40, 0x00}, // l
  {0x7C, 0x04, 0x78, 0x04, 0x78}, // m
  {0x7C, 0x08, 0x04, 0x04, 0x78}, // n
  {0x38, 0x44, 0x44, 0x44, 0x38}, // o
  {0x7C, 0x14, 0x14, 0x14, 0x08}, // p
  {0x08, 0x14, 0x14, 0x14, 0x7C}, // q
  {0x00, 0x7C, 0x08, 0x04, 0x04}, // r
  {0x48, 0x54, 0x54, 0x54, 0x20}, // s
  {0x04, 0x04, 0x3F, 0x44, 0x44}, // t
  {0x3C, 0x40, 0x40, 0x20, 0x7C}, // u
  {0x1C, 0x20, 0x40, 0x20, 0x1C}, // v
  {0x3C, 0x40, 0x30, 0x40, 0x3C}, // w
  {0x44, 0x28, 0x10, 0x28, 0x44}, // x
  {0x0C, 0x50, 0x50, 0x50, 0x3C}, // y
  {0x44, 0x64, 0x54, 0x4C, 0x44}, // z
  {0x00, 0x08, 0x36, 0x41, 0x41}, // {
  {0x00, 0x00, 0x7F, 0x00, 0x00}, // |
  {0x41, 0x41, 0x36, 0x08, 0x00}, // }
  {0x02, 0x01, 0x02, 0x04, 0x02}, // ~
  {0X7F, 0X7F, 0X7F, 0X7F, 0X7F}  // del (I think this can be used as a cursor?)
};



void USART_Init(void)
{
    // /*Set baud rate*/
    UBRR0H = UBRR>>8;
    UBRR0L = UBRR;
    //Enable receiver and xmitter
    UCSR0B = (1<<TXEN0)|(1<<RXEN0);
    UCSR0A = (1<<U2X0);
    //No Parity, 1 stop bit, 8-bit character
    UCSR0C = (1<<UCSZ01)|(1<<UCSZ00);
}

void TWI_Init(void){
    TWBR = 12;
}

unsigned char ssd1306_start(void){
    uint8_t twst;

    // send START condition
    TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);

    // wait until transmission completed
    while(!(TWCR & (1<<TWINT)));

    // check value of TWI Status Register. Mask prescaler bits.
    twst = TW_STATUS & 0xF8;
    if ( (twst != TW_START) && (twst != TW_REP_START)) return 1;

    // send device address
    TWDR = SSD1306_WRITE;
    TWCR = (1<<TWINT) | (1<<TWEN);

    // wail until transmission completed and ACK/NACK has been received
    while(!(TWCR & (1<<TWINT)));

    // check value of TWI Status Register. Mask prescaler bits.
    twst = TW_STATUS & 0xF8;
    if ( (twst != TW_MT_SLA_ACK) && (twst != TW_MR_SLA_ACK) ) return 1;

    return 0;
}

void i2c_stop(void)
{
    /* send stop condition */
    TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO);
    
    // wait until stop condition is executed and bus released
    while(TWCR & (1<<TWSTO));

}/* i2c_stop */

unsigned char i2c_write( unsigned char data )
{	
    uint8_t   twst;
    
    // send data to the previously addressed device
    TWDR = data;
    TWCR = (1<<TWINT) | (1<<TWEN);

    // wait until transmission completed
    while(!(TWCR & (1<<TWINT)));

    // check value of TWI Status Register. Mask prescaler bits
    twst = TW_STATUS & 0xF8;
    if( twst != TW_MT_DATA_ACK) return 1;
    return 0;

}

void i2c_start_wait(void)
{
    uint8_t   twst;


    while ( 1 )
    {
        // send START condition
        TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);
    
        // wait until transmission completed
        while(!(TWCR & (1<<TWINT)));
    
        // check value of TWI Status Register. Mask prescaler bits.
        twst = TW_STATUS & 0xF8;
        if ( (twst != TW_START) && (twst != TW_REP_START)) continue;
    
        // send device address
        TWDR = SSD1306_WRITE;
        TWCR = (1<<TWINT) | (1<<TWEN);
    
        // wail until transmission completed
        while(!(TWCR & (1<<TWINT)));
    
        // check value of TWI Status Register. Mask prescaler bits.
        twst = TW_STATUS & 0xF8;
        if ( (twst == TW_MT_SLA_NACK )||(twst ==TW_MR_DATA_NACK) ) 
        {    	    
            /* device busy, send stop condition to terminate write operation */
            TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO);
            
            // wait until stop condition is executed and bus released
            while(TWCR & (1<<TWSTO));
            
            continue;
        }
        //if( twst != TW_MT_SLA_ACK) return 1;
        break;
     }

}

void ssd1306_command(uint8_t c){
    ssd1306_start();
    i2c_write(0);
    i2c_write(c);
    i2c_stop();
}

void ssd1306_Init()
{
    ssd1306_command(0xAE);
    ssd1306_command(0xD5);
    ssd1306_command(0x80);
    ssd1306_command(0xA8);
    ssd1306_command(31);
    ssd1306_command(0xD3);
    ssd1306_command(0x0);
    ssd1306_command(0x40 | 0x0);
    ssd1306_command(0x8D);
    ssd1306_command(0x14);
    ssd1306_command(0x20);
    ssd1306_command(0x0);
    ssd1306_command(0xA0|0x1);
    ssd1306_command(0xC8);
    ssd1306_command(0xDA);
    ssd1306_command(0x02);
    ssd1306_command(0x81);
    ssd1306_command(0x8F);
    ssd1306_command(0xD9);
    ssd1306_command(0xF1);
    ssd1306_command(0xDB);
    ssd1306_command(0x40);
    ssd1306_command(0xA4);
    ssd1306_command(0xA6);
    ssd1306_command(0x2E);
    ssd1306_command(0xAF);
}

void USART_Transmit(unsigned char data)
{
    while (!(UCSR0A & (1<<UDRE0))){;}
    UDR0=data;
}

void USART_Receive(void)
{
    uint8_t i = 0;
    for(i=0;i<CHAR_PER_LINE;i++)
    {
        input.inputData[i] = 0x20;
    }
    i = 0;
    //Wait for data to be received
    while (i < CHAR_PER_LINE)
    {
        while( !(UCSR0A & (1<<RXC0)) ){;}
        input.inputData[i] = UDR0;
        USART_Transmit(input.inputData[i]);
        if (input.inputData[i] == '\n' || input.inputData[i] == '\r')
        {
            break;
        }
        i++;
    }
}

void printString(char * string)
{
    while(*string != '\0'){
        while (!(UCSR0A & (1<<UDRE0))){;}
        UDR0=*string++;
    }
}

int main(void)
{
    //setup
    USART_Init();

    TWI_Init();
    TWI_Init();

    ssd1306_start();
    ssd1306_Init();

    ssd1306_command(0x21);  // COLUMNADDR
    ssd1306_command(0);     // COL START ADDRESS
    ssd1306_command(127);   // COL END ADDRESS

    ssd1306_command(0x22);  // PAGE ADDR
    ssd1306_command(0);     // PAGE START ADDR
    ssd1306_command(3);     // PAGE END ADDR

    

    uint16_t i = 0;
    uint8_t x = 0;
    // uint8_t j = 0;

    //clearing screen
    ssd1306_start();
    i2c_write(0x40);
    for (i=0; i<(128*32/8); i++) 
    {
        i2c_write(0x00);
    }
    i2c_stop();

    char test[]= "Current conditions:";
    for (i = 0; i<(sizeof(test)/sizeof(char)-1);i++)
    {
        ssd1306_start();
        i2c_write(0x40);
        for(x=0;x<5;x++)
        {
            i2c_write(GLCDFONTDECL[test[i]-0x20][x]);
        }
        i2c_stop();
    }

    

    printString("Finished inits!\n\r");


    while(1)
    {
        ssd1306_command(0xB1);
        ssd1306_command(0x21);
        ssd1306_command(0);
        ssd1306_command(127);
        USART_Receive();
        ssd1306_start();
        i2c_write(0x40);
        for(i = 0;i<128;i++)
        {
            i2c_write(0x00);
        }
        i2c_stop();
        ssd1306_command(0xB1);
        ssd1306_command(0x21);
        ssd1306_command(0);
        ssd1306_command(127);
        //printString("Finished Receiving! Here's what you entered:\n\r");
        for (i = 0; i<(sizeof(input.inputData)/sizeof(char)-1);i++)
        {
            if(input.inputData[i] == '\0'|| input.inputData[i] == '\n' || input.inputData[i] == '\r')
            {
                break;
            }
            ssd1306_start();
            i2c_write(0x40);
                for(x=0;x<5;x++)
                {
                    i2c_write(GLCDFONTDECL[input.inputData[i]-0x20][x]);
                }
            i2c_stop();
        }
        USART_Transmit('\n');
        USART_Transmit('\r');
        
        ssd1306_command(0xB2);
        ssd1306_command(0x21);
        ssd1306_command(0);
        ssd1306_command(127);
        USART_Receive();
        ssd1306_start();
        i2c_write(0x40);
        for(i = 0;i<128;i++)
        {
            i2c_write(0x00);
        }
        i2c_stop();
        ssd1306_command(0xB2);
        ssd1306_command(0x21);
        ssd1306_command(0);
        ssd1306_command(127);
        for (i = 0; i<(sizeof(input.inputData)/sizeof(char)-1);i++)
        {
            if(input.inputData[i] == '\0'|| input.inputData[i] == '\n' || input.inputData[i] == '\r')
            {
                break;
            }
            ssd1306_start();
            i2c_write(0x40);
                for(x=0;x<5;x++)
                {
                    i2c_write(GLCDFONTDECL[input.inputData[i]-0x20][x]);
                }
            i2c_stop();
        }
        USART_Transmit('\n');
        USART_Transmit('\r');
    }
    return 0;
}
