

#ifndef MCP4725_HEADER_GUARD
#define MCP4725_HEADER_GUARD


#include <Arduino.h>
#include <Wire.h>





class mcp4725
{

    private:

        // the i2c address of this device
        uint8_t _i2caddress;

    public:

        mcp4725();
        ~mcp4725();

        // initialisor
        void begin( uint8_t i2caddress );



        uint16_t getval();
        void setval( uint16_t requestedvalue );


        uint16_t geteepromval();
        void seteepromval( uint16_t );


    };














#endif


