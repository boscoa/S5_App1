#include "mbed.h"
#include "SevenSegmentSerial.h"

// initialize new 7-Segment Serial display using SPI  mosi, sclk, SS(optional)
SevenSegmentSerial mydisplay(SPI_MODE, p11, p13, p14);

int main()
{
    // write text to the display
    mydisplay= "-HI-";
    wait(1.5);
    mydisplay= "GAGA";
    wait(1.5);
        mydisplay= "GANG";
    wait(1.5);

    while (1) {
        // count down to -1000
        for (int i = 0; i > -1000; i--) {
            mydisplay.clear();
            mydisplay= i;
            wait_ms(100);
        }
    }
}
