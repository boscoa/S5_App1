#include "mbed.h"
 
DigitalOut myled(LED1);
 
I2C i2c(p9, p10);
 
const int addr = 0xE2; //7-Segment Serial display default address is 0xE2 (8 bit format, mbed compatible)
 
int main() {
     char digits[10];
     digits[0] = 0x30;
     digits[1] = 0x31;
      digits[2] = 0x32;
       digits[3] = 0x33;
        digits[4] = 0x34;
         digits[5] = 0x35;
          digits[6] = 0x36;
           digits[7] = 0x37;
            digits[8] = 0x38;
             digits[9] = 0x39;
              
     
    char cmd[4]; //[0x31][0x32][0x41][0x42] (ASCII-code for '1', '2', 'a', 'b')
    
    cmd[0] =    digits[0];
    cmd[1] =    digits[6];
    cmd[2] =    digits[6];
    cmd[3] =    digits[9];
    
    i2c.write(addr, cmd, 4);
    
    while(1) {
        myled = 1;
        wait(0.2);
        myled = 0;
        wait(0.2);
    }
} 