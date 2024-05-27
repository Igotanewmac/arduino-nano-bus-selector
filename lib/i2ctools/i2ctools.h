
#ifndef I2CTOOLS_HEADER_GUARD
#define I2CTOOLS_HEADER_GUARD

// Arduino platform interface.
#include <Arduino.h>

// Arduino platform i2c library.
#include <Wire.h>




// bus connector library


class i2ctools
{
    
    private:
        /* data */

    public:

        /// @brief The constructor for this object
        i2ctools(/* args */);

        /// @brief The deconstructor for this object
        ~i2ctools();

        /// @brief Set up i2c and serial
        /// @param requestedserialspeed the serial speed to use.  default 115200
        void begin( uint32_t requestedserialspeed = 115200 );

        /// @brief perform an i2c scan
        void i2cscan();



};






#endif
