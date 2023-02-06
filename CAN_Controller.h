#ifndef __SERIAL_CAN_FD_H__
#define __SERIAL_CAN_FD_H__

#include <Arduino.h>
#include "SerialType.h"

#define CAN_REC_BUF_SIZE    100

class CAN_Controller {
    private:
        SERIAL_LIB* uart;
        int dtaLen;

        unsigned char dtaCan[CAN_REC_BUF_SIZE];

        unsigned long char2long(unsigned char *str);
        void long2char(unsigned long num, unsigned char *str);
        unsigned int CrcCalculate(unsigned char Byte, unsigned int _CRC);
        unsigned int calCRC(unsigned char *str);
        unsigned char checkCRC(unsigned char *str);

        void strProcess(int num);
        void checkData();
        void serialProcess();
        void dtaProcess(int len, unsigned char *dta);

    public:
        CAN_Controller(HardwareSerial* _uart);
        ~CAN_Controller();

        void init(unsigned long baud, int8_t rxPin = -1, int8_t txPin = -1);
        void set_speed_fd(unsigned long speed_20, unsigned long speed_fd);
        void set_speed_20(unsigned long speed_20);
        void send(unsigned long id, unsigned char ext, unsigned char rtr, unsigned char fdf, unsigned char len, unsigned char *dta);
        int read(unsigned long *id, unsigned char *ext, unsigned char *rtr, unsigned char *fdf, unsigned char *len, unsigned char *dta);
        void set_mask_filt(unsigned char num, unsigned char ext, unsigned long mask, unsigned long filt);
        void set_baudrate(unsigned long __baud);

};

#endif