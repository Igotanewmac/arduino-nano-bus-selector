
#include <Arduino.h>


// i2c tools
#include <i2ctools.h>

i2ctools myi2ctoolsobj;


#include <basetools.h>


// include busconnector library.
#include <busconnect.h>

// create a busconnector object.
busconnect mybusconnectobj;


// bus connector hardware addresses.
#define  BUSCONNECT_CHANNEL_0_I2C_ADDRESS   0x20
#define  BUSCONNECT_CHANNEL_0_BYTE_ADDRESS  0
#define  BUSCONNECT_CHANNEL_1_I2C_ADDRESS   0x20
#define  BUSCONNECT_CHANNEL_1_BYTE_ADDRESS  1
#define  BUSCONNECT_CHANNEL_2_I2C_ADDRESS   0x21
#define  BUSCONNECT_CHANNEL_2_BYTE_ADDRESS  0
#define  BUSCONNECT_CHANNEL_3_I2C_ADDRESS   0x21
#define  BUSCONNECT_CHANNEL_3_BYTE_ADDRESS  1

#define  BUSCONNECT_CHANNEL_4_I2C_ADDRESS   0x22
#define  BUSCONNECT_CHANNEL_4_BYTE_ADDRESS  0
#define  BUSCONNECT_CHANNEL_5_I2C_ADDRESS   0x22
#define  BUSCONNECT_CHANNEL_5_BYTE_ADDRESS  1
#define  BUSCONNECT_CHANNEL_6_I2C_ADDRESS   0x23
#define  BUSCONNECT_CHANNEL_6_BYTE_ADDRESS  0
#define  BUSCONNECT_CHANNEL_7_I2C_ADDRESS   0x23
#define  BUSCONNECT_CHANNEL_7_BYTE_ADDRESS  1

#define  BUSCONNECT_CHANNEL_8_I2C_ADDRESS   0x24
#define  BUSCONNECT_CHANNEL_8_BYTE_ADDRESS  0
#define  BUSCONNECT_CHANNEL_9_I2C_ADDRESS   0x24
#define  BUSCONNECT_CHANNEL_9_BYTE_ADDRESS  1
#define  BUSCONNECT_CHANNEL_10_I2C_ADDRESS  0x25
#define  BUSCONNECT_CHANNEL_10_BYTE_ADDRESS 0
#define  BUSCONNECT_CHANNEL_11_I2C_ADDRESS  0x25
#define  BUSCONNECT_CHANNEL_11_BYTE_ADDRESS 1

#define  BUSCONNECT_CHANNEL_12_I2C_ADDRESS  0x26
#define  BUSCONNECT_CHANNEL_12_BYTE_ADDRESS 0
#define  BUSCONNECT_CHANNEL_13_I2C_ADDRESS  0x26
#define  BUSCONNECT_CHANNEL_13_BYTE_ADDRESS 1
#define  BUSCONNECT_CHANNEL_14_I2C_ADDRESS  0x27
#define  BUSCONNECT_CHANNEL_14_BYTE_ADDRESS 0
#define  BUSCONNECT_CHANNEL_15_I2C_ADDRESS  0x27
#define  BUSCONNECT_CHANNEL_15_BYTE_ADDRESS 1




// ina219 library
#include <ina219.h>

// declare an array of 4 multimeters
ina219 multimeter[4];




// mcp4725 library
#include <mcp4725.h>

mcp4725 dacarray[2];




// tca9548 library
#include <tca9548.h>

tca9548 i2cbusmaster;







// new definitions
#define TCA9548_I2C_ADDRESS 0x70

#define PCF8574_I2C_ADDRESS_0 0x20






// command function declarations

String commandline = "";

void command_busconnect_parser( String &commandline );

void command_ina219_parser( String &commandline );

void command_mcp4725_parser( String &commandline );

void command_tca9548_parser( String &commandline );


void command_dump_registers();

void command_measure();




// test routines

void command_pinset_parser( String &commandline );


void command_7400_test( String &commandline );




void setup() {
  // put your setup code here, to run once:



  // i2c busmaster!
  i2cbusmaster.begin( 0x70 );
  i2cbusmaster.setbank(0);

  // configure the bus connector object.

  // call bigin to initialise the object.
  mybusconnectobj.begin();

  // set hardware addresses.
  mybusconnectobj.setchanneladdresss( 0  , BUSCONNECT_CHANNEL_0_I2C_ADDRESS , BUSCONNECT_CHANNEL_0_BYTE_ADDRESS );
  mybusconnectobj.setchanneladdresss( 1  , BUSCONNECT_CHANNEL_1_I2C_ADDRESS , BUSCONNECT_CHANNEL_1_BYTE_ADDRESS );
  mybusconnectobj.setchanneladdresss( 2  , BUSCONNECT_CHANNEL_2_I2C_ADDRESS , BUSCONNECT_CHANNEL_2_BYTE_ADDRESS );
  mybusconnectobj.setchanneladdresss( 3  , BUSCONNECT_CHANNEL_3_I2C_ADDRESS , BUSCONNECT_CHANNEL_3_BYTE_ADDRESS );
  mybusconnectobj.setchanneladdresss( 4  , BUSCONNECT_CHANNEL_4_I2C_ADDRESS , BUSCONNECT_CHANNEL_4_BYTE_ADDRESS );
  mybusconnectobj.setchanneladdresss( 5  , BUSCONNECT_CHANNEL_5_I2C_ADDRESS , BUSCONNECT_CHANNEL_5_BYTE_ADDRESS );
  mybusconnectobj.setchanneladdresss( 6  , BUSCONNECT_CHANNEL_6_I2C_ADDRESS , BUSCONNECT_CHANNEL_6_BYTE_ADDRESS );
  mybusconnectobj.setchanneladdresss( 7  , BUSCONNECT_CHANNEL_7_I2C_ADDRESS , BUSCONNECT_CHANNEL_7_BYTE_ADDRESS );
  mybusconnectobj.setchanneladdresss( 8  , BUSCONNECT_CHANNEL_8_I2C_ADDRESS , BUSCONNECT_CHANNEL_8_BYTE_ADDRESS );
  mybusconnectobj.setchanneladdresss( 9  , BUSCONNECT_CHANNEL_9_I2C_ADDRESS , BUSCONNECT_CHANNEL_9_BYTE_ADDRESS );
  mybusconnectobj.setchanneladdresss( 10 , BUSCONNECT_CHANNEL_10_I2C_ADDRESS , BUSCONNECT_CHANNEL_10_BYTE_ADDRESS );
  mybusconnectobj.setchanneladdresss( 11 , BUSCONNECT_CHANNEL_11_I2C_ADDRESS , BUSCONNECT_CHANNEL_11_BYTE_ADDRESS );
  mybusconnectobj.setchanneladdresss( 12 , BUSCONNECT_CHANNEL_12_I2C_ADDRESS , BUSCONNECT_CHANNEL_12_BYTE_ADDRESS );
  mybusconnectobj.setchanneladdresss( 13 , BUSCONNECT_CHANNEL_13_I2C_ADDRESS , BUSCONNECT_CHANNEL_13_BYTE_ADDRESS );
  mybusconnectobj.setchanneladdresss( 14 , BUSCONNECT_CHANNEL_14_I2C_ADDRESS , BUSCONNECT_CHANNEL_14_BYTE_ADDRESS );
  mybusconnectobj.setchanneladdresss( 15 , BUSCONNECT_CHANNEL_15_I2C_ADDRESS , BUSCONNECT_CHANNEL_15_BYTE_ADDRESS );
  
  // set everything to off.
  i2cbusmaster.setbank(2);
  mybusconnectobj.setchannelstate(  0 , 1 );
  mybusconnectobj.setchannelstate(  1 , 1 );
  mybusconnectobj.setchannelstate(  2 , 1 );
  mybusconnectobj.setchannelstate(  3 , 1 );
  mybusconnectobj.setchannelstate(  4 , 1 );
  mybusconnectobj.setchannelstate(  5 , 1 );
  mybusconnectobj.setchannelstate(  6 , 1 );
  mybusconnectobj.setchannelstate(  7 , 1 );
  i2cbusmaster.setbank(4);
  mybusconnectobj.setchannelstate(  8 , 1 );
  mybusconnectobj.setchannelstate(  9 , 1 );
  mybusconnectobj.setchannelstate( 10 , 1 );
  mybusconnectobj.setchannelstate( 11 , 1 );
  mybusconnectobj.setchannelstate( 12 , 1 );
  mybusconnectobj.setchannelstate( 13 , 1 );
  mybusconnectobj.setchannelstate( 14 , 1 );
  mybusconnectobj.setchannelstate( 15 , 1 );

  // set all busses to highest entry.
  // we default to top because pcf defaults to on
  // so at power on it will devault to 0b111 = 7
  i2cbusmaster.setbank(2);
  mybusconnectobj.setchannelinput(  0 , 7 );
  mybusconnectobj.setchannelinput(  1 , 7 );
  mybusconnectobj.setchannelinput(  2 , 7 );
  mybusconnectobj.setchannelinput(  3 , 7 );
  mybusconnectobj.setchannelinput(  4 , 7 );
  mybusconnectobj.setchannelinput(  5 , 7 );
  mybusconnectobj.setchannelinput(  6 , 7 );
  mybusconnectobj.setchannelinput(  7 , 7 );
  i2cbusmaster.setbank(4);
  mybusconnectobj.setchannelinput(  8 , 7 );
  mybusconnectobj.setchannelinput(  9 , 7 );
  mybusconnectobj.setchannelinput( 10 , 7 );
  mybusconnectobj.setchannelinput( 11 , 7 );
  mybusconnectobj.setchannelinput( 12 , 7 );
  mybusconnectobj.setchannelinput( 13 , 7 );
  mybusconnectobj.setchannelinput( 14 , 7 );
  mybusconnectobj.setchannelinput( 15 , 7 );
  
  // the bus is now in a known safe state.
  // all channels are disabled
  // all channels are on default bus line 7

  i2cbusmaster.setbank(1);

  // configure ina219's
  multimeter[0].begin( 0x40 );
  multimeter[1].begin( 0x41 );
  multimeter[2].begin( 0x44 );
  multimeter[3].begin( 0x45 );


  // turn on dacs
  dacarray[0].begin( 0x60 );
  dacarray[1].begin( 0x61 );







  // turn on i2ctools for i2c scanner
  myi2ctoolsobj.begin();



  // all done
  
}





// a utility function to make sure we only show the prompt once.
uint8_t serialpromptstate = 0;
void doprompt( uint8_t requestedstate ) {
  if ( requestedstate ) {
    if ( !serialpromptstate ) {
      serialpromptstate = 1;
      Serial.print( "Ready: " );
    }
  }
  else {
    serialpromptstate = 0;
  }
}






void loop() {
  // put your main code here, to run repeatedly:

  // do prompt if required
  doprompt(1);

  // await serial input.
  if (!Serial.available()) {
    return;
  }

  // reset prompt flag
  doprompt(0);

  // get first line
  commandline = Serial.readStringUntil( 0x0A );

  // discard the rest
  while ( Serial.available() ) { Serial.read(); }

  // repeat it
  Serial.print("<<: " );
  Serial.println( commandline );

  // test command to check if we are connected
  if ( commandline.startsWith( "test" ) ) { Serial.println("Testing 123" ); return; }

  // enumerate the i2c bus.
  if ( commandline.startsWith( "i2cscan" ) ) { myi2ctoolsobj.i2cscan(); return; }


  // busconnect command parser
  if ( commandline.startsWith( "bc" ) ) { command_busconnect_parser( commandline ); return; }

  // ina219 command parser
  if ( commandline.startsWith( "ina" ) ) { command_ina219_parser( commandline ); return; }

  if ( commandline.startsWith( "mcp" ) ) { command_mcp4725_parser( commandline ); return; }


  if ( commandline.startsWith( "tca" ) ) { command_tca9548_parser( commandline ); return; }



  // dump registers
  if ( commandline.startsWith( "dr" ) ) { command_dump_registers(); return; }

  // quick measure
  if ( commandline.startsWith( "mr" ) ) { command_measure(); return; }



  if ( commandline.startsWith("ps" ) ) { command_pinset_parser( commandline ); return; }



  // 7400 parser
  if ( commandline.startsWith( "7400" ) ) { command_7400_test( commandline ); return; }





  // done command processing
  Serial.println("Command not recognised.");
  
}








void command_tca9548_parser( String &commandline ) {
  commandline = commandline.substring( 4 );

  if ( commandline.startsWith( "get" ) ) {
    Serial.print( "I2cbus " );
    Serial.print( i2cbusmaster.getbank() );
    Serial.print( " : " );
    showbin( i2cbusmaster.getbank() );
    Serial.println( " is selected!" );
    return;
  }

  if ( commandline.startsWith( "set" ) ) {
    commandline = commandline.substring( 4 );
    uint8_t bankrequested = commandline.toInt();
    i2cbusmaster.setbank( bankrequested );
    Serial.print( "I2cbus " );
    Serial.print( bankrequested );
    Serial.print( " " );
    showbin( bankrequested );
    Serial.println( " has been set" );
    return;
  }

  Serial.println( "Usage: " );
  Serial.println( "tca set N");
  Serial.println( "Sets the i2c bus number" );
  Serial.println( "tca get" );
  Serial.println( "Gets the current i2c bus" );

}

void command_busconnect_parser( String &commandline ) {

  // clean up the input string
  commandline = commandline.substring( 3 );
  commandline.trim();
  commandline.toLowerCase();

  // handle empty string
  if (!commandline.length()) { Serial.println("busconnect loaded ok!" ); return; }


  // actually do stuff
  if ( commandline.startsWith("get") ) {
    // show the current bus setup
    Serial.println( "Chan\ti2c\tByte\tBus\tState" );

    for ( uint8_t i = 0 ; i < 16 ; i++ ) {
      
      showhex( i ); Serial.print("\t");
      showhex( mybusconnectobj.getchanneli2caddress( i ) ); Serial.print("\t");
      showhex( mybusconnectobj.getchannelbyteaddress( i ) ); Serial.print("\t");
      showhex( mybusconnectobj.getchannelinput( i ) ); Serial.print("\t");
      showhex( mybusconnectobj.getchannelstate( i ) ); Serial.println(); 
    }

    return;

  }



  if ( commandline.startsWith("state") ) {
    commandline = commandline.substring(6);
    uint8_t requestedchannel = commandline.toInt();
    commandline = commandline.substring( commandline.indexOf(" ") + 1 );
    uint8_t requestedstate = commandline.toInt();
    
    Serial.print("Setting channel " );
    Serial.print( requestedchannel );
    Serial.print( " to state " );
    Serial.print( requestedstate );
    Serial.println(".");

    mybusconnectobj.setchannelstate( requestedchannel , requestedstate );

    Serial.println("Done!");

    return;

  }

  if ( commandline.startsWith("input") ) {
    commandline = commandline.substring(6);
    uint8_t requestedchannel = commandline.toInt();
    commandline = commandline.substring( commandline.indexOf(" ") + 1 );
    uint8_t requestedinput = commandline.toInt();
    
    Serial.print("Setting channel " );
    Serial.print( requestedchannel );
    Serial.print( " to input " );
    Serial.print( requestedinput );
    Serial.println(".");

    mybusconnectobj.setchannelinput( requestedchannel , requestedinput );

    Serial.println("Done!");

    return;

  }


  Serial.println("Busconnect usage");
  Serial.println("");
  Serial.println("Usage:");
  Serial.println("bc get");
  Serial.println("show the busconnect table");
  Serial.println("bc input X N");
  Serial.println("set channel X to input N");
  Serial.println("bc state X N");
  Serial.println("set channel X to state N");
  Serial.println("");
  
  return;
}

void command_ina219_parser( String &commandline ) {

  // clean up the input string
  commandline = commandline.substring( 7 );
  commandline.trim();
  commandline.toLowerCase();

  // handle empty string
  if (!commandline.length()) { Serial.println("ina219 is awesome!" ); return; }

  // get the device index
  uint8_t deviceindex = commandline.toInt();
  commandline = commandline.substring( commandline.indexOf(" ") + 1 );


  // config options
  if ( commandline.startsWith("config") ) {
    commandline = commandline.substring( 7 );


    // getters
    if ( commandline.startsWith("get") ) {
      commandline = commandline.substring(4);
      
      // brng getter
      if ( commandline.startsWith("brng") ) {
          Serial.print("Device " );
          Serial.print( deviceindex );
          Serial.print( " has BRNG set to ");
          Serial.println( multimeter[deviceindex].configgetbrng() );
      }

      // pg getter
      if ( commandline.startsWith("pg") ) {
          Serial.print("Device " );
          Serial.print( deviceindex );
          Serial.print( " has PG set to ");
          Serial.println( multimeter[deviceindex].configgetpg() );
      }

      // badc getter
      if ( commandline.startsWith("badc") ) {
          Serial.print("Device " );
          Serial.print( deviceindex );
          Serial.print( " has BADC set to ");
          Serial.println( multimeter[deviceindex].configgetbadc() );
      }

      // sadc getter
      if ( commandline.startsWith("sadc") ) {
          Serial.print("Device " );
          Serial.print( deviceindex );
          Serial.print( " has SADC set to ");
          Serial.println( multimeter[deviceindex].configgetsadc() );
      }

      // mode getter
      if ( commandline.startsWith("mode") ) {
          Serial.print("Device " );
          Serial.print( deviceindex );
          Serial.print( " has MODE set to ");
          Serial.println( multimeter[deviceindex].configgetmode() );
      }


      // calibration getter
      if ( commandline.startsWith("cal") ) {
          Serial.print("Device " );
          Serial.print( deviceindex );
          Serial.print( " has CAL set to ");
          Serial.println( multimeter[deviceindex].configgetmode() );
      }



      // end of getters
    }


    // setters
    if ( commandline.startsWith("set") ) {
      commandline = commandline.substring(4);
      
      // brng setter
      if ( commandline.startsWith("brng") ) {
        commandline = commandline.substring( 5 );
        uint8_t requestedvalue = (uint8_t)( commandline.toInt() & 0b1 );
        Serial.print( "Setting device " );
        Serial.print( deviceindex );
        Serial.print( " to " );
        Serial.println( requestedvalue );
        multimeter[deviceindex].configsetbrng( requestedvalue );
      }

      // pg setter
      if ( commandline.startsWith("pg") ) {
        commandline = commandline.substring( 3 );
        uint8_t requestedvalue = (uint8_t)( commandline.toInt() & 0b11 );
        Serial.print( "Setting device " );
        Serial.print( deviceindex );
        Serial.print( " to " );
        Serial.println( requestedvalue );
        multimeter[deviceindex].configsetpg( requestedvalue );
      }
      
      // pg setter
      if ( commandline.startsWith("badc") ) {
        commandline = commandline.substring( 5 );
        uint8_t requestedvalue = (uint8_t)( commandline.toInt() & 0b1111 );
        Serial.print( "Setting device " );
        Serial.print( deviceindex );
        Serial.print( " to " );
        Serial.println( requestedvalue );
        multimeter[deviceindex].configsetbadc( requestedvalue );
      }


      // mode setter
      if ( commandline.startsWith("mode") ) {
        commandline = commandline.substring( 5 );
        uint8_t requestedvalue = (uint8_t)( commandline.toInt() & 0b111 );
        Serial.print( "Setting device " );
        Serial.print( deviceindex );
        Serial.print( " to " );
        Serial.println( requestedvalue );
        multimeter[deviceindex].configsetmode( requestedvalue );
      }


      // mode setter
      if ( commandline.startsWith("cal") ) {
        commandline = commandline.substring( 4 );
        uint16_t requestedvalue = commandline.toInt();
        Serial.print( "Setting device " );
        Serial.print( deviceindex );
        Serial.print( " to " );
        Serial.println( requestedvalue );
        multimeter[deviceindex].configsetmode( requestedvalue );
      }



      // end of setters
    }

    // end of config options
  }


  // if no argument is given, measure
  Serial.println( "ID\tBus\tShunt\tCurrent\tPower" );
  Serial.print( deviceindex ); Serial.print( "\t" );
  Serial.print( multimeter[deviceindex].getbusvoltage() ); Serial.print("\t" );
  Serial.print( multimeter[deviceindex].getshuntvoltage() ); Serial.print("\t" );
  Serial.print( multimeter[deviceindex].getcurrent() ); Serial.print("\t" );
  Serial.print( multimeter[deviceindex].getpower() ); Serial.println();




}

void command_mcp4725_parser( String &commandline ) {

  // clean up the input string
  commandline = commandline.substring( 7 );
  commandline.trim();
  commandline.toLowerCase();

  // handle empty string
  if (!commandline.length()) { Serial.println("ina219 is awesome!" ); return; }

  // get the device index
  uint8_t deviceindex = commandline.toInt();
  commandline = commandline.substring( commandline.indexOf(" ") + 1 );



  // is this comand a getter or setter?
  if ( commandline.startsWith( "get" ) ) {
    commandline = commandline.substring( 4 );
    
    // operating value
    if ( commandline.startsWith("val") ) {
      uint16_t returnvalue = dacarray[deviceindex].getval();
      Serial.print("Actual: ");
      showhex( returnvalue >> 8 );
      showhex( ( uint8_t)returnvalue );
      Serial.print(" : ");
      showbin( returnvalue >> 8 );
      showbin( ( uint8_t)returnvalue );
      Serial.print(" : " );
      Serial.print( returnvalue );
      Serial.println();
      return;
    }


    // operating value
    if ( commandline.startsWith("eeprom") ) {
      uint16_t returnvalue = dacarray[deviceindex].geteepromval();
      Serial.print("Actual: ");
      showhex( returnvalue >> 8 );
      showhex( ( uint8_t)returnvalue );
      Serial.print(" : ");
      showbin( returnvalue >> 8 );
      showbin( ( uint8_t)returnvalue );
      Serial.print(" : " );
      Serial.print( returnvalue );
      Serial.println();
      return;
    }


  




  }

  // setters
  if ( commandline.startsWith("set" ) ) {
    commandline = commandline.substring( 4 );
    
    if ( commandline.startsWith("val") ) {
      commandline = commandline.substring( 4 );

      uint16_t requestedvalue = commandline.toInt();

      Serial.print("Set device " );
      Serial.print( deviceindex );
      Serial.print( " is set to " );
      Serial.println( requestedvalue );
      
      dacarray[deviceindex].setval( requestedvalue );

    }



    if ( commandline.startsWith("eeprom") ) {
      commandline = commandline.substring( 7 );

      uint16_t requestedvalue = commandline.toInt();

      Serial.print("Set device " );
      Serial.print( deviceindex );
      Serial.print( " is set to " );
      Serial.println( requestedvalue );
      
      dacarray[deviceindex].seteepromval( requestedvalue );

    }



  }






}

void command_dump_registers() {

  uint16_t registervalue = 0x0000;

  for ( uint8_t i = 0 ; i < 6 ; i++ ) {
    registervalue = multimeter[0].dumpregister(i);
    Serial.print(i); Serial.print(" : " );
    showhex( registervalue >> 8 );
    showhex( registervalue );
    Serial.print(" : " );
    showbin( registervalue >> 8 );
    showbin( registervalue );
    Serial.print(" : ");
    Serial.print( registervalue );
    Serial.println();
  }

}

void command_measure() {

  Serial.println( "ID\tBus\tShunt\tCurrent\tPower" );

  for ( uint8_t i = 0 ; i < 4 ; i++ ) {

    Serial.print( i ); Serial.print( "\t" );
    Serial.print( multimeter[i].getbusvoltage() ); Serial.print("\t" );
    Serial.print( multimeter[i].getshuntvoltage() ); Serial.print("\t" );
    Serial.print( multimeter[i].getcurrent() ); Serial.print("\t" );
    Serial.print( multimeter[i].getpower() ); Serial.println();
    
  }

}





// bus lanes
// 00 GND
// 01 VCC
// 02 DAC1
// 03 DAC2
// 04 Metered Current Sink
// 05 Metered VCC



// command: ps PIN BUS STATE

void command_pinset_parser( String &commandline ) {

  commandline = commandline.substring( 3 );
  commandline.trim();
  commandline.toLowerCase();

  if ( commandline.equals("") ) {
    
    Serial.println("pinselect usage");
    Serial.println("");
    Serial.println("ps X Y Z");
    Serial.println("X is pin to set");
    Serial.println("Y is input bus to select");
    Serial.println("Z is state to set");
    Serial.println("");
    return;
  }


  Serial.print( "Pinset request: " );
  Serial.println( commandline );

  uint8_t requestedpin = commandline.toInt();
  commandline = commandline.substring( commandline.indexOf(" ") + 1 );

  uint8_t requestedinput = commandline.toInt();
  commandline = commandline.substring( commandline.indexOf(" ") + 1 );

  uint8_t requestedstate = commandline.toInt();

  Serial.print( "Setting pin " );
  Serial.print( requestedpin );
  Serial.print( " to bus " );
  Serial.print( requestedinput );
  Serial.print( " with state " );
  Serial.print( requestedstate );
  Serial.println();


  switch ( requestedpin ) {
  
    case 1: {
      // pin 1
      // bus 2
      // pcf 0
      // nibble 0

      // switch to the right bus
      Wire.beginTransmission( TCA9548_I2C_ADDRESS );
      Wire.write( 0b00000010 );
      Wire.endTransmission();
      
      // get the current state from the pcf
      Wire.requestFrom( PCF8574_I2C_ADDRESS_0 , 1 );
      uint8_t databyte = Wire.read();

      Serial.print(" rcv: " );
      showbin( databyte );
      Serial.println();

      // we are on the high bit, so set that now
      databyte &= 0b11110000;
      databyte |= ( requestedstate & 0b1 );
      databyte |= ( ( requestedinput & 0b111 ) << 1 );

      Serial.print( "snd: " );
      showbin( databyte );
      Serial.println();

      Wire.beginTransmission( PCF8574_I2C_ADDRESS_0 );
      Wire.write( databyte );
      Wire.endTransmission();

      break;
    }



    case 2: {
      // pin 2
      // bus 2
      // pcf 0
      // nibble 1

      // switch to the right bus
      Wire.beginTransmission( TCA9548_I2C_ADDRESS );
      Wire.write( 0b00000010 );
      Wire.endTransmission();
      
      // get the current state from the pcf
      Wire.requestFrom( PCF8574_I2C_ADDRESS_0 , 1 );
      uint8_t databyte = Wire.read();

      Serial.print(" rcv: " );
      showbin( databyte );
      Serial.println();

      // we are on the high bit, so set that now
      databyte &= 0b00001111;
      databyte |= ( ( requestedstate & 0b1 ) << 4);
      databyte |= ( ( requestedinput & 0b111 ) << 5 );

      Serial.print( "snd: " );
      showbin( databyte );
      Serial.println();

      Wire.beginTransmission( PCF8574_I2C_ADDRESS_0 );
      Wire.write( databyte );
      Wire.endTransmission();

      break;
    }





    case 3: {
      // pin 1
      // bus 2
      // pcf 0
      // nibble 0

      // switch to the right bus
      Wire.beginTransmission( TCA9548_I2C_ADDRESS );
      Wire.write( 0b00000010 );
      Wire.endTransmission();
      
      // get the current state from the pcf
      Wire.requestFrom( PCF8574_I2C_ADDRESS_0 , 1 );
      uint8_t databyte = Wire.read();

      Serial.print(" rcv: " );
      showbin( databyte );
      Serial.println();

      // we are on the high bit, so set that now
      databyte &= 0b11110000;
      databyte |= ( requestedstate & 0b1 );
      databyte |= ( ( requestedinput & 0b111 ) << 1 );

      Serial.print( "snd: " );
      showbin( databyte );
      Serial.println();

      Wire.beginTransmission( PCF8574_I2C_ADDRESS_0 );
      Wire.write( databyte );
      Wire.endTransmission();

      break;
    }



    case 4: {
      // pin 2
      // bus 2
      // pcf 0
      // nibble 1

      // switch to the right bus
      Wire.beginTransmission( TCA9548_I2C_ADDRESS );
      Wire.write( 0b00000010 );
      Wire.endTransmission();
      
      // get the current state from the pcf
      Wire.requestFrom( PCF8574_I2C_ADDRESS_0 , 1 );
      uint8_t databyte = Wire.read();

      Serial.print(" rcv: " );
      showbin( databyte );
      Serial.println();

      // we are on the high bit, so set that now
      databyte &= 0b00001111;
      databyte |= ( ( requestedstate & 0b1 ) << 4);
      databyte |= ( ( requestedinput & 0b111 ) << 5 );

      Serial.print( "snd: " );
      showbin( databyte );
      Serial.println();

      Wire.beginTransmission( PCF8574_I2C_ADDRESS_0 );
      Wire.write( databyte );
      Wire.endTransmission();

      break;
    }


    case 5: {
      // pin 1
      // bus 2
      // pcf 0
      // nibble 0

      // switch to the right bus
      Wire.beginTransmission( TCA9548_I2C_ADDRESS );
      Wire.write( 0b00000010 );
      Wire.endTransmission();
      
      // get the current state from the pcf
      Wire.requestFrom( PCF8574_I2C_ADDRESS_0 , 1 );
      uint8_t databyte = Wire.read();

      Serial.print(" rcv: " );
      showbin( databyte );
      Serial.println();

      // we are on the high bit, so set that now
      databyte &= 0b11110000;
      databyte |= ( requestedstate & 0b1 );
      databyte |= ( ( requestedinput & 0b111 ) << 1 );

      Serial.print( "snd: " );
      showbin( databyte );
      Serial.println();

      Wire.beginTransmission( PCF8574_I2C_ADDRESS_0 );
      Wire.write( databyte );
      Wire.endTransmission();

      break;
    }



    case 6: {
      // pin 2
      // bus 2
      // pcf 0
      // nibble 1

      // switch to the right bus
      Wire.beginTransmission( TCA9548_I2C_ADDRESS );
      Wire.write( 0b00000010 );
      Wire.endTransmission();
      
      // get the current state from the pcf
      Wire.requestFrom( PCF8574_I2C_ADDRESS_0 , 1 );
      uint8_t databyte = Wire.read();

      Serial.print(" rcv: " );
      showbin( databyte );
      Serial.println();

      // we are on the high bit, so set that now
      databyte &= 0b00001111;
      databyte |= ( ( requestedstate & 0b1 ) << 4);
      databyte |= ( ( requestedinput & 0b111 ) << 5 );

      Serial.print( "snd: " );
      showbin( databyte );
      Serial.println();

      Wire.beginTransmission( PCF8574_I2C_ADDRESS_0 );
      Wire.write( databyte );
      Wire.endTransmission();

      break;
    }




    case 7: {
      // pin 1
      // bus 2
      // pcf 0
      // nibble 0

      // switch to the right bus
      Wire.beginTransmission( TCA9548_I2C_ADDRESS );
      Wire.write( 0b00000010 );
      Wire.endTransmission();
      
      // get the current state from the pcf
      Wire.requestFrom( PCF8574_I2C_ADDRESS_0 , 1 );
      uint8_t databyte = Wire.read();

      Serial.print(" rcv: " );
      showbin( databyte );
      Serial.println();

      // we are on the high bit, so set that now
      databyte &= 0b11110000;
      databyte |= ( requestedstate & 0b1 );
      databyte |= ( ( requestedinput & 0b111 ) << 1 );

      Serial.print( "snd: " );
      showbin( databyte );
      Serial.println();

      Wire.beginTransmission( PCF8574_I2C_ADDRESS_0 );
      Wire.write( databyte );
      Wire.endTransmission();

      break;
    }



    case 8: {
      // pin 2
      // bus 2
      // pcf 0
      // nibble 1

      // switch to the right bus
      Wire.beginTransmission( TCA9548_I2C_ADDRESS );
      Wire.write( 0b00000010 );
      Wire.endTransmission();
      
      // get the current state from the pcf
      Wire.requestFrom( PCF8574_I2C_ADDRESS_0 , 1 );
      uint8_t databyte = Wire.read();

      Serial.print(" rcv: " );
      showbin( databyte );
      Serial.println();

      // we are on the high bit, so set that now
      databyte &= 0b00001111;
      databyte |= ( ( requestedstate & 0b1 ) << 4);
      databyte |= ( ( requestedinput & 0b111 ) << 5 );

      Serial.print( "snd: " );
      showbin( databyte );
      Serial.println();

      Wire.beginTransmission( PCF8574_I2C_ADDRESS_0 );
      Wire.write( databyte );
      Wire.endTransmission();

      break;
    }






    case 9: {
      // pin 1
      // bus 2
      // pcf 0
      // nibble 0

      // switch to the right bus
      Wire.beginTransmission( TCA9548_I2C_ADDRESS );
      Wire.write( 0b00000010 );
      Wire.endTransmission();
      
      // get the current state from the pcf
      Wire.requestFrom( PCF8574_I2C_ADDRESS_0 , 1 );
      uint8_t databyte = Wire.read();

      Serial.print(" rcv: " );
      showbin( databyte );
      Serial.println();

      // we are on the high bit, so set that now
      databyte &= 0b11110000;
      databyte |= ( requestedstate & 0b1 );
      databyte |= ( ( requestedinput & 0b111 ) << 1 );

      Serial.print( "snd: " );
      showbin( databyte );
      Serial.println();

      Wire.beginTransmission( PCF8574_I2C_ADDRESS_0 );
      Wire.write( databyte );
      Wire.endTransmission();

      break;
    }



    case 10: {
      // pin 2
      // bus 2
      // pcf 0
      // nibble 1

      // switch to the right bus
      Wire.beginTransmission( TCA9548_I2C_ADDRESS );
      Wire.write( 0b00000010 );
      Wire.endTransmission();
      
      // get the current state from the pcf
      Wire.requestFrom( PCF8574_I2C_ADDRESS_0 , 1 );
      uint8_t databyte = Wire.read();

      Serial.print(" rcv: " );
      showbin( databyte );
      Serial.println();

      // we are on the high bit, so set that now
      databyte &= 0b00001111;
      databyte |= ( ( requestedstate & 0b1 ) << 4);
      databyte |= ( ( requestedinput & 0b111 ) << 5 );

      Serial.print( "snd: " );
      showbin( databyte );
      Serial.println();

      Wire.beginTransmission( PCF8574_I2C_ADDRESS_0 );
      Wire.write( databyte );
      Wire.endTransmission();

      break;
    }





    case 11: {
      // pin 1
      // bus 2
      // pcf 0
      // nibble 0

      // switch to the right bus
      Wire.beginTransmission( TCA9548_I2C_ADDRESS );
      Wire.write( 0b00000010 );
      Wire.endTransmission();
      
      // get the current state from the pcf
      Wire.requestFrom( PCF8574_I2C_ADDRESS_0 , 1 );
      uint8_t databyte = Wire.read();

      Serial.print(" rcv: " );
      showbin( databyte );
      Serial.println();

      // we are on the high bit, so set that now
      databyte &= 0b11110000;
      databyte |= ( requestedstate & 0b1 );
      databyte |= ( ( requestedinput & 0b111 ) << 1 );

      Serial.print( "snd: " );
      showbin( databyte );
      Serial.println();

      Wire.beginTransmission( PCF8574_I2C_ADDRESS_0 );
      Wire.write( databyte );
      Wire.endTransmission();

      break;
    }



    case 12: {
      // pin 2
      // bus 2
      // pcf 0
      // nibble 1

      // switch to the right bus
      Wire.beginTransmission( TCA9548_I2C_ADDRESS );
      Wire.write( 0b00000010 );
      Wire.endTransmission();
      
      // get the current state from the pcf
      Wire.requestFrom( PCF8574_I2C_ADDRESS_0 , 1 );
      uint8_t databyte = Wire.read();

      Serial.print(" rcv: " );
      showbin( databyte );
      Serial.println();

      // we are on the high bit, so set that now
      databyte &= 0b00001111;
      databyte |= ( ( requestedstate & 0b1 ) << 4);
      databyte |= ( ( requestedinput & 0b111 ) << 5 );

      Serial.print( "snd: " );
      showbin( databyte );
      Serial.println();

      Wire.beginTransmission( PCF8574_I2C_ADDRESS_0 );
      Wire.write( databyte );
      Wire.endTransmission();

      break;
    }





    case 13: {
      // pin 1
      // bus 2
      // pcf 0
      // nibble 0

      // switch to the right bus
      Wire.beginTransmission( TCA9548_I2C_ADDRESS );
      Wire.write( 0b00000010 );
      Wire.endTransmission();
      
      // get the current state from the pcf
      Wire.requestFrom( PCF8574_I2C_ADDRESS_0 , 1 );
      uint8_t databyte = Wire.read();

      Serial.print(" rcv: " );
      showbin( databyte );
      Serial.println();

      // we are on the high bit, so set that now
      databyte &= 0b11110000;
      databyte |= ( requestedstate & 0b1 );
      databyte |= ( ( requestedinput & 0b111 ) << 1 );

      Serial.print( "snd: " );
      showbin( databyte );
      Serial.println();

      Wire.beginTransmission( PCF8574_I2C_ADDRESS_0 );
      Wire.write( databyte );
      Wire.endTransmission();

      break;
    }



    case 14: {
      // pin 2
      // bus 2
      // pcf 0
      // nibble 1

      // switch to the right bus
      Wire.beginTransmission( TCA9548_I2C_ADDRESS );
      Wire.write( 0b00000010 );
      Wire.endTransmission();
      
      // get the current state from the pcf
      Wire.requestFrom( PCF8574_I2C_ADDRESS_0 , 1 );
      uint8_t databyte = Wire.read();

      Serial.print(" rcv: " );
      showbin( databyte );
      Serial.println();

      // we are on the high bit, so set that now
      databyte &= 0b00001111;
      databyte |= ( ( requestedstate & 0b1 ) << 4);
      databyte |= ( ( requestedinput & 0b111 ) << 5 );

      Serial.print( "snd: " );
      showbin( databyte );
      Serial.println();

      Wire.beginTransmission( PCF8574_I2C_ADDRESS_0 );
      Wire.write( databyte );
      Wire.endTransmission();

      break;
    }





    case 15: {
      // pin 1
      // bus 2
      // pcf 0
      // nibble 0

      // switch to the right bus
      Wire.beginTransmission( TCA9548_I2C_ADDRESS );
      Wire.write( 0b00000010 );
      Wire.endTransmission();
      
      // get the current state from the pcf
      Wire.requestFrom( PCF8574_I2C_ADDRESS_0 , 1 );
      uint8_t databyte = Wire.read();

      Serial.print(" rcv: " );
      showbin( databyte );
      Serial.println();

      // we are on the high bit, so set that now
      databyte &= 0b11110000;
      databyte |= ( requestedstate & 0b1 );
      databyte |= ( ( requestedinput & 0b111 ) << 1 );

      Serial.print( "snd: " );
      showbin( databyte );
      Serial.println();

      Wire.beginTransmission( PCF8574_I2C_ADDRESS_0 );
      Wire.write( databyte );
      Wire.endTransmission();

      break;
    }



    case 16: {
      // pin 2
      // bus 2
      // pcf 0
      // nibble 1

      // switch to the right bus
      Wire.beginTransmission( TCA9548_I2C_ADDRESS );
      Wire.write( 0b00000010 );
      Wire.endTransmission();
      
      // get the current state from the pcf
      Wire.requestFrom( PCF8574_I2C_ADDRESS_0 , 1 );
      uint8_t databyte = Wire.read();

      Serial.print(" rcv: " );
      showbin( databyte );
      Serial.println();

      // we are on the high bit, so set that now
      databyte &= 0b00001111;
      databyte |= ( ( requestedstate & 0b1 ) << 4);
      databyte |= ( ( requestedinput & 0b111 ) << 5 );

      Serial.print( "snd: " );
      showbin( databyte );
      Serial.println();

      Wire.beginTransmission( PCF8574_I2C_ADDRESS_0 );
      Wire.write( databyte );
      Wire.endTransmission();

      break;
    }









    default: {
      Serial.println( "No change made! (switch case fallthrough!)");
      break;
    }


  }




}









void command_7400_test( String &commandline ) {
  
  commandline = commandline.substring( 5 );

  if ( commandline.equals("") ) { commandline = "show"; }

  if ( commandline.startsWith("show") ) {

    Serial.println("7400 Series Logic.");
    Serial.println("");
    Serial.println("Quad 2 input NAND gates.");
    Serial.println("");
    Serial.println("      +----u----+     ");
    Serial.println("  1A -| 1    14 |- VCC");
    Serial.println("  1B -| 2    13 |- 4B ");
    Serial.println("  1Y -| 3    12 |- 4A ");
    Serial.println("  2A -| 4    11 |- 4Y ");
    Serial.println("  2B -| 5    10 |- 3B ");
    Serial.println("  2Y -| 6    9  |- 3A ");
    Serial.println(" GND -| 7    8  |- 3Y ");
    Serial.println("      +---------+     ");
    Serial.println("");
    
  }

  if ( commandline.startsWith("test") ) {

      Serial.println("7400 Series Logic.");
      Serial.println("");
      Serial.println("Quad 2 input NAND gates.");
      Serial.println("");


    // configure all the pins
    // DAC1 to 1  1A
    // DAC2 to 2  1B
    // MGND to 2  1Y
    //  VCC to 4  2A
    //  VCC to 5  2B
    //  GND to 6  2Y
    //  GND to 7  GND
    //  GND to 8  3Y
    //  VCC to 9  3A
    //  VCC to 10 3B
    //  GND to 11 4Y
    //  VCC to 12 4A
    //  VCC to 13 4B
    // MVCC to 14 VCC

    // switch bank to first bus
    i2cbusmaster.setbank(2);

    // configure all the pins
    // DAC1 to 1  1A
    mybusconnectobj.setchannelinput( 0 , 2 );
    mybusconnectobj.setchannelstate( 0 , 0 );

    // DAC2 to 2  1B
    mybusconnectobj.setchannelinput( 1 , 3 );
    mybusconnectobj.setchannelstate( 1 , 0 );

    // MGND to 2  1Y
    mybusconnectobj.setchannelinput( 2 , 4 );
    mybusconnectobj.setchannelstate( 2 , 0 );

    //  VCC to 4  2A
    mybusconnectobj.setchannelinput( 3 , 1 );
    mybusconnectobj.setchannelstate( 3 , 0 );

    //  VCC to 5  2B
    mybusconnectobj.setchannelinput( 4 , 1 );
    mybusconnectobj.setchannelstate( 4 , 0 );

    //  GND to 6  2Y
    mybusconnectobj.setchannelinput( 5 , 0 );
    mybusconnectobj.setchannelstate( 5 , 0 );

    //  GND to 7  GND
    mybusconnectobj.setchannelinput( 6 , 0 );
    mybusconnectobj.setchannelstate( 6 , 0 );

    // skip 7

    // skip 8

    i2cbusmaster.setbank(4);

    //  GND to 8  3Y
    mybusconnectobj.setchannelinput( 9 , 0 );
    mybusconnectobj.setchannelstate( 9 , 0 );
    
    //  VCC to 9  3A
    mybusconnectobj.setchannelinput( 10 , 1 );
    mybusconnectobj.setchannelstate( 10 , 0 );
    
    //  VCC to 10 3B
    mybusconnectobj.setchannelinput( 11 , 1 );
    mybusconnectobj.setchannelstate( 11 , 0 );
    
    //  GND to 11 4Y
    mybusconnectobj.setchannelinput( 12 , 0 );
    mybusconnectobj.setchannelstate( 12 , 0 );
    
    //  VCC to 12 4A
    mybusconnectobj.setchannelinput( 13 , 1 );
    mybusconnectobj.setchannelstate( 13 , 0 );
    
    //  VCC to 13 4B
    mybusconnectobj.setchannelinput( 14 , 1 );
    mybusconnectobj.setchannelstate( 14 , 0 );
    
    // MVCC to 14 VCC
    mybusconnectobj.setchannelinput( 15 , 5 );
    mybusconnectobj.setchannelstate( 15 , 0 );
    
    
    i2cbusmaster.setbank(1);


    Serial.println("Bus Configured\n");

    command_measure();




    
  }








}






