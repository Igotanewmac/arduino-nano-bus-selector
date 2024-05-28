

#ifndef BASETOOLS_BODY_GUARD
#define BASETOOLS_BODY_GUARD

#include <basetools.h>






void showhex( uint8_t hexvalue ) {
  Serial.print( hexvalue < 16 ? "0" : "" );
  Serial.print( hexvalue , HEX );
}


void showbin( uint8_t binvalue ) {
    Serial.print( binvalue & 128 ? 1 : 0 );
    Serial.print( binvalue & 64 ? 1 : 0 );
    Serial.print( binvalue & 32 ? 1 : 0 );
    Serial.print( binvalue & 16 ? 1 : 0 );
    Serial.print( binvalue & 8 ? 1 : 0 );
    Serial.print( binvalue & 4 ? 1 : 0 );
    Serial.print( binvalue & 2 ? 1 : 0 );
    Serial.print( binvalue & 1 ? 1 : 0 );
}





#endif