#pragma once

#include "mbed.h"

class AS5048Encoder {
    public:
      struct Options {
        PinName mosi;
        PinName miso;
        PinName sck;
        PinName cs;
      };

      AS5048Encoder(const Options& options) : spi_(options.mosi, options.miso, options.sck, NC),
                                        cs_(options.cs, 1) {
        spi_.format(16, 1);
        spi_.frequency(1000000);
      }

      void Zero();
      uint16_t SampleRaw();
      uint16_t SampleRawZeroed();
      uint16_t GetZeroPosition();
    private:
      mbed::SPI spi_;
      uint16_t raw_position_;
      uint16_t zero_position_;
      uint16_t raw_zeroed_;
      DigitalOut cs_;

};