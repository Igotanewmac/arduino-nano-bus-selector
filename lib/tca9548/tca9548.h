

#ifndef TCA9548_HEADER_GUARD
#define TCA9548_HEADER_GUARD


#include <Arduino.h>

#include <Wire.h>



class tca9548 {

    private:

        uint8_t _i2caddress;

    public:

        tca9548(/* args */);
        ~tca9548();

        void begin( uint8_t i2caddress );

        void setbank( uint8_t bankmask );
        uint8_t getbank();

};








#endif