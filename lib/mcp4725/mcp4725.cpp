

#ifndef MCP4725_BODY_GUARD
#define MCP4725_BODY_GUARD

#include <mcp4725.h>









mcp4725::mcp4725(/* args */)
{
}

mcp4725::~mcp4725()
{
}



void mcp4725::begin( uint8_t i2caddress ) {
    _i2caddress = i2caddress;
}



uint16_t mcp4725::getval() {

    uint16_t returnvalue = 0;

    Wire.requestFrom( (uint8_t)_i2caddress , (uint8_t)4 );
    Wire.read(); // settings
    returnvalue |= Wire.read() << 8; // current value high byte
    returnvalue |= Wire.read(); // current value low byte
    Wire.read(); // eeprom data
    Wire.read(); // eeprom data

    return ( returnvalue >> 4 );

}


void mcp4725::setval( uint16_t requestedvalue ) {

    Wire.beginTransmission( _i2caddress );
    Wire.write( (uint8_t)( ( requestedvalue >> 8 ) & 0b00001111 ) );
    Wire.write( (uint8_t)requestedvalue );
    Wire.endTransmission();

}






uint16_t mcp4725::geteepromval() {

    uint16_t returnvalue = 0;
    Wire.requestFrom( (uint8_t)_i2caddress , (uint8_t)5 );
    Wire.read(); // config
    Wire.read(); // current value
    Wire.read(); // current value
    returnvalue |= Wire.read() << 8; // eeprom value
    returnvalue |= Wire.read(); // eeprom value

    return returnvalue;

};




void mcp4725::seteepromval( uint16_t requestedvalue ) {

    Wire.beginTransmission( _i2caddress );
    Wire.write( 0b01100000 );
    Wire.write( ( requestedvalue >> 4 ) & 0xFF );
    Wire.write( ( requestedvalue << 4 ) & 0xFF );
    Wire.endTransmission();

};


#endif


