#include "SevenSegmentSerial.h"


/**
 * Create a seven segment serial object using the given mode on the specified pins
 * @param m Mode on which to transmit data
 * @param pin1 pin used to transmit data (TX)
 */

SevenSegmentSerial::SevenSegmentSerial(int m, PinName pin1, int baud)
{
    serial = new Serial(pin1, NC);
    mode = UART_MODE;
    baudrate= baud;
    i2cAddr = 0xE2;
    (*serial).baud(baud);
}

/**
 * Create a seven segment serial object using the given mode on the specified pins
 * @param m Mode on which to transmit data
 * @param pin1 pin used to transmit data (SDA or MOSI)
 * @param pin2 second pin used to transmit data (SCL or SCK)
 */
SevenSegmentSerial::SevenSegmentSerial(int m, PinName pin1, PinName pin2, char addr)
{
    mode = m;
    if (mode == SPI_MODE) {
        spi= new SPI(pin1, NC, pin2);
        (*spi).frequency(250000);
    } else {
        i2c= new I2C(pin1, pin2);
        i2cAddr = 0xE2;
        (*i2c).frequency(400000);
    }
    baudrate= 9600;
}
/**
 * Create a seven segment serial object using the given mode on the specified pins
 * @param m Mode on which to transmit data
 * @param pin1 pin used to transmit data (MOSI)
 * @param pin2 second pin used to transmit data (SCK)
 * @param pin3 third pin used to transmit data (SS)
 */
SevenSegmentSerial::SevenSegmentSerial(int m, PinName pin1, PinName pin2, PinName pin3)
{
    spi= new SPI(pin1, NC, pin2, pin3);
    (*spi).frequency(250000);
    mode = SPI_MODE;
    baudrate= 9600;
    i2cAddr = 0xE2;
}


// used to send a character over the specified interface
void SevenSegmentSerial::sendChar(char c)
{
    switch(mode) {
        case(UART_MODE):
            (*serial).putc(c);
            break;
        case (I2C_MODE):
            (*i2c).write(i2cAddr, &c, 1);
            wait_us(50);
            break;
        case (SPI_MODE):
            (*spi).write(c);
            break;
        default:
            break;
    }
}

void SevenSegmentSerial::locate(int pos)
{
    sendChar(0x79);
    sendChar((char)pos);

}

/**
 * Reset display to totally blank
 */
void SevenSegmentSerial::clear()
{
    sendChar((char)0x76);
}

/**
 * Set UART baud rate
 * @param baud Any standard serial baud rate
 */
void SevenSegmentSerial::setBaud(int baud)
{
    char baudcode= 12;
    switch (baud) {
        case(2400):
            baudcode= 0x00;
            break;
        case(4800):
            baudcode= 1;
            break;
        case(9600):
            baudcode= 2;
            break;
        case(14400):
            baudcode= 3;
            break;
        case(19200):
            baudcode= 4;
            break;
        case(38400):
            baudcode= 5;
            break;
        case(57600):
            baudcode= 6;
            break;
        case(76800):
            baudcode= 7;
            break;
        case(115200):
            baudcode= 8;
            break;
        case(250000):
            baudcode= 9;
            break;
        case(500000):
            baudcode= 10;
            break;
        case(1000000):
            baudcode= 11;
            break;
        default:
            break;
    }
    if (baudcode != 12) {
        baudrate= baud;
        sendChar(0x7F);
        sendChar(baudcode);
        (*serial).baud(baud);
    }
    return;
}

/**
 * Set a specific digit on the display
 * @param digit the character to display
 * @param loc the position (0-3) where the char should be displayed
 */
void SevenSegmentSerial::setDigit(char digit, int loc)
{
    sendChar(0x79);
    sendChar((char)loc);
    sendChar(digit);
}

/**
 * Sets decimal points and other nonstandard 7seg segments
 * @param d 8 bits representing each of the 8 variable spots
 */
void SevenSegmentSerial::setDecimal(char d)
{
    sendChar(0x77);
    sendChar(d);
}

/**
 * Set seven segment display brightness
 * @param brightness A value (0-100) to set the display brightness to
 */
void SevenSegmentSerial::setBrightness(int brightness)
{
    if (brightness > 100) {
        brightness = 100;
    } else if (brightness < 0) {
        brightness = 0;
    }
    sendChar(0x7A);
    sendChar((char)brightness);
}

/**
 * Set specific segments at a given location
 * @param segs 8 bits representing which segments to enable (see datasheet for more info)
 * @param loc the position (0-3) where the segs should be enabled
 */
void SevenSegmentSerial::setSegmentsManual(char segs, int loc)
{
    sendChar(0x7B + (char)loc);
    sendChar(segs);
}

/**
 * Reset the display to its factory default baud rate and i2c address
 */
void SevenSegmentSerial::factoryReset()
{
    sendChar(0x81);
}

/**
 * Set specific segments at a given location
 * @param addr a single byte (0-127) representing the new I2C addr
 */
void SevenSegmentSerial::setI2CAddr(char addr)
{
    sendChar(0x80);
    sendChar(addr);
    i2cAddr= addr;
}

/**
 * Write the four least significant digits >0 to the display, includes decimal places if possible
 * @param f the float to be written
 */
void SevenSegmentSerial::write(float f)
{
    char strrep[50];
    int lastChar= sprintf(strrep,"%.4f", f);
    lastChar= lastChar-1;
    if (f < 0) {
        sendChar(strrep[0]);
        if (f > -10) {
            sendChar(strrep[1]);
            sendChar(strrep[3]);
            sendChar(strrep[4]);
            setDecimal(0b00000010);
        } else if (f > -100) {
            sendChar(strrep[1]);
            sendChar(strrep[2]);
            sendChar(strrep[4]);
            setDecimal(0b00000100);
        } else if (f > -1000) {
            sendChar(strrep[1]);
            sendChar(strrep[2]);
            sendChar(strrep[3]);
            sendChar(0x77);
            sendChar(0x00);
        } else {
            int e= (int) log10(-1*f);
            sendChar(strrep[1]);
            sendChar('E');
            sendChar((char)(e % 10));
            sendChar(0x77);
            sendChar(0x00);
        }
    } else {
        if (f <10) {
            sendChar(strrep[0]);
            sendChar(strrep[2]);
            sendChar(strrep[3]);
            sendChar(strrep[4]);
            sendChar(0x77);
            sendChar(0b00000001);
        } else if (f < 100) {
            sendChar(strrep[0]);
            sendChar(strrep[1]);
            sendChar(strrep[3]);
            sendChar(strrep[4]);
            sendChar(0x77);
            sendChar(0b00000010);
        } else if (f < 1000) {
            sendChar(strrep[0]);
            sendChar(strrep[1]);
            sendChar(strrep[2]);
            sendChar(strrep[4]);
            sendChar(0x77);
            sendChar(0b0000100);
        } else if (f < 10000) {
            sendChar(strrep[0]);
            sendChar(strrep[1]);
            sendChar(strrep[2]);
            sendChar(strrep[3]);
            sendChar(0x77);
            sendChar(0b00000000);
        } else {
            int e= (int) log10(f);
            sendChar(strrep[1]);
            sendChar('E');
            if (e >= 10) {
                sendChar((char)(e % 100)/10);
                sendChar((char)(e%10));
            } else {
                sendChar((char)(e%10));
            }
            sendChar(0x77);
            sendChar(0b00000000);
        }
    }
}

/**
 * Write the passed int to the display defaults to 4 LSB
 * @param i the integer to write
 */
void SevenSegmentSerial::write(int i)
{
    clear();
    char strrep[50];
    int lastChar= sprintf(strrep,"%d", i);
    lastChar= lastChar-1;
    if ( i < 0) {

        if (i > -10) {
            locate(2);
            sendChar('-');
            sendChar(strrep[lastChar]);
        } else if (i > -100) {
            locate(1);
            sendChar('-');
            sendChar(strrep[lastChar-1]);
            sendChar(strrep[lastChar]);
        } else {
            sendChar('-');
            sendChar(strrep[lastChar-2]);
            sendChar(strrep[lastChar-1]);
            sendChar(strrep[lastChar]);
        }
    } else {
        if ( i < 10) {
            locate(3);
            sendChar(strrep[lastChar]);
        } else if (i < 100) {
            locate(2);
            sendChar(strrep[lastChar-1]);
            sendChar(strrep[lastChar]);
        } else if (i < 1000) {
            locate(1);
            sendChar(strrep[lastChar-2]);
            sendChar(strrep[lastChar-1]);
            sendChar(strrep[lastChar]);
        } else {
            sendChar(strrep[lastChar-3]);
            sendChar(strrep[lastChar-2]);
            sendChar(strrep[lastChar-1]);
            sendChar(strrep[lastChar]);
        }
    }




}

/**
 * Write the first 4 characters of a string to the display
 * @param s the string to write
 */
void SevenSegmentSerial::write(string s)
{
    int i;
    clear();
    for (i = 0; i < s.length(); i++) {
        sendChar(s[i]);
    }
}


/**
 * Operator Overload - Write the four least significant digits >0 to the display, includes decimal places if possible
 * @param f the float to be written
 */
SevenSegmentSerial& SevenSegmentSerial::operator=(float f)
{
    write(f);
    return *this;
}

/**
 * Operator Overload - Write the passed int to the display defaults to 4 LSB
 * @param i the integer to write
 */
SevenSegmentSerial& SevenSegmentSerial::operator=(int i)
{
    write(i);
    return *this;
}

/**
 * Operator Overload - Write the first 4 characters of a string to the display
 * @param s the string to write
 */
SevenSegmentSerial& SevenSegmentSerial::operator=(string s)
{
    write(s);
    return *this;
}

