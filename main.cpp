#include "mbed.h"
#include "library/encoder_as5048.h"
 
// SPI spi_(PC_12, PC_11, PC_10, NC); // mosi, miso, sclk
// DigitalOut cs(PB_6, 1);
RawSerial pc(USBTX, USBRX); // tx, rx
 
int main() {
    pc.baud(9600);
    
    AS5048Encoder::Options enc_options{PC_12, PC_11, PC_10, PB_6};
    AS5048Encoder encoder(enc_options);

    int count{0};
    double sample{0};
    uint16_t old_counts{0};
    int rotations{0};
    int CPR{16384};

    pc.printf("sample,encoder\n");

    encoder.Zero();

    while (true) {    
        //Extract last 14 bits
        uint16_t result = encoder.SampleRaw();

        uint16_t angle = result;                                               // Correct for nonlinearity with lookup table from calibration
        if(angle - old_counts > CPR/2){
            rotations -= 1;
        }
        else if (angle - old_counts < -CPR/2){
            rotations += 1;
        }
    
        old_counts = angle;
        if (count == 1000) {
            pc.printf("%f,%05d\n", sample, result + rotations*CPR - encoder.GetZeroPosition());
            count = 0;
            sample++;
        }
        count++;
    }
}