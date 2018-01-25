#include <mbed.h>
#include <string.h>

void Write7SegmentSPI(char * data);
float GetAxisAcc(int addr);
int CalculateAngle(float z);

SPI spi(p11, NC, p13);
I2C i2c(p9, p10);

DigitalOut led(LED1);
DigitalOut cs(p14);
DigitalIn button(p29); 

const int accAddr = 0x1D << 1;
const int accZRegisterMSB = 0x05;
const double PI = 3.14159265359;
const int uint14Max = 16383;

int main() {
    int offset = 0;
    uint8_t initializeAcc[2] = {0x2A, 0x01};

    char segmentDigits[4];

    cs = 1;
    spi.frequency(250000);
    cs = 0;

    spi.write(0x76);
    spi.write(0x7A);
    spi.write(0x64); //Éclairage à 100;

    i2c.write(accAddr, (char *)initializeAcc, 2);

    button.mode(PullDown);

    while(1) {
        float zAxis = GetAxisAcc(accZRegisterMSB);
        int angle = CalculateAngle(zAxis);
        printf("z Axis :%f", zAxis);
        printf("\n");        
        printf("Angle :%d", angle);
        printf("\n");
        if(button) {
            offset = angle;
        }

        if(offset != 0) {
            angle -= offset;

            if(angle < 0){
                angle *= -1;
            }
        }



        for(int i = 0; i < 4; i++){
            segmentDigits[i] = (int)angle % 10;
            angle /= 10.0;
        }

        Write7SegmentSPI(segmentDigits);

        wait(0.1);
    }
}

float GetAxisAcc(int addr) {
    uint8_t data[2];
    char axisRegister[1] = {(char)addr};
    i2c.write(accAddr, axisRegister, 1, true);
    i2c.read(accAddr, (char *)data, 2);

    int16_t axisAcc = (data[0] << 6) | (data[1] >> 2);
    if (axisAcc > uint14Max/2)
        axisAcc -= uint14Max;
 
    return axisAcc / 4096.0;
}

int CalculateAngle(float z){
    if(z > 1){
        z = 1;
    }
    if(z < -1){
        z = -1;    
    }

    double angle = std::acos(z);

    if(angle > PI/2)
    {
        angle = PI - angle;
    }

    return (int)(angle*180/PI);
}

void Write7SegmentSPI(char * data){
            spi.write(data[3]);
            spi.write(data[2]);
            spi.write(data[1]);
            spi.write(data[0]);
}