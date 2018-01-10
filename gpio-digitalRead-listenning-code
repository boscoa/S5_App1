#include "mbed.h"
 
DigitalIn myInputPin(p5);
 
int main() {
    myInputPin.mode(PullUp);  //set the mbed to use a pullup resistor
    while(1) {        
        if(myInputPin.read()==1) {    //a shorter way to write this is:    if(myInputPin){
            //insert code to do something if the input value is 1.
        }
    }
}