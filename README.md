# Tomas Hujer CAN BUS Controller Arduino Library

Arduino library for Serial CAN-FD BUS Module.

[Forked and remaked from Longan Labs module](https://github.com/Longan-Labs/Serial_CAN_FD_Module.git)

With this library, you can,

1. Send a CAN2.0 frame
2. Receive a CAN2.0 frame
3. Send a CAN FD frame
4. Receive a CAN FD frame

## Installation
1. [Download the library](https://github.com/thujer/arduino-can-controller/archive/refs/heads/main.zip)
2. Extract the zip file
3. In the Arduino IDE, navigate to Sketch > Include Library > Add .ZIP Library

## Respository Contents

* [**/examples**](./examples) - Example sketches for the library (.ino). Run these from the Arduino IDE.
* [**/src**](./src) - Source files for the library (.cpp, .h).
* [**keywords.txt**](./keywords.txt) - Keywords from this library that will be highlighted in the Arduino IDE.
* [**library.properties**](./library.properties) - General library properties for the Arduino package manager.

## Functions

- init
- set_speed_fd
- set_speed_20
- send
- read
- set_mask_filt
- set_baudrate

## Examples

here are many examples implemented in this library. One of the examples is below. You can find other examples [here](./examples)

```Cpp
// example for send a can 2.0 frame
// SEND A CAN20 FRAME EXAMPLE OF SERIAL CAN MODULE
// void can_send(unsigned long id, unsigned char ext, unsigned char rtr, unsigned char fdf, unsigned char len, unsigned char *dta);
// SUPPORT: info@longan-labs.cc

#include <Arduino.h>
#include "CAN_Controller.h"

HardwareSerial uart_can(0); // Init Serial0

CAN_Controller can_bus(&uart_can);  // Init CAN bus controller with Serial0


void setup()
{
    Serial.begin(115200);

    can_bus.init(115200);
    can_bus.set_speed_20(5000000);          // set can bus baudrate to 500k
}


void loop()
{
    unsigned long __id = 0x01;      // can id
    unsigned char __ext = 0;        // extended frame or standard frame
    unsigned char __rtr = 0;        // remote frame or data frame
    unsigned char __fdf = 0;        // can fd or can 2.0
    unsigned char __len = 8;        // data length
    unsigned char __dta[8] = {1, 2, 3, 4, 5, 6, 7, 8};      // data
    
    can_bus.send(__id, __ext, __rtr, __fdf, __len, __dta);
    delay(100);
}

// END FILE
```

## Get a Dev Board

If you need a Dev board, plese try,

[Waveshare SN65HVD230 CAN Board](https://www.waveshare.com/sn65hvd230-can-board.htm)
[Waveshare SN65HVD230 CAN Board on RPIShop](https://rpishop.cz/can-bus/5002-waveshare-sn65hvd230-can-board.html)

[TJA1050 CAN Controller Interface Module BUS Driver Interface Module](https://www.elecbee.com/en-31575-TJA1050-CAN-Controller-Interface-Module-BUS-Driver-Interface-Module?utm_term=&utm_campaign=&utm_source=adwords&utm_medium=ppc&hsa_acc=9958698819&hsa_cam=18719854073&hsa_grp=140153146062&hsa_ad=630917925115&hsa_src=g&hsa_tgt=pla-1186173532455&hsa_kw=&hsa_mt=&hsa_net=adwords&hsa_ver=3)

Probably compatible, but not tested
- [Grove - CAN BUS Module based on GD32E103](https://www.seeedstudio.com/Grove-CAN-BUS-Module-based-on-GD32E103-p-5456.html)


## License

```
MIT License

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
```

## Contact us

If you have any question, please feel free to contact [tomas@hujer.eu](tomas@hujer.eu)
