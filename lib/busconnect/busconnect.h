
#ifndef BUSCONNECT_HEADER_GUARD
#define BUSCONNECT_HEADER_GUARD

// Arduino platform interface.
#include <Arduino.h>

// Arduino platform i2c library.
#include <Wire.h>




// bus connector library


class busconnect {

    private:

        /// @brief the channel address storage array.
        uint8_t channeladdress[16] = {0};

        /// @brief fetch the current databyte from a pcf
        /// @param i2caddress the address of the device to fetch from
        /// @return the 8 bit databyte value
        uint8_t _getdatabyte( uint8_t i2caddress );

        /// @brief set a databyte to a pcf
        /// @param i2caddress the address of the device to set
        /// @param databyte the 8 bit value to set
        void _setdatabyte( uint8_t i2caddress , uint8_t databyte );

    public:

        /// @brief The constuctor for the object.
        busconnect(/* args */);

        /// @brief The deconstructor for the object.
        ~busconnect();

        /// @brief the initial setup.
        void begin();

        /// @brief Set a channels hardware addresses.
        /// @param channel The channel to set.
        /// @param i2c_address The i2c address of the PCF.
        /// @param byte_address The byte mask to use.  0 = low 0b00001111 , 1 = high 0b11110000.
        void setchanneladdresss( uint8_t channel , uint8_t i2c_address , uint8_t byte_address );

        /// @brief get a channels i2c address of the pcf controlling it
        /// @param channel the chanel to check
        /// @return the i2c address of the pcf controlling this channel
        uint8_t getchanneli2caddress( uint8_t channel );
        
        /// @brief get a channels byte address of the pcf controlling it
        /// @param channel the chanel to check
        /// @return the byte address of this channel. 0 = low 0b00001111 , 1 = high 0b11110000.
        uint8_t getchannelbyteaddress( uint8_t channel );
        
        /// @brief set the channels enabled state.
        /// @param channel the channel to set.
        /// @param requestedstate the state to set.  active low : 0 is on, 1 is off.
        void setchannelstate( uint8_t channel , uint8_t requestedstate );

        /// @brief gets the channels enabled state.
        /// @param channel the channel to check.
        /// @return the enabled state of the channel.  active low : 0 is on, 1 is off.
        uint8_t getchannelstate( uint8_t channel );


        /// @brief set a channels input selection.
        /// @param channel the channel to set.
        /// @param requestedinput the input to select. 0 to 7.
        void setchannelinput( uint8_t channel , uint8_t requestedinput );

        /// @brief gets the channels selected input.
        /// @param channel the channel to check.
        /// @return the currently selected input. 0 to 7.
        uint8_t getchannelinput( uint8_t channel );


};






#endif
