

#ifndef INA219_HEADER_GUARD
#define INA219_HEADER_GUARD

#include <Arduino.h>

#include <Wire.h>




class ina219
{
    private:

        /// @brief the i2c address of this device
        uint8_t _i2caddress;

        /// @brief get a 16 bit register
        /// @param registernumber the number of register to get
        /// @return the value of the register
        uint16_t _getregister( uint8_t registernumber );

        /// @brief set a 16 bit register
        /// @param registernumber the number of register to set
        /// @param dataword the value to set to the register
        void _setregister( uint8_t registernumber , uint16_t dataword );

        
    public:

        ina219();
        ~ina219();


        void begin( uint8_t i2caddress );


        uint16_t dumpregister( uint8_t registernumber );


        float getbusvoltage();

        float getshuntvoltage();


};
























#endif


