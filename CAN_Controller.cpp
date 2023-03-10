

#include "CAN_Controller.h"


CAN_Controller::CAN_Controller(HardwareSerial* _uart) {
    uart = _uart;
    nRecIndexWrite = 0;
    memset(recBuffer, 0, sizeof(recBuffer));

    nRecIndexWrite = 0;
    nRecIndexRead = 0;
    nRecInBuf = 0;
}


CAN_Controller::~CAN_Controller() {

}


void CAN_Controller::init(unsigned long baud, int8_t rxPin, int8_t txPin) {
    Serial.printf("CAN UART init RX=%d, TX=%d, ptr=%p\r\n", rxPin, txPin, uart);
    uart->begin(baud, SERIAL_8N1, rxPin, txPin);
    nRecIndexWrite = 0;
}


unsigned long CAN_Controller::char2long(unsigned char *str) {
    unsigned long num = 0;
    memcpy((unsigned char *)(&num), str, 4);
    return num;
}


void CAN_Controller::long2char(unsigned long num, unsigned char *str) {
    memcpy(str, (unsigned char*)(&num), 4);
}


unsigned int CAN_Controller::CrcCalculate(unsigned char Byte, unsigned int _CRC) {
    unsigned char Carry;
    int Bit_Index;
    _CRC = _CRC ^ (Byte & 0xFF);
    for (Bit_Index = 0; Bit_Index < 8; Bit_Index++)
    {
        Carry = _CRC & 0x0001;
        _CRC = _CRC >> 1;
        if (Carry)
        _CRC = _CRC ^ 0xA001;
    }
    return _CRC;
}


unsigned int CAN_Controller::calCRC(unsigned char *str) {
    unsigned int __crc = 0xffff;
    int i=0;

    for(i=0; i < str[1]; i++) {
        __crc = CrcCalculate(str[i+2], __crc);
    }
    return __crc;
}


unsigned char CAN_Controller::checkCRC(unsigned char *str) {
    unsigned int __crc = calCRC(str);
    unsigned int __crc0 = str[3+str[1]];

    __crc0 <<= 8;
    __crc0 |= str[2+str[1]];

    return (__crc == __crc0);
}

// unsigned long id, unsigned char ext, unsigned char rtr, unsigned char fdf, unsigned char len
void CAN_Controller::send(CAN_MESSAGE_CONTROL* msgControl, unsigned char *dta) {
    unsigned char str[80];

    str[0] = 0xAA;
    str[1] = msgControl->m_nLen + 9;
    str[2] = 0x01;
    long2char(msgControl->m_nId, &str[3]);
    str[7] = msgControl->m_nExt;
    str[8] = msgControl->m_nRtr;
    str[9] = msgControl->m_nFdf;
    str[10] = msgControl->m_nLen;
    for(int i=0; i < msgControl->m_nLen; i++)
    {
        str[11+i] = dta[i];
    }

    unsigned int crc = calCRC(str);
    str[11 + msgControl->m_nLen] = crc & 0xff;
    str[12 + msgControl->m_nLen] = (crc>>8) & 0xff;

    Serial.println("SND RAW:");

    for(int i=0; i < (13 + msgControl->m_nLen); i++) {
        uart->write(str[i]);

        Serial.printf("%d ", str[i]);
    }

    Serial.println();
}


void CAN_Controller::set_speed_fd(unsigned long speed_20, unsigned long speed_fd) {
    unsigned char str[80];
    str[0] = 0xAA;
    str[1] = 0x09;
    str[2] = 0x02;
    long2char(speed_20, &str[3]);
    long2char(speed_fd, &str[7]);


    unsigned int crc = calCRC(str);
    str[str[1]+2] = crc & 0xff;
    str[str[1]+3] = (crc>>8) & 0xff;

    for(int i=0; i<(4+str[1]); i++)
    {
        uart->write(str[i]);
    }
}


void CAN_Controller::set_speed_20(unsigned long speed_20) {
    set_speed_fd(speed_20, 4000000);
}


/*
void CAN_Controller::checkData() {

    int __len = 0;
    unsigned char __dta[CAN_REC_BUF_SIZE];

    for(int i=0; i<nRecIndexWrite; i++) {

        if(recBuffer[0] != 0xAA)
        {
            strProcess(1);
        }
        else break;
    }

    if(nRecIndexWrite>2) {

        if(nRecIndexWrite >= (recBuffer[1] + 4)) {

            __len = nRecIndexWrite;
            memcpy(__dta, recBuffer, __len);
            strProcess(__len);
            dtaProcess(__len, __dta);
        }
    }
}
*/


/*
void CAN_Controller::resetRecBuf() {
    nRecIndexWrite = 0;
    memset(recBuffer, 0, sizeof(recBuffer));
}
*/

void CAN_Controller::serialProcess() {

    if(uart->available()) {
        Serial.printf("CAN serialProcess %d avail\r\n", uart->available());
    }

    if(nRecInBuf < (CAN_REC_BUF_SIZE - uart->available())) {

        while(uart->available()) {

            char incomingChar = (char) uart->read(); 

            nRecIndexWrite %= CAN_REC_BUF_SIZE;
            recBuffer[nRecIndexWrite] = incomingChar;
            
            Serial.printf("CAN Added %c [%d, %Xh] Ix: %d\r\n", recBuffer[nRecIndexWrite], recBuffer[nRecIndexWrite], recBuffer[nRecIndexWrite], nRecIndexWrite);

            nRecIndexWrite++;
            nRecInBuf++;
        }

    } else {
        Serial.printf("!!! CAN receive buffer FULL !!! writeIndex=%d, InBuf: %d\r\n", nRecIndexWrite, nRecInBuf);
    }
}


void CAN_Controller::dtaProcess(int len, unsigned char *dta) {

    int i=0;
    unsigned long tmp = 0;
    int __crc = checkCRC(dta);

    if(__crc)
    {
        Serial.println("CAN: get dta crc ok");
    }
}


void CAN_Controller::findNextMessage() {
    for(; nRecInBuf && (recBuffer[nRecIndexRead] != 0xAA); nRecIndexRead++) {
        nRecInBuf--;
        Serial.printf("Skipped not AA, ixWr: %d, inBuf: %d\r\n", nRecIndexRead, nRecInBuf);
    }
}

// unsigned long *id, unsigned char *ext, unsigned char *rtr, unsigned char *fdf, unsigned char *len
int CAN_Controller::read(CAN_MESSAGE_CONTROL* structControlData, unsigned char *dta) {

    serialProcess();

    if(!nRecInBuf) {
        return 0;
    }

    // Found message start
    findNextMessage();

    if(!nRecInBuf) {
        return 0;
    }

    if(nRecInBuf && recBuffer[nRecIndexRead] == 0xAA) {
        Serial.printf("AA found, ixWr: %d, inBuf: %d\r\n", nRecIndexRead, nRecInBuf);
    } else {
        Serial.printf("Message start (AA) not found, ixWr: %d, inBuf: %d\r\n", nRecIndexRead, nRecInBuf);
    }
   
    if(nRecInBuf > 2) {

        uint8_t nMsgLength = recBuffer[((nRecIndexRead + 1) % CAN_REC_BUF_SIZE)];

        Serial.printf("Msg length: %d\r\n", nMsgLength);

        if(((nMsgLength + 4) >= CAN_REC_BUF_SIZE)) {
            Serial.printf("CAN: Msg length is bigger than buffer: %d/%d\r\n", nMsgLength + 4, CAN_REC_BUF_SIZE);

            // Skip char
            nRecIndexRead ++;
            nRecInBuf--;    

            return 0;
        }

        if(nRecInBuf < (nMsgLength + 4)) {
            Serial.printf("CAN: Invalid msg length inside: %d\r\n", nMsgLength + 4);

            // Skip char
            nRecIndexRead ++;
            nRecInBuf--;    

            return 0;
        }

        unsigned char __dta[CAN_REC_BUF_SIZE];
        
        for(uint8_t ix=0; ix < nMsgLength + 4; ix++) {

            nRecIndexRead %= CAN_REC_BUF_SIZE;
            __dta[ix] = recBuffer[nRecIndexRead];

            Serial.printf("%d ", __dta[ix]);

            nRecIndexRead++;
            nRecInBuf--;

            if(!nRecInBuf) {
                break;
            }
        }

        Serial.println();

        int __crc = checkCRC(__dta);

        if(__crc) {

            Serial.println("CAN: CRC OK, parsing...");

            structControlData->m_nId = char2long(&__dta[3]);
            structControlData->m_nExt = __dta[7];
            structControlData->m_nRtr = __dta[8];
            structControlData->m_nFdf = __dta[9];
            structControlData->m_nLen = __dta[10];
            
            for(int i=0; i < structControlData->m_nLen; i++) {
                dta[i] = __dta[11 + i];
            }

            // Reset message buffer
            memset(__dta, 0, sizeof(__dta));

            return 1;
        }
        else {
            Serial.println("CAN: BAD CRC");

            // Reset message buffer
            memset(__dta, 0, sizeof(__dta));

            return 0;
        }
    } else {
        Serial.println("CAN: Msg size is shorter than minimum");

        // Skip char
        nRecIndexRead ++;
        nRecInBuf--;    
    }
    
    return 0;
}


void CAN_Controller::set_mask_filt(unsigned char num, unsigned char ext, unsigned long mask, unsigned long filt) {
    if(num > 3) {
        return;
    }
    
    unsigned char str[15];
    str[0] = 0xAA;
    str[1] = 11;
    str[2] = 0x03 + num;
    str[3] = 1;
    str[4] = ext;
   
    long2char(mask, &str[5]);
    long2char(filt, &str[9]);
    
    unsigned int crc = calCRC(str);
    str[13] = crc & 0xff;
    str[14] = (crc>>8) & 0xff;

    for(int i=0; i<15; i++)
    {
        uart->write(str[i]);
    }
}


void CAN_Controller::set_baudrate(unsigned long __baud) {

    unsigned char str[15];
    str[0] = 0xAA;
    str[1] = 5;
    str[2] = 7;
   
    long2char(__baud, &str[3]);
    
    unsigned int crc = calCRC(str);
    str[7] = crc & 0xff;
    str[8] = (crc>>8) & 0xff;

    for(int i=0; i<9; i++)
    {
        uart->write(str[i]);
    }
    
    delay(10);
}


