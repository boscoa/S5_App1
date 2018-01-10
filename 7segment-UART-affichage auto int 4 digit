// sparkfun spi/serial 4 digits 7-segment display
// 0-9999 counter the hard way
// 0x7B digit 1
// 0x7C digit 2
// 0x7D digit 3
// 0x7E digit 4 
// 0=63     5=109
// 1=6      6=124
// 2=91     7=7
// 3=79     8=127
// 4=102    9=103
//
#include "mbed.h"

Serial sd(p28, p27);
int cijfer[10]={63,6,91,79,102,109,124,7,127,103};

int main() {
    
    int a, b=0, c=0, d=0;
    sd.putc(0x76); // reset display
    sd.putc(0x7A); // brightness
    sd.putc(0x00); // value
    
    int entry = 6549; // lentree recu est mis en tableau puis afficher sur le 7 segent en ordre.
                    // le tableau est populer a laide de modulo puis enleve le chiffre entree dans le tableau a laide d'une division par 10
    
    int entryArray[4];

for (int i = 3; i >= 0; i--) {
    entryArray[i] = entry % 10;
    entry /= 10;
}
     sd.putc(0x7E);
            sd.putc(cijfer[entryArray[3]]);
            sd.putc(0x7D);
            sd.putc(cijfer[entryArray[2]]);
            sd.putc(0x7C);
            sd.putc(cijfer[entryArray[1]]);
            sd.putc(0x7B);
            sd.putc(cijfer[entryArray[0]]);
   /* while (1) {
        for (a=0; a<10; a++) {
            sd.putc(0x7E);
            sd.putc(cijfer[a]);
            sd.putc(0x7D);
            sd.putc(cijfer[b]);
            sd.putc(0x7C);
            sd.putc(cijfer[c]);
            sd.putc(0x7B);
            sd.putc(cijfer[d]);
            wait_ms(10);
        }
        if (b<9) b++;
        else {
            b=0;
            if (c<9) c++;
            else {
                c=0;
                if (d<9) d++;
                else d=0;
            }
        }*/

    }

