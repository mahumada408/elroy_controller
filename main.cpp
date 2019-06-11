#include "mbed.h"
#include "library/encoder_as5048.h"
 
SPI spi_(PC_12, PC_11, PC_10, NC); // mosi, miso, sclk
DigitalOut cs(PB_6, 1);
RawSerial pc(USBTX, USBRX); // tx, rx
 
int main() {
    pc.baud(9600);
    
    spi_.format(16, 1);
    spi_.frequency(1000000);

    int count{0};
    double sample{0};
    uint16_t old_counts{0};
    int rotations{0};
    int CPR{16384};

    pc.printf("sample,encoder\n");

    // Zero the encoder
    cs = 0;
    uint16_t zero_angle = spi_.write(0xFFFF);
    zero_angle &= 0x3FFF;
    cs = 1;

    while (true) {    
        // Select the device by seting chip select low
        cs = 0;
    
        //Extract last 14 bits
        uint16_t result = spi_.write(0xFFFF);
        result &= 0x3FFF;
        
        // Deselect the device
        cs = 1;

        uint16_t angle = result;                                               // Correct for nonlinearity with lookup table from calibration
        if(angle - old_counts > CPR/2){
            rotations -= 1;
        }
        else if (angle - old_counts < -CPR/2){
            rotations += 1;
        }
    
        old_counts = angle;
        if (count == 1000) {
            pc.printf("%f,%05d\n", sample, result + rotations*CPR - zero_angle);
            count = 0;
            sample++;
        }
        count++;
    }
}