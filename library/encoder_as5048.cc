#include "library/encoder_as5048.h"

void AS5048Encoder::Zero() {
    zero_position_ = SampleRaw();
}

uint16_t AS5048Encoder::SampleRaw() {
    // Select the device by seting chip select low.
    cs_ = 0;
    
    // Query encoder.
    uint16_t raw_position_ = spi_.write(0xFFFF);
    //Extract last 14 bits.
    raw_position_ &= 0x3FFF;
        
    // Deselect the device.
    cs_ = 1;

    return raw_position_;
}

uint16_t AS5048Encoder::SampleRawZeroed() {
    return SampleRaw() - zero_position_;
}