
#include <Arduino.h>


// i2c tools
#include <i2ctools.h>

i2ctools myi2ctoolsobj;


#include <basetools.h>




// ina219 library
#include <ina219.h>
// declare an array of 4 multimeters
ina219 multimeter[4];

// mcp4725 library
#include <mcp4725.h>
mcp4725 dacarray[2];


// new definitions
#define TCA9548_I2C_ADDRESS 0x70

#define PCF8574_I2C_ADDRESS_0 0x20
#define PCF8574_I2C_ADDRESS_1 0x21
#define PCF8574_I2C_ADDRESS_2 0x22
#define PCF8574_I2C_ADDRESS_3 0x23
#define PCF8574_I2C_ADDRESS_4 0x24
#define PCF8574_I2C_ADDRESS_5 0x25
#define PCF8574_I2C_ADDRESS_6 0x26
#define PCF8574_I2C_ADDRESS_7 0x27






// command function declarations

String commandline = "";

void command_ina219_parser( String &commandline );

void command_mcp4725_parser( String &commandline );

void command_measure();

void command_7400_test( String &commandline );


// test routines

void command_pinset_parser( String &commandline );





void setup() {
  // put your setup code here, to run once:

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

  commandline.trim();
  commandline.toLowerCase();

  if ( commandline.equals("") ) { return; }

  // test command to check if we are connected
  if ( commandline.startsWith( "test" ) ) { Serial.println("Testing 123" ); return; }

  // enumerate the i2c bus.
  if ( commandline.startsWith( "i2cscan" ) ) { myi2ctoolsobj.i2cscan(); return; }


  // ina219 command parser
  if ( commandline.startsWith( "ina" ) ) { command_ina219_parser( commandline ); return; }

  if ( commandline.startsWith( "mcp" ) ) { command_mcp4725_parser( commandline ); return; }

  // quick measure
  if ( commandline.startsWith( "mr" ) ) { command_measure(); return; }






  // pinset parser
  if ( commandline.startsWith("ps" ) ) { command_pinset_parser( commandline ); return; }



  // 7400 parser
  if ( commandline.startsWith( "7400" ) ) { command_7400_test( commandline ); return; }





  // done command processing
  Serial.println("Command not recognised.");
  
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
  commandline = commandline.substring( 4 );
  commandline.trim();
  commandline.toLowerCase();

  // handle empty string
  if (!commandline.length()) { Serial.println("mcp4725 is awesome!" ); return; }

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

  Wire.beginTransmission( TCA9548_I2C_ADDRESS );
  Wire.write( 0x00000001 );
  Wire.endTransmission();

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

  if ( commandline.startsWith( "show" ) ) {

    uint8_t databyte = 0;

    Serial.println( "Pin\tBus\tState" );

    // switch to i2cbus 2
    Wire.beginTransmission( TCA9548_I2C_ADDRESS );
    Wire.write( 0b00000010 );
    Wire.endTransmission();

    // read out pin1
    // read out pin2
    Wire.requestFrom( PCF8574_I2C_ADDRESS_0 , 1 );
    databyte = Wire.read();

    Serial.print( "1\t" );
    Serial.print( (databyte & 0b00001110 ) >> 1 );
    Serial.print( "\t" );
    Serial.println( databyte & 0b1 );
    
    Serial.print( "2\t" );
    Serial.print( (databyte & 0b11100000 ) >> 5 );
    Serial.print( "\t" );
    Serial.println( ( databyte >> 4 ) & 0b1 );

    // read out pin3
    // read out pin4
    Wire.requestFrom( PCF8574_I2C_ADDRESS_1 , 1 );
    databyte = Wire.read();

    Serial.print( "3\t" );
    Serial.print( (databyte & 0b00001110 ) >> 1 );
    Serial.print( "\t" );
    Serial.println( databyte & 0b1 );
    
    Serial.print( "4\t" );
    Serial.print( (databyte & 0b11100000 ) >> 5 );
    Serial.print( "\t" );
    Serial.println( ( databyte >> 4 ) & 0b1 );
    
    
    // read out pin5
    // read out pin6
    Wire.requestFrom( PCF8574_I2C_ADDRESS_2 , 1 );
    databyte = Wire.read();

    Serial.print( "5\t" );
    Serial.print( (databyte & 0b00001110 ) >> 1 );
    Serial.print( "\t" );
    Serial.println( databyte & 0b1 );
    
    Serial.print( "6\t" );
    Serial.print( (databyte & 0b11100000 ) >> 5 );
    Serial.print( "\t" );
    Serial.println( ( databyte >> 4 ) & 0b1 );
    
    // read out pin7
    // read out pin8
    Wire.requestFrom( PCF8574_I2C_ADDRESS_3 , 1 );
    databyte = Wire.read();

    Serial.print( "7\t" );
    Serial.print( (databyte & 0b00001110 ) >> 1 );
    Serial.print( "\t" );
    Serial.println( databyte & 0b1 );
    
    Serial.print( "8\t" );
    Serial.print( (databyte & 0b11100000 ) >> 5 );
    Serial.print( "\t" );
    Serial.println( ( databyte >> 4 ) & 0b1 );






    // switch to icbus 4
    Wire.beginTransmission( TCA9548_I2C_ADDRESS );
    Wire.write( 0b00000100 );
    Wire.endTransmission();

    // read out pin9
    // read out pin10
    Wire.requestFrom( PCF8574_I2C_ADDRESS_4 , 1 );
    databyte = Wire.read();

    Serial.print( "9\t" );
    Serial.print( (databyte & 0b00001110 ) >> 1 );
    Serial.print( "\t" );
    Serial.println( databyte & 0b1 );
    
    Serial.print( "10\t" );
    Serial.print( (databyte & 0b11100000 ) >> 5 );
    Serial.print( "\t" );
    Serial.println( ( databyte >> 4 ) & 0b1 );

    // read out pin11
    // read out pin12
    Wire.requestFrom( PCF8574_I2C_ADDRESS_5 , 1 );
    databyte = Wire.read();

    Serial.print( "11\t" );
    Serial.print( (databyte & 0b00001110 ) >> 1 );
    Serial.print( "\t" );
    Serial.println( databyte & 0b1 );
    
    Serial.print( "12\t" );
    Serial.print( (databyte & 0b11100000 ) >> 5 );
    Serial.print( "\t" );
    Serial.println( ( databyte >> 4 ) & 0b1 );

    // read out pin13
    // read out pin14
    Wire.requestFrom( PCF8574_I2C_ADDRESS_6 , 1 );
    databyte = Wire.read();

    Serial.print( "13\t" );
    Serial.print( (databyte & 0b00001110 ) >> 1 );
    Serial.print( "\t" );
    Serial.println( databyte & 0b1 );
    
    Serial.print( "14\t" );
    Serial.print( (databyte & 0b11100000 ) >> 5 );
    Serial.print( "\t" );
    Serial.println( ( databyte >> 4 ) & 0b1 );

    // read out pin15
    // read out pin16
    Wire.requestFrom( PCF8574_I2C_ADDRESS_7 , 1 );
    databyte = Wire.read();

    Serial.print( "15\t" );
    Serial.print( (databyte & 0b00001110 ) >> 1 );
    Serial.print( "\t" );
    Serial.println( databyte & 0b1 );
    
    Serial.print( "16\t" );
    Serial.print( (databyte & 0b11100000 ) >> 5 );
    Serial.print( "\t" );
    Serial.println( ( databyte >> 4 ) & 0b1 );


    return;
  }

  uint8_t requestedpin = commandline.toInt();
  commandline = commandline.substring( commandline.indexOf(" ") + 1 );

  uint8_t requestedinput = commandline.toInt();
  commandline = commandline.substring( commandline.indexOf(" ") + 1 );

  uint8_t requestedstate = commandline.toInt();

  switch ( requestedpin ) {
  
    case 1: {
      // switch to the right bus
      Wire.beginTransmission( TCA9548_I2C_ADDRESS );
      Wire.write( 0b00000010 );
      Wire.endTransmission();
      
      // get the current state from the pcf
      Wire.requestFrom( PCF8574_I2C_ADDRESS_0 , 1 );
      uint8_t databyte = Wire.read();

      // we are on the high bit, so set that now
      databyte &= 0b11110000;
      databyte |= ( requestedstate & 0b1 );
      databyte |= ( ( requestedinput & 0b111 ) << 1 );

      Wire.beginTransmission( PCF8574_I2C_ADDRESS_0 );
      Wire.write( databyte );
      Wire.endTransmission();

      break;
    }

    case 2: {
      // switch to the right bus
      Wire.beginTransmission( TCA9548_I2C_ADDRESS );
      Wire.write( 0b00000010 );
      Wire.endTransmission();
      
      // get the current state from the pcf
      Wire.requestFrom( PCF8574_I2C_ADDRESS_0 , 1 );
      uint8_t databyte = Wire.read();

      // we are on the high bit, so set that now
      databyte &= 0b00001111;
      databyte |= ( ( requestedstate & 0b1 ) << 4);
      databyte |= ( ( requestedinput & 0b111 ) << 5 );

      Wire.beginTransmission( PCF8574_I2C_ADDRESS_0 );
      Wire.write( databyte );
      Wire.endTransmission();

      break;
    }

    case 3: {
      // switch to the right bus
      Wire.beginTransmission( TCA9548_I2C_ADDRESS );
      Wire.write( 0b00000010 );
      Wire.endTransmission();
      
      // get the current state from the pcf
      Wire.requestFrom( PCF8574_I2C_ADDRESS_1 , 1 );
      uint8_t databyte = Wire.read();

      // we are on the high bit, so set that now
      databyte &= 0b11110000;
      databyte |= ( requestedstate & 0b1 );
      databyte |= ( ( requestedinput & 0b111 ) << 1 );

      Wire.beginTransmission( PCF8574_I2C_ADDRESS_1 );
      Wire.write( databyte );
      Wire.endTransmission();

      break;
    }

    case 4: {
      // switch to the right bus
      Wire.beginTransmission( TCA9548_I2C_ADDRESS );
      Wire.write( 0b00000010 );
      Wire.endTransmission();
      
      // get the current state from the pcf
      Wire.requestFrom( PCF8574_I2C_ADDRESS_1 , 1 );
      uint8_t databyte = Wire.read();

      // we are on the high bit, so set that now
      databyte &= 0b00001111;
      databyte |= ( ( requestedstate & 0b1 ) << 4);
      databyte |= ( ( requestedinput & 0b111 ) << 5 );

      Wire.beginTransmission( PCF8574_I2C_ADDRESS_1 );
      Wire.write( databyte );
      Wire.endTransmission();

      break;
    }

    case 5: {
      // switch to the right bus
      Wire.beginTransmission( TCA9548_I2C_ADDRESS );
      Wire.write( 0b00000010 );
      Wire.endTransmission();
      
      // get the current state from the pcf
      Wire.requestFrom( PCF8574_I2C_ADDRESS_2 , 1 );
      uint8_t databyte = Wire.read();

      // we are on the high bit, so set that now
      databyte &= 0b11110000;
      databyte |= ( requestedstate & 0b1 );
      databyte |= ( ( requestedinput & 0b111 ) << 1 );

      Wire.beginTransmission( PCF8574_I2C_ADDRESS_2 );
      Wire.write( databyte );
      Wire.endTransmission();

      break;
    }

    case 6: {
      // switch to the right bus
      Wire.beginTransmission( TCA9548_I2C_ADDRESS );
      Wire.write( 0b00000010 );
      Wire.endTransmission();
      
      // get the current state from the pcf
      Wire.requestFrom( PCF8574_I2C_ADDRESS_2 , 1 );
      uint8_t databyte = Wire.read();

      // we are on the high bit, so set that now
      databyte &= 0b00001111;
      databyte |= ( ( requestedstate & 0b1 ) << 4);
      databyte |= ( ( requestedinput & 0b111 ) << 5 );

      Wire.beginTransmission( PCF8574_I2C_ADDRESS_2 );
      Wire.write( databyte );
      Wire.endTransmission();

      break;
    }

    case 7: {
      // switch to the right bus
      Wire.beginTransmission( TCA9548_I2C_ADDRESS );
      Wire.write( 0b00000010 );
      Wire.endTransmission();
      
      // get the current state from the pcf
      Wire.requestFrom( PCF8574_I2C_ADDRESS_3 , 1 );
      uint8_t databyte = Wire.read();

      // we are on the high bit, so set that now
      databyte &= 0b11110000;
      databyte |= ( requestedstate & 0b1 );
      databyte |= ( ( requestedinput & 0b111 ) << 1 );

      Wire.beginTransmission( PCF8574_I2C_ADDRESS_3 );
      Wire.write( databyte );
      Wire.endTransmission();

      break;
    }

    case 8: {
      // switch to the right bus
      Wire.beginTransmission( TCA9548_I2C_ADDRESS );
      Wire.write( 0b00000010 );
      Wire.endTransmission();
      
      // get the current state from the pcf
      Wire.requestFrom( PCF8574_I2C_ADDRESS_3 , 1 );
      uint8_t databyte = Wire.read();

      // we are on the high bit, so set that now
      databyte &= 0b00001111;
      databyte |= ( ( requestedstate & 0b1 ) << 4);
      databyte |= ( ( requestedinput & 0b111 ) << 5 );

      Wire.beginTransmission( PCF8574_I2C_ADDRESS_3 );
      Wire.write( databyte );
      Wire.endTransmission();

      break;
    }

    case 9: {
      // switch to the right bus
      Wire.beginTransmission( TCA9548_I2C_ADDRESS );
      Wire.write( 0b00000100 );
      Wire.endTransmission();
      
      // get the current state from the pcf
      Wire.requestFrom( PCF8574_I2C_ADDRESS_4 , 1 );
      uint8_t databyte = Wire.read();

      // we are on the high bit, so set that now
      databyte &= 0b11110000;
      databyte |= ( requestedstate & 0b1 );
      databyte |= ( ( requestedinput & 0b111 ) << 1 );

      Wire.beginTransmission( PCF8574_I2C_ADDRESS_4 );
      Wire.write( databyte );
      Wire.endTransmission();

      break;
    }

    case 10: {
      // switch to the right bus
      Wire.beginTransmission( TCA9548_I2C_ADDRESS );
      Wire.write( 0b00000100 );
      Wire.endTransmission();
      
      // get the current state from the pcf
      Wire.requestFrom( PCF8574_I2C_ADDRESS_4 , 1 );
      uint8_t databyte = Wire.read();

      // we are on the high bit, so set that now
      databyte &= 0b00001111;
      databyte |= ( ( requestedstate & 0b1 ) << 4);
      databyte |= ( ( requestedinput & 0b111 ) << 5 );

      Wire.beginTransmission( PCF8574_I2C_ADDRESS_4 );
      Wire.write( databyte );
      Wire.endTransmission();

      break;
    }

    case 11: {
      // switch to the right bus
      Wire.beginTransmission( TCA9548_I2C_ADDRESS );
      Wire.write( 0b00000100 );
      Wire.endTransmission();
      
      // get the current state from the pcf
      Wire.requestFrom( PCF8574_I2C_ADDRESS_5 , 1 );
      uint8_t databyte = Wire.read();

      // we are on the high bit, so set that now
      databyte &= 0b11110000;
      databyte |= ( requestedstate & 0b1 );
      databyte |= ( ( requestedinput & 0b111 ) << 1 );

      Wire.beginTransmission( PCF8574_I2C_ADDRESS_5 );
      Wire.write( databyte );
      Wire.endTransmission();

      break;
    }

    case 12: {
      // switch to the right bus
      Wire.beginTransmission( TCA9548_I2C_ADDRESS );
      Wire.write( 0b00000100 );
      Wire.endTransmission();
      
      // get the current state from the pcf
      Wire.requestFrom( PCF8574_I2C_ADDRESS_5 , 1 );
      uint8_t databyte = Wire.read();

      // we are on the high bit, so set that now
      databyte &= 0b00001111;
      databyte |= ( ( requestedstate & 0b1 ) << 4);
      databyte |= ( ( requestedinput & 0b111 ) << 5 );

      Wire.beginTransmission( PCF8574_I2C_ADDRESS_5 );
      Wire.write( databyte );
      Wire.endTransmission();

      break;
    }

    case 13: {
      // switch to the right bus
      Wire.beginTransmission( TCA9548_I2C_ADDRESS );
      Wire.write( 0b00000100 );
      Wire.endTransmission();
      
      // get the current state from the pcf
      Wire.requestFrom( PCF8574_I2C_ADDRESS_6 , 1 );
      uint8_t databyte = Wire.read();

      // we are on the high bit, so set that now
      databyte &= 0b11110000;
      databyte |= ( requestedstate & 0b1 );
      databyte |= ( ( requestedinput & 0b111 ) << 1 );

      Wire.beginTransmission( PCF8574_I2C_ADDRESS_6 );
      Wire.write( databyte );
      Wire.endTransmission();

      break;
    }

    case 14: {
      // switch to the right bus
      Wire.beginTransmission( TCA9548_I2C_ADDRESS );
      Wire.write( 0b00000100 );
      Wire.endTransmission();
      
      // get the current state from the pcf
      Wire.requestFrom( PCF8574_I2C_ADDRESS_6 , 1 );
      uint8_t databyte = Wire.read();

      // we are on the high bit, so set that now
      databyte &= 0b00001111;
      databyte |= ( ( requestedstate & 0b1 ) << 4);
      databyte |= ( ( requestedinput & 0b111 ) << 5 );

      Wire.beginTransmission( PCF8574_I2C_ADDRESS_6 );
      Wire.write( databyte );
      Wire.endTransmission();

      break;
    }

    case 15: {
      // switch to the right bus
      Wire.beginTransmission( TCA9548_I2C_ADDRESS );
      Wire.write( 0b00000100 );
      Wire.endTransmission();
      
      // get the current state from the pcf
      Wire.requestFrom( PCF8574_I2C_ADDRESS_7 , 1 );
      uint8_t databyte = Wire.read();

      // we are on the high bit, so set that now
      databyte &= 0b11110000;
      databyte |= ( requestedstate & 0b1 );
      databyte |= ( ( requestedinput & 0b111 ) << 1 );

      Wire.beginTransmission( PCF8574_I2C_ADDRESS_7 );
      Wire.write( databyte );
      Wire.endTransmission();

      break;
    }

    case 16: {
      // switch to the right bus
      Wire.beginTransmission( TCA9548_I2C_ADDRESS );
      Wire.write( 0b00000100 );
      Wire.endTransmission();
      
      // get the current state from the pcf
      Wire.requestFrom( PCF8574_I2C_ADDRESS_7 , 1 );
      uint8_t databyte = Wire.read();

      // we are on the high bit, so set that now
      databyte &= 0b00001111;
      databyte |= ( ( requestedstate & 0b1 ) << 4);
      databyte |= ( ( requestedinput & 0b111 ) << 5 );

      Wire.beginTransmission( PCF8574_I2C_ADDRESS_7 );
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

    // input bus
    // 0 GND
    // 1 VCC
    // 2 MGND
    // 3 MVCC
    // 4 VMVCC1
    // 5 VMVCC2
    // 6 NC
    // 7 NC


    Serial.println( "Testing gate 1.");

    // configure all the pins
    
    String commandstring = "";

    // 1A VMVCC1
    commandstring = "ps 1 4 0";
    command_pinset_parser( commandstring );
    
    // 1B VMVCC2
    commandstring = "ps 2 5 0";
    command_pinset_parser( commandstring );
    
    // 1Y MGND
    commandstring = "ps 3 2 0";
    command_pinset_parser( commandstring );
    
    // 2A VCC
    commandstring = "ps 4 1 0";
    command_pinset_parser( commandstring );
    // 2B VCC
    commandstring = "ps 5 1 0";
    command_pinset_parser( commandstring );
    // 2Y GND
    commandstring = "ps 6 0 0";
    command_pinset_parser( commandstring );

    // 3A VCC
    commandstring = "ps 11 1 0";
    command_pinset_parser( commandstring );
    // 3B VCC
    commandstring = "ps 10 1 0";
    command_pinset_parser( commandstring );
    // 3Y GND
    commandstring = "ps 9 0 0";
    command_pinset_parser( commandstring );

    // 4A VCC
    commandstring = "ps 14 1 0";
    command_pinset_parser( commandstring );
    // 4B VCC
    commandstring = "ps 15 1 0";
    command_pinset_parser( commandstring );
    // 4Y GND
    commandstring = "ps 13 0 0";
    command_pinset_parser( commandstring );

    // GND GND
    commandstring = "ps 7 0 0";
    command_pinset_parser( commandstring );
    // VCC VCC
    commandstring = "ps 16 3 0";
    command_pinset_parser( commandstring );
    
    Serial.println( "Bus configured!" );


    
  }








}






