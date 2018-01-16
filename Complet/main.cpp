#include <mbed.h>
#include <lpc17xx.h>

float GetAxisAcc(int addr);
int CalculateAngle(float z);

I2C i2c(p9, p10);

const int accAddr = 0x1D << 1;
const int accZRegisterMSB = 0x05;
const double PI = 3.14159265359;
const int uint14Max = 16383;

int main() {
    SystemInit();
    // LPC_GPIO1->FIODIR |= (1<<18);
    LPC_GPIO0->FIODIR &= ~((1<<5));
    LPC_PINCON->PINMODE0 |= (11<<10); //
    //LPC_PINCON

    int resetAngle = 0;
    uint8_t initializeAcc[2] = {0x2A, 0x01};

    i2c.write(accAddr, (char *)initializeAcc, 2);

    while(1) {
        float zAxis = GetAxisAcc(accZRegisterMSB);
        int angle = CalculateAngle(zAxis);

        if(LPC_GPIO0->FIOPIN & (1<<5)) {
            printf("salut!");
            resetAngle = angle;
        }

        printf("Angle before : %d", angle);
        printf("\n");
        printf("resetAngle : %d", resetAngle);
        printf("\n");

        if(resetAngle != 0) {
            angle -= resetAngle;

            if(angle < 0){
                angle *= -1;
            }
        }

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