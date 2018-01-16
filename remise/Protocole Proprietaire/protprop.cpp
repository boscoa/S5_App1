#include <mbed.h>

//InterruptIn  inputDataPin(p21); //la pine 21 est aussi celle qui correspond au data sur le 2e mbed
DigitalIn  inputDataPin(p21); 
DigitalIn inputClockPin(p22);// la pine 22 est aussi celle qui correspond au clock sur le 2e mbed
Serial pc(USBTX, USBRX);

bool justRead = false;
char data [30];
char clk [30];
int cpt =0;



int main() {
     pc.printf("main");
      // inputDataPin.rise(&flip);  // on rleie l'adresse de la fonction flip lorsque le gpio recois un rising edge
       inputClockPin.mode(PullDown);  //
       inputDataPin.mode(PullDown);
       
       while(inputClockPin.read()!=0) { } 
       while(inputClockPin.read()!=1) { }   // on se synchronise sur un versant montant puis descendant pour etre certain de commencer nos wait
                                                                               // pile au moment ou le clock monte a 1
         
       while(1) { // on loop a chaque 0.004 seconde pour lire en symbiose avec le clock
    
             clk[cpt] = inputClockPin.read();
             cpt++;
             justRead = true;
             
             
            // on attend d'avoir nos 7 coup de clock pour le data pour partir lenregistrement du data, question d'etre certain de nos lectures                     
             if(clk[cpt] =='1' || clk[cpt-1] =='1' || clk[cpt-2] =='1' || clk[cpt-3] =='1' || clk[cpt-4] =='1' || clk[cpt-5] =='1' || clk[cpt-6] =='1'){
                 
               while(1){ // loop partant tout de suite apres une suite de 7 coup de clock confirmant du data valide
                 
               while(inputClockPin.read()!=1){ wait(0.004);}// on attend le premier coup de clock a 1 indiquant le debut du bon data 
               
               data [0] = inputDataPin.read();
               wait(0.004);
               
               data [1] = inputDataPin.read();
               wait(0.004);
               
               data [2] = inputDataPin.read();
               wait(0.004);
               
               data [3] = inputDataPin.read();
               wait(0.004);
               
               data [4] = inputDataPin.read();
               wait(0.004);
               
               data [5] = inputDataPin.read();
               wait(0.004);
               
               data [6] = inputDataPin.read();
               wait(0.004);

// ajouter chaque cas different avec un appel a la fonction voulu
               if(data[0] == '' &&  data[1] == '' &&   data[2] == '' &&   data[3] == '' &&  data[4] == '' &&  data[5] == '' &&  data[6] == ''){ 
                
               }
               
            
        
        
        }
    
    }

}



