#include "mbed.h"
 
InterruptIn inputDataPin(p21); //la pine 21 est aussi celle qui correspond au data sur le 2e mbed
DigitalIn InputClockPin(p22);// la pine 22 est aussi celle qui correspond au clock sur le 2e mbed
Serial pc(USBTX, USBRX);

char now;
int zeroCpt;
int oneCpt;
bool manyZero;
char data [7];
void flip() {
    led = !led;
}
 
int main() {

	InputClockPin.rise(&flip);  // on rleie l'adresse de la fonction flip lorsque le gpio recois un rising edge
	myInputPin.mode(PullDown);  //
	
	while(InputClockPin.read()!=0) {} 
	while(InputClockPin.read()!=1) {}   // on se synchronise sur un versant montant puis descendant pour etre certain de commencer nos wait
																			// pile au moment ou le clock monte a 1
		
	while(1) { // on loop a chaque 0.004 seconde pour lire en symbiose avec le clock
	if(InputClockPin.read()==0){
		zeroCpt++;
		oneCpt =0;
		if(zeroCpt>2)
			manyZero = true;
	}
	else{
		if(manyZero){ // on detecte que plusieurs zeros viennent de passer et que nous sommes sur un 1 alors le data arrive
		manyZero = false;			 
			for( int i = 0; a < 7; i = i + 1 ) {
						data[i] = inputDataPin.read();
						wait(0.004); 
				}
				  pc.printf("7 data =  %c  \n  ",data);
		}
		zeroCpt=0
		oneCpt++;
	}
		
		
			wait(0.004); 
			}
	
		}
	}
}
