#include <mbed.h>

float GetAxisAcc(int addr);
int CalculateAngle(float z);
void Write7SegmentUART(char data);

volatile unsigned int* pinmode0 = reinterpret_cast<volatile unsigned int*>(0x4002C040);
volatile unsigned int* fio0dir0 = reinterpret_cast<volatile unsigned int*>(0x2009C000);
volatile unsigned int* fio0pin0 = reinterpret_cast<volatile unsigned int*>(0x2009C014);

volatile unsigned int* u1thr = reinterpret_cast<volatile unsigned int*>(0x40010000);
volatile unsigned int* u1dll = reinterpret_cast<volatile unsigned int*>(0x40010000);
volatile unsigned int* u1dlm = reinterpret_cast<volatile unsigned int*>(0x40010004);
volatile unsigned int* u1fcr = reinterpret_cast<volatile unsigned int*>(0x40010008);
volatile unsigned int* u1lcr = reinterpret_cast<volatile unsigned int*>(0x4001000C);

uint32_t txBitMask = 0xF0000000;
uint32_t txModeMask = 0xC0 << 20;

volatile unsigned int* pinsel0 = reinterpret_cast<volatile unsigned int*>(0x4002C000);

I2C i2c(p9, p10);

const int accAddr = 0x1D << 1;
const int accZRegisterMSB = 0x05;
const double PI = 3.14159265359;
const int uint14Max = 16383;

int main() {
    SystemInit();

    *fio0dir0 &= ~((1<<5)); //P0.5 Mode input
    *pinmode0 |= (11 << 10); //P0.5 Mode Pull-down

    int baudRate = 240000000 / (16 * 9600);
    char segmentDigits[4];
    *u1lcr = (0x03) | (1<<0x07); //DLAB à 1.
    *u1dll = baudRate & 0xFF;
    *u1dlm = (baudRate >> 0x08) & 0xFF;
    *u1lcr &= ~((1<<0x07)); //DLAB à 0.
    *u1fcr = (1<<0x00)| (1<<0x01) | (1<<0x02); // Active le FIFO.
    *pinsel0 &= ~txBitMask;
    *pinsel0 |= 0x40000000; // Active le port TXD1.

    int resetAngle = 0;
    uint8_t initializeAcc[2] = {0x2A, 0x01};
    i2c.write(accAddr, (char *)initializeAcc, 2);
    
    *u1thr = 0x76;
    *u1thr = 0x7A;
    *u1thr = 0xFF;

    while(1) {
        float zAxis = GetAxisAcc(accZRegisterMSB);
        int angle = CalculateAngle(zAxis);

        if(*fio0pin0 & (1<<5)) {
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

        for(int i = 0; i < 4; i++){
            segmentDigits[i] = (int)angle % 10;
            angle /= 10.0;
            printf("Envoi de %c\n", segmentDigits[i]);
            *u1thr = 0x01;
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

void Write7SegmentUART(char data){
    printf("Envoi de %c\n", data);
    *u1thr = data;
}