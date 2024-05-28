

#ifndef INA219_BODY_GUARD
#define INA219_BODY_GUARD

#include <ina219.h>









// private


/// @brief get a 16 bit register
/// @param registernumber the number of register to get
/// @return the value of the register
uint16_t ina219::_getregister( uint8_t registernumber ) { 

    uint16_t returnbyte = 0;

    Wire.beginTransmission( _i2caddress );
    Wire.write( registernumber );
    Wire.endTransmission();
    Wire.requestFrom( (int8_t)_i2caddress , (int8_t)2 );
    returnbyte |= Wire.read() << 8;
    returnbyte |= Wire.read();

    return returnbyte;

 }



/// @brief set a 16 bit register
/// @param registernumber the number of register to set
/// @param dataword the value to set to the register
void ina219::_setregister( uint8_t registernumber , uint16_t dataword ) { }










// public

/// @brief The constructor for the object
ina219::ina219() {

}

/// @brief The deconstructor for the object
ina219::~ina219() {
}


/// @brief The initialisor for the object
void ina219::begin( uint8_t i2caddress ) {

    // double check someone turned on i2c
    Wire.begin();

    _i2caddress = i2caddress;

    // set calibration and config
    // brng = 0
    // pg = 11
    // badc = 1000
    // sadc = 1000
    // mode = 111

    uint16_t configdata = 0b0000000110011111;

    
    Wire.beginTransmission( _i2caddress );
    Wire.write( 0x00 );
    Wire.write( (uint8_t)( ( configdata >> 8 ) & 0xFF ) );
    Wire.write( (uint8_t)( configdata & 0xFF ) );
    Wire.endTransmission();


    uint16_t calibration = 8192;
    Wire.beginTransmission( _i2caddress );
    Wire.write( 0x05 );
    Wire.write( (uint8_t)( ( calibration >> 8 ) & 0xFF ) );
    Wire.write( (uint8_t)( calibration & 0xFF ) );
    Wire.endTransmission();
    

}





uint16_t ina219::dumpregister( uint8_t registernumber ) {
    return _getregister( registernumber );
}





float ina219::getbusvoltage() {

    return ( ( ( _getregister(0x02) >> 3 ) * 4 ) * 0.001 );

};


float ina219::getshuntvoltage() {

    return ( _getregister(0x01) * 0.01 );

}





#endif


