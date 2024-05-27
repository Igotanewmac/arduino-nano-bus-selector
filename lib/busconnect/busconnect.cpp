

#ifndef BUSCONNECT_BODY_GUARD
#define BUSCONNECT_BODY_GUARD

#include <busconnect.h>





// private methods




uint8_t busconnect::_getdatabyte( uint8_t i2caddress ) {

    Wire.requestFrom( (int)i2caddress , (int)1 );
    return Wire.read();

}



void busconnect::_setdatabyte( uint8_t i2caddress , uint8_t databyte ) {

    Wire.beginTransmission( i2caddress );
    Wire.write( databyte );
    Wire.endTransmission();

}





// public methods


busconnect::busconnect(/* args */) {
}

busconnect::~busconnect() {
}

void busconnect::begin() {
    // just double check that somebody turned on i2c...
    Wire.begin();
}




void busconnect::setchanneladdresss( uint8_t channel , uint8_t i2c_address , uint8_t byte_address ) {

    channeladdress[ channel ] = ( i2c_address & 0b01111111 ) | ( ( byte_address & 0b1 ) << 7 );

}



uint8_t busconnect::getchanneli2caddress( uint8_t channel ) {

    return ( channeladdress[ channel ] & 0b0111111 );

}



uint8_t busconnect::getchannelbyteaddress( uint8_t channel ) {

    return ( ( channeladdress[ channel ] >> 7 ) & 0b1 );

}


void busconnect::setchannelstate( uint8_t channel , uint8_t requestedstate ) {

    // fetch the current state data byte from the chip
    uint8_t statedata = _getdatabyte( ( channeladdress[ channel ] & 0b01111111 ) );
    
    // set the relevent state bit
    if ( ( channeladdress[ channel ] >> 7 ) ) {
        // high side flag is set, need bit 5
        // clear bit 5
        statedata &= 0b11101111;
        // set bit4 to requested state
        statedata |= ( ( requestedstate & 0b1 ) << 4 );
    }
    else {
        // low side flag is set, need bit 0
        // clear bit 0
        statedata &= 0b11111110;
        // set bit 0
        statedata |= ( requestedstate & 0b1 );
    }

    // now write this state to the chip
    _setdatabyte( ( channeladdress[ channel ] & 0b01111111 ) , statedata );

}



uint8_t busconnect::getchannelstate( uint8_t channel ) {

    // fetch the current state data byte from the chip
    uint8_t statedata = _getdatabyte( ( channeladdress[ channel ] & 0b01111111 ) );
    
    // return the right bit
    if ( channeladdress[ channel ] >> 7 ) {
        // return high bit
        return ( ( statedata >> 4 ) & 0b1 );
    }
    else {
        // return low bit
        return ( statedata & 0b1 );
    }

}




void busconnect::setchannelinput( uint8_t channel , uint8_t requestedinput ) {

    // fetch the current state data byte from the chip
    uint8_t statedata = _getdatabyte( ( channeladdress[ channel ] & 0b01111111 ) );
    
    // set the relevant bits
    if ( channeladdress[ channel ] >> 7 ) {
        // high bits
        // reset the bits
        statedata &= 0b00011111;
        // set the bits
        statedata |= ( ( requestedinput & 0b111 ) << 5 );
    }
    else {
        // low bits
        // reset the bits
        statedata &= 0b11110001;
        // set the bits
        statedata |= ( ( requestedinput & 0b111 ) << 1 );
    }

    // now write this state to the chip
    _setdatabyte( ( channeladdress[ channel ] & 0b01111111 ) , statedata );
    
}




uint8_t busconnect::getchannelinput( uint8_t channel ) {

    // fetch the current state data byte from the chip
    uint8_t statedata = _getdatabyte( ( channeladdress[ channel ] & 0b01111111 ) );
    
    // return the right bits
    if ( channeladdress[ channel ] >> 7 ) {
        // high bits
        return ( ( statedata >> 5 ) & 0b111 );
    }
    else {
        // low bits
        return ( ( statedata >> 1 ) & 0b111 );
    }

}




#endif