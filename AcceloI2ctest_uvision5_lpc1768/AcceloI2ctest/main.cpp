#include "mbed.h"
#include "MMA8452.h"

DigitalOut myled(LED1);
I2C i2c(p9, p10);
Serial pc(USBTX, USBRX);
MMA8452 acc(p9, p10, 40000);


const int addr = 0xE2; //7-Segment Serial display default address is 0xE2 (8 bit format, mbed compatible)

int main() {


    
    double x = 0, y = 0, z = 0;
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

    
    acc.setBitDepth(MMA8452::BIT_DEPTH_12);
    acc.setDynamicRange(MMA8452::DYNAMIC_RANGE_4G);
    acc.setDataRate(MMA8452::RATE_100);

    //pc.baud(115200);
    
    pc.printf("starting the code \n");
    while(1) {
      if(!acc.isXYZReady()) {
         wait(0.1);
         continue;
      }
      acc.readXYZGravity(&x,&y,&z);
      pc.printf("Gravities: %lf %lf %lf\r\n",x,y,z);
   }  
    
}
