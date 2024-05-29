

#ifndef INA219_BODY_GUARD
#define INA219_BODY_GUARD

#include <ina219.h>




#define INA219_REGISTER_CONFIGURATION 0x00
#define INA219_REGISTER_SHUNT 0x01
#define INA219_REGISTER_BUS 0x02
#define INA219_REGISTER_POWER 0x03
#define INA219_REGISTER_CURRENT 0x04
#define INA219_REGISTER_CALIBRATION 0x05





// private



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



void ina219::_setregister( uint8_t registernumber , uint16_t dataword ) {

    Wire.beginTransmission( _i2caddress );
    Wire.write( registernumber );
    Wire.write( (uint8_t)( ( dataword >> 8 ) & 0xFF ) );
    Wire.write( (uint8_t)( dataword & 0xFF ) );
    Wire.endTransmission();

}










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
    // pg = 00
    // badc = 0011
    // sadc = 0011
    // mode = 111

    uint16_t configdata = 0b0000000110011111;
    _setregister( INA219_REGISTER_CONFIGURATION , configdata );


    uint16_t calibration = 8192;
    _setregister( INA219_REGISTER_CALIBRATION , calibration );

}





uint16_t ina219::dumpregister( uint8_t registernumber ) {
    return _getregister( registernumber );
}



float ina219::getbusvoltage() {

    return ( ( ( _getregister( INA219_REGISTER_BUS) >> 3 ) * 4 ) * 0.001 );

};


float ina219::getshuntvoltage() {

    return ( _getregister( INA219_REGISTER_SHUNT ) * 0.01 );

}


float ina219::getcurrent() {

    return ( _getregister( INA219_REGISTER_CURRENT ) / 10 ) ;

}



float ina219::getpower() {
    return _getregister( INA219_REGISTER_POWER );
}











// confg options

void ina219::configsetbrng( uint8_t datavalue ) {

    // get the word
    uint16_t registerword = _getregister( INA219_REGISTER_CONFIGURATION );

    // modify the word
    registerword &= 0b1101111111111111;
    registerword |= ( ( datavalue & 0b1 ) << 13 );

    // set the word
    _setregister( INA219_REGISTER_CONFIGURATION , registerword );

}

uint8_t ina219::configgetbrng() {

    return ( ( _getregister( INA219_REGISTER_CONFIGURATION ) >> 13 ) & 0b1 );

}




void ina219::configsetpg( uint8_t datavalue ) {

    // get the word
    uint16_t registerword = _getregister( INA219_REGISTER_CONFIGURATION );

    // modify the word
    registerword &= 0b1110011111111111;
    registerword |= ( ( datavalue & 0b11 ) << 11 );

    // set the word
    _setregister( INA219_REGISTER_CONFIGURATION , registerword );

}

uint8_t ina219::configgetpg() {

    return ( ( _getregister( INA219_REGISTER_CONFIGURATION ) >> 11 ) & 0b11 );

}








void ina219::configsetbadc( uint8_t datavalue ) {

    // get the word
    uint16_t registerword = _getregister( INA219_REGISTER_CONFIGURATION );

    // modify the word
    registerword &= 0b1111100001111111;
    registerword |= ( ( datavalue & 0b1111 ) << 7 );

    // set the word
    _setregister( INA219_REGISTER_CONFIGURATION , registerword );

}

uint8_t ina219::configgetbadc() {

    return ( ( _getregister( INA219_REGISTER_CONFIGURATION ) >> 7 ) & 0b1111 );

}











void ina219::configsetsadc( uint8_t datavalue ) {

    // get the word
    uint16_t registerword = _getregister( INA219_REGISTER_CONFIGURATION );

    // modify the word
    registerword &= 0b1111111110000111;
    registerword |= ( ( datavalue & 0b1111 ) << 3 );

    // set the word
    _setregister( INA219_REGISTER_CONFIGURATION , registerword );

}

uint8_t ina219::configgetsadc() {

    return ( ( _getregister( INA219_REGISTER_CONFIGURATION ) >> 3 ) & 0b1111 );

}











void ina219::configsetmode( uint8_t datavalue ) {

    // get the word
    uint16_t registerword = _getregister( INA219_REGISTER_CONFIGURATION );

    // modify the word
    registerword &= 0b1111111111111000;
    registerword |= ( datavalue & 0b111 );

    // set the word
    _setregister( INA219_REGISTER_CONFIGURATION , registerword );

}

uint8_t ina219::configgetmode() {

    return ( _getregister( INA219_REGISTER_CONFIGURATION ) & 0b111 );

}




// calibration value
void ina219::configsetcalval( uint16_t datavalue ) {

    // just set the world and lets go
    _setregister( INA219_REGISTER_CALIBRATION , datavalue );

}



uint16_t ina219::configgetcalval() {

    // return the word
    return _getregister( INA219_REGISTER_CALIBRATION );
    
}









#endif


