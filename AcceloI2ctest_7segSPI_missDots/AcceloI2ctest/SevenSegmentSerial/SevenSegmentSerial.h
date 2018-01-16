#ifndef __SevenSegmentSerial_H__
#define __SevenSegmentSerial_H__

#include "mbed.h"
#include <string>
#include <math.h>

#define UART_MODE 0
#define SPI_MODE 1
#define I2C_MODE 2
/** Seven Segment Serial Display Class.  
 * Made for SparkFun product "Serial 7 Seven Segment Display"
 * available modes: "SPI_MODE", "I2C_MODE", "UART_MODE"
 */
class SevenSegmentSerial
{
    public:
    
    /** 
     * Create a seven segment serial object using the given mode on the specified pins.  
     *   Single pin constructor only supports m="UART_MODE"
     * @param m Mode on which to transmit data
     * @param pin1 pin used to transmit data (TX pin for Serial)
     */
    SevenSegmentSerial(int m, PinName pin1, int baud= 9600);
    
    /** 
     * Create a seven segment serial object using the given mode on the specified pins.  
     *   2 pin constructor supports m= SPI_MODE and I2C_MODE
     * @param m Mode on which to transmit data
     * @param pin1 pin used to transmit data (SDA or MOSI)
     * @param pin2 second pin used to transmit data (SCL or SCK)
     */
    SevenSegmentSerial(int m, PinName pin1, PinName pin2, char addr= 0xE2);
    
    /** 
     * Create a seven segment serial object using the given mode on the specified pins.  
     *   3 pin constructor only supports m = SPI_MODE
     * @param m Mode on which to transmit data
     * @param pin1 pin used to transmit data (MOSI)
     * @param pin2 second pin used to transmit data (SCK)
     * @param pin3 third pin used to transmit data (SS)
     */
    SevenSegmentSerial(int m, PinName pin1, PinName pin2, PinName pin3);
    

    
    /**
     * Reset display to totally blank
     */ 
    void clear();
    
    /**
     * Set UART baud rate
     * @param baud Any standard serial baud rate
     */ 
    void setBaud(int baud);
    
    /**
     * Set a specific digit on the display
     * @param digit the character to display
     * @param loc the position (0-3) where the char should be displayed
     */ 
    void setDigit(char digit, int loc);
    
    /**
     * Sets decimal points and other nonstandard 7seg segments
     * @param d 8 bits representing each of the 8 variable spots
     */
    void setDecimal(char d);
    
    /**
     * Set seven segment display brightness
     * @param brightness A value (0-100) to set the display brightness to
     */
    void setBrightness(int brightness);
    
    /**
     * Set specific segments at a given location
     * @param segs 8 bits representing which segments to enable (see datasheet for more info)
     * @param loc the position (0-3) where the segs should be enabled
     */
    void setSegmentsManual(char segs, int loc);
    
    /**
     * Reset the display to its factory default baud rate and i2c address
     */
    void factoryReset();
    
    /**
     * Set specific segments at a given location
     * @param addr a single byte (0-127) representing the new I2C addr
     */
    void setI2CAddr(char addr);
    
    /**
     * Write the four least significant digits >0 to the display, includes decimal places if possible
     * @param f the float to be written
     */
    void write(float f);
    
    /**
     * Write the passed int to the display defaults to 4 LSB
     * @param i the integer to write
     */
    void write(int i);
    
    /**
     * Write the first 4 characters of a string to the display
     * @param s the string to write
     */
    void write(string s);
    
    
    /**
     * Operator Overload - Write the four least significant digits >0 to the display, includes decimal places if possible
     * @param f the float to be written
     */
    SevenSegmentSerial& operator=(float f);
    
    /**
     * Operator Overload - Write the passed int to the display defaults to 4 LSB
     * @param i the integer to write
     */
    SevenSegmentSerial& operator=(int i);
    
    /**
     * Operator Overload - Write the first 4 characters of a string to the display
     * @param s the string to write
     */
    SevenSegmentSerial& operator=(string s);
    
    /**
     * Send a character over the correct interface
     * @param c the char to send
     */
    void sendChar(char c);
    
    /**
     * Move the cursor to the specified position
     * @param l integer representing the position to shift to (0-3)
     */
    void locate(int l);
    private:
    // current data transmit mode
   
    
    // used to send a character over the specified interface

    

protected:

    // pointers to each possible interface
    Serial* serial;
    SPI* spi;
    I2C* i2c;
    
    // keeps track of which comm mode the device is in
    int mode;
    
    // current baud rate (defaults to 9600)
    int baudrate;
    
    // current i2c addr (defaults to 0xE2)
    char i2cAddr;
       
};
#endif