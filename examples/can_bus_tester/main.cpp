
// Example for send a can 2.0 frame
//
// SEND A CAN20 FRAME EXAMPLE OF SERIAL CAN MODULE
// void can_bus.send(unsigned long id, unsigned char ext, unsigned char rtr, unsigned char fdf, unsigned char len, unsigned char *dta);
// SUPPORT: info@longan-labs.cc

#include "CAN_Controller.h"
#include "Countdown.h"

#define DEVICE_ID   8

#define IS_TRANSMITTER
#define IS_RECEIVER

#define TM_NEXT_MESSAGE    1000

#define uart_terminal   Serial
HardwareSerial          uart_can(0);

#define USE_CAN_MODULE  // Comment to test UART only, uncomment to test CAN bus via UART

#ifdef USE_CAN_MODULE
CAN_Controller can_bus(&uart_can);

unsigned long w_id = 0x01;      // can id
unsigned char w_ext = 0;        // extended frame or standard frame
unsigned char w_rtr = 0;        // remote frame or data frame
unsigned char w_fdf = 0;        // can fd or can 2.0
unsigned char w_len = 8;        // data length
unsigned char w_dta[8] = {1, 2, 3, 4, 5, 6, 7, 8};      // data

unsigned long r_id = 0;         // can id
unsigned char r_ext = 0;        // extended frame or standard frame
unsigned char r_rtr = 0;        // remote frame or data frame
unsigned char r_fdf = 0;        // can fd or can 2.0
unsigned char r_len = 0;        // data length
unsigned char r_dta[8];         // data
#endif

Countdown timer;

uint64_t counter = 0;


void setup()
{
    uart_terminal.begin(115200);

    while(!uart_terminal) {};

    delay(3000);

    uart_terminal.println("CAN sender/receiver test");

    #ifdef USE_CAN_MODULE
        can_bus.init(115200);
        can_bus.set_speed_20(5000000);              // set can bus baudrate to 500k
        //can_bus.can_speed_fd(500000, 1000000);      // set can bus baudrate to 500k-1M
    #else
        uart_can.begin(115200, SERIAL_8N1, -1, -1);
    #endif
    
    timer.set(1000);
}


#ifdef USE_CAN_MODULE
void sendMessage()
{
    w_id = DEVICE_ID; // can id
    w_ext = 0;        // extended frame or standard frame
    w_rtr = 0;        // remote frame or data frame
    w_fdf = 0;        // can fd or can 2.0
    w_len = 8;        // data length

    w_dta[0] = counter;

    can_bus.send(w_id, w_ext, w_rtr, w_fdf, w_len, w_dta);

    counter++;
}
#endif


void loop()
{    
    #ifdef IS_TRANSMITTER
        if(uart_terminal.available()) {
            #ifdef USE_CAN_MODULE
                char ch = uart_terminal.read();

                uart_terminal.write(ch);

                if(ch == ' ') {
                    sendMessage();
                }
            #else
                char ch = uart_terminal.read();
                uart_can.write(ch);
            #endif
        }

        if(timer.timeout()) {
            timer.set(TM_NEXT_MESSAGE);

            #ifdef USE_CAN_MODULE
                uart_terminal.write('.');
                sendMessage();
            #else
                uart_can.write('A');
            #endif
        }
    #endif

    #ifndef USE_CAN_MODULE
        if(uart_can.available()) {
            char ch = uart_can.read();
            Serial.write(ch);
            uart_terminal.write(ch_can);
        }
    #endif

    #ifdef IS_RECEIVER
        #ifdef USE_CAN_MODULE
        if(can_bus.read(&r_id, &r_ext, &r_rtr, &r_fdf, &r_len, r_dta)) {
            uart_terminal.print("GET DATA FROM: 0x");
            uart_terminal.print(r_id, HEX);
            //uart_terminal.print("EXT = ");
            //uart_terminal.println(r_ext);
            //uart_terminal.print("RTR = ");
            //uart_terminal.println(r_rtr);
            //uart_terminal.print("FDF = ");
            //uart_terminal.println(r_fdf);
            uart_terminal.print(", LEN = ");
            uart_terminal.print(r_len);
            uart_terminal.print(" ");
            
            for(int i=0; i<r_len; i++) {
                uart_terminal.printf("%2X ", r_dta[i]);
            }
            uart_terminal.println("");
        }
        #endif
    #endif
}

