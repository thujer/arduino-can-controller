#ifndef __SERIAL_CAN_FD_H__
#define __SERIAL_CAN_FD_H__

#include <Arduino.h>
#include "SerialType.h"

#define CAN_REC_BUF_SIZE    200

struct CAN_MESSAGE_CONTROL {
    unsigned long m_nId;
    unsigned char m_nExt;
    unsigned char m_nRtr;
    unsigned char m_nFdf;
    unsigned char m_nLen;
    unsigned char m_nDta;
};


class CAN_Controller {
    private:
        SERIAL_LIB* uart;

        uint8_t nRecIndexWrite;
        uint8_t nRecIndexRead;
        uint8_t nRecInBuf;

        unsigned char recBuffer[CAN_REC_BUF_SIZE];

        unsigned long char2long(unsigned char *str);
        void long2char(unsigned long num, unsigned char *str);
        unsigned int CrcCalculate(unsigned char Byte, unsigned int _CRC);
        unsigned int calCRC(unsigned char *str);
        unsigned char checkCRC(unsigned char *str);

        void strProcess(int num);
        void checkData();
        void serialProcess();
        void findNextMessage();
        void dtaProcess(int len, unsigned char *dta);

        void resetRecBuf();

    public:
        CAN_Controller(HardwareSerial* _uart);
        ~CAN_Controller();

        void init(unsigned long baud, int8_t rxPin = -1, int8_t txPin = -1);
        void set_speed_fd(unsigned long speed_20, unsigned long speed_fd);
        void set_speed_20(unsigned long speed_20);
        void send(unsigned long id, unsigned char ext, unsigned char rtr, unsigned char fdf, unsigned char len, unsigned char *dta);

        // unsigned long *id, unsigned char *ext, unsigned char *rtr, unsigned char *fdf, unsigned char *len
        int read(CAN_MESSAGE_CONTROL* structControlData, unsigned char *dta);

        void set_mask_filt(unsigned char num, unsigned char ext, unsigned long mask, unsigned long filt);
        void set_baudrate(unsigned long __baud);

};

#endif