


#ifndef TCA9548_BODY_GUARD
#define TCA9548_BODY_GUARD


#include <tca9548.h>


tca9548::tca9548(/* args */) {
}

tca9548::~tca9548() {
}






void tca9548::begin( uint8_t i2caddress ) {

    _i2caddress = i2caddress;

}






void tca9548::setbank( uint8_t bankmask ) {

    Wire.beginTransmission( _i2caddress );
    Wire.write( bankmask );
    Wire.endTransmission();

};



uint8_t tca9548::getbank() {

    Wire.requestFrom( _i2caddress , 1 );
    return Wire.read();

};


















#endif

