#include "mbed.h"
#include "MMA8452.h"
#include <sstream>
#include <ctype.h>
#include "SevenSegmentSerial.h"


using namespace std;
DigitalOut myled(LED1);
SevenSegmentSerial mydisplay(SPI_MODE, p11, p13, p14);
Serial pc(USBTX, USBRX);
MMA8452 acc(p9, p10, 40000);

const double PI = 3.14159265359;
const int addr = 0xE2; //7-Segment Serial display default address is 0xE2 (8 bit format, mbed compatible)

int main() {
    char digits[11];
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
    digits[10] = 0x2E;
  /*    
    double x = 0, y = 0, z = 0;  
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
   }*/
   
   
   // - - - -  - test lecture convertion angle accelero 
   // Accelero acc;
    
    double angle = 0;
    double repAngle =0;
/** calculation de l'angle avec code non-personnel, simply to test da shit. theese should all be functions and s ------ **/  
    while(true){
    double x=0, y=0, z=0;
    acc.readXYZGravity(&x,&y,&z);
    double ZValue = z;
     if(ZValue>1){
        ZValue = 1;
    }
    if(ZValue <-1){
        ZValue = -1;    
    }
    double  angle =std::acos(ZValue);
     // std::acos return a value from 0 to pi, adjust it to 0 to PI/2 ( 0 , 90 )
    if(angle > PI/2)
    {
        angle = PI - angle;
    }
    repAngle =  (angle*180/ PI);

/** apres la calculation, on tente d'ecrire sur le 7 segment **/


    ostringstream strs;
    strs << repAngle;
    string str = strs.str();
    string seg = str.substr(0, 4);
    pc.printf("angle is : %s    ",seg);

    pc.printf("toseg is : %s \n",seg);
/** test print 7seg with  ------ **/
    
//    i2c.write(addr, toseg, 4);
   mydisplay = seg;
        wait(0.9);
    }  
    
}
