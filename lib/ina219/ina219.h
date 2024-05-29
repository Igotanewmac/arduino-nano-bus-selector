

#ifndef INA219_HEADER_GUARD
#define INA219_HEADER_GUARD

#include <Arduino.h>

#include <Wire.h>




class ina219
{
    private:

        /// @brief the i2c address of this device.
        uint8_t _i2caddress;

        /// @brief get a 16 bit register.
        /// @param registernumber the number of register to get.
        /// @return the value of the register.
        uint16_t _getregister( uint8_t registernumber );

        /// @brief set a 16 bit register.
        /// @param registernumber the number of register to set.
        /// @param dataword the value to set to the register.
        void _setregister( uint8_t registernumber , uint16_t dataword );

        
    public:

        ina219();
        ~ina219();

        /// @brief Initialise the device to 16v 340ma.
        /// @param i2caddress the i2c address of the device.
        void begin( uint8_t i2caddress );



        /// @brief dump the raw contents of a register.
        /// @param registernumber the register number to dump.
        /// @return the 16 bit value of the register.
        uint16_t dumpregister( uint8_t registernumber );




        /// @brief Gets the bus voltage, in volts.
        /// @return The bus voltage, in volts.
        float getbusvoltage();

        /// @brief Gets the shunt voltage, in millivolts.
        /// @return The shut value, in millivolts.
        float getshuntvoltage();

        /// @brief Gets the current usage in milliamps.
        /// @return The current usage in milliamps.
        float getcurrent();

        /// @brief Gets the power usage in milliwatts.
        /// @return The power usage in milliwatts.
        float getpower();


        // config settings

        // BRNG
        void configsetbrng( uint8_t datavalue );
        uint8_t configgetbrng();

        // PG
        void configsetpg( uint8_t datavalue );
        uint8_t configgetpg();


        // BADC
        void configsetbadc( uint8_t datavalue );
        uint8_t configgetbadc();

        // SADC
        void configsetsadc( uint8_t datavalue );
        uint8_t configgetsadc();

        // MODE
        void configsetmode( uint8_t datavalue );
        uint8_t configgetmode();

        // calibration value
        void configsetcalval( uint16_t datavalue );
        uint16_t configgetcalval();

};
























#endif


