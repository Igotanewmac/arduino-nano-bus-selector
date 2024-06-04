
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


// tca9548 library
#include <tca9548.h>
tca9548 i2cbusswitcher;



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


// input bus addresses
#define    BUS_GND 0x00
#define    BUS_VCC 0x01
#define   BUS_MGND 0x02
#define   BUS_MVCC 0x03
#define BUS_VMVCC0 0x04
#define BUS_VMVCC1 0x05


// input bus state
#define    BUS_CONNECTED 0x00
#define BUS_DISCONNECTED 0x01






// i2c bus addresses
#define      I2C_BUS_MEASURE 0b00000001
#define  I2C_BUS_PINS_0_TO_8 0b00000010
#define I2C_BUS_PINS_9_TO_16 0b00000100








// command function declarations

String commandline = "";

void command_ina219_parser( String &commandline );

void command_mcp4725_parser( String &commandline );

void command_measure();

void command_pinset_parser( String &commandline );








/*

████████╗███████╗███████╗████████╗                              
╚══██╔══╝██╔════╝██╔════╝╚══██╔══╝                              
   ██║   █████╗  ███████╗   ██║                                 
   ██║   ██╔══╝  ╚════██║   ██║                                 
   ██║   ███████╗███████║   ██║                                 
   ╚═╝   ╚══════╝╚══════╝   ╚═╝                                 
                                                                
██████╗  ██████╗ ██╗   ██╗████████╗██╗███╗   ██╗███████╗███████╗
██╔══██╗██╔═══██╗██║   ██║╚══██╔══╝██║████╗  ██║██╔════╝██╔════╝
██████╔╝██║   ██║██║   ██║   ██║   ██║██╔██╗ ██║█████╗  ███████╗
██╔══██╗██║   ██║██║   ██║   ██║   ██║██║╚██╗██║██╔══╝  ╚════██║
██║  ██║╚██████╔╝╚██████╔╝   ██║   ██║██║ ╚████║███████╗███████║
╚═╝  ╚═╝ ╚═════╝  ╚═════╝    ╚═╝   ╚═╝╚═╝  ╚═══╝╚══════╝╚══════╝
                                                                
*/


// test routines


float findvih( uint8_t statetodetect ) {

  // make sure we are on the right bus
  Wire.beginTransmission( TCA9548_I2C_ADDRESS );
  Wire.write( 0x01 );
  Wire.endTransmission();


  uint16_t curdacvalue = 0;
  float busvoltage = 0;

  dacarray[0].setval( curdacvalue );
  delay(1);

  for ( curdacvalue = 0 ; curdacvalue < 4096 ; curdacvalue++ ) {

    dacarray[0].setval( curdacvalue );
    delay(1);

    busvoltage = multimeter[1].getbusvoltage();

    if ( statetodetect ) {
      // check for high output
      if ( busvoltage > 2 ) {
        return busvoltage;
      }
    }
    else {
      // check for low output
      if ( busvoltage < 2 ) {
        return busvoltage;
      }
    }

  }

  return 0;

}

float findvil( uint8_t statetodetect ) {

  // make sure we are on the right bus
  Wire.beginTransmission( TCA9548_I2C_ADDRESS );
  Wire.write( 0x01 );
  Wire.endTransmission();


  uint16_t curdacvalue = 4095;
  float busvoltage = 0;

  dacarray[0].setval( curdacvalue );
  delay(1);

  for ( curdacvalue = 4095 ; curdacvalue > 0 ; curdacvalue-- ) {

    dacarray[0].setval( curdacvalue );
    delay(1);

    busvoltage = multimeter[1].getbusvoltage();

    if ( statetodetect ) {
      // check for high output
      if ( busvoltage > 2 ) {
        return busvoltage;
      }
    }
    else {
      // check for low output
      if ( busvoltage < 2 ) {
        return busvoltage;
      }
    }

  }

  return 0;

}


// pinset shim
String pinsetshimdata = "";
void pinsetshim( uint8_t requestedpin , uint8_t requestedinput , uint8_t requestedstate ) {
  pinsetshimdata = "ps " + String(requestedpin) + " " + String( requestedinput ) + " " + String(requestedstate) ;
  command_pinset_parser( pinsetshimdata );
}


void showpass() {
  Serial.println( "Pass!" );
}

void showfail() {
  Serial.println( "Fail!" );
}



void command_7400_test( String &commandline );

void command_7402_test( String &commandline );

void command_7404_test( String &commandline );

void command_7408_test( String &commandline );

void command_7432_test( String &commandline );

void command_7486_test( String &commandline );










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

  // turn on bus switcher
  i2cbusswitcher.begin( TCA9548_I2C_ADDRESS );

  // all done
  
}






/*

██╗      ██████╗  ██████╗ ██████╗ 
██║     ██╔═══██╗██╔═══██╗██╔══██╗
██║     ██║   ██║██║   ██║██████╔╝
██║     ██║   ██║██║   ██║██╔═══╝ 
███████╗╚██████╔╝╚██████╔╝██║     
╚══════╝ ╚═════╝  ╚═════╝ ╚═╝     
                                  
*/






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


  // 7402 parser
  if ( commandline.startsWith( "7402" ) ) { command_7402_test( commandline ); return; }


  // 7404 parser
  if ( commandline.startsWith( "7404" ) ) { command_7404_test( commandline ); return; }

  // 7408 parser
  if ( commandline.startsWith( "7408" ) ) { command_7408_test( commandline ); return; }

  // 7432 parser
  if ( commandline.startsWith( "7432" ) ) { command_7432_test( commandline ); return; }

  // 7486 parser
  if ( commandline.startsWith( "7486" ) ) { command_7486_test( commandline ); return; }
  
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







/*

██████╗ ██╗███╗   ██╗███████╗███████╗████████╗  
██╔══██╗██║████╗  ██║██╔════╝██╔════╝╚══██╔══╝  
██████╔╝██║██╔██╗ ██║███████╗█████╗     ██║     
██╔═══╝ ██║██║╚██╗██║╚════██║██╔══╝     ██║     
██║     ██║██║ ╚████║███████║███████╗   ██║     
╚═╝     ╚═╝╚═╝  ╚═══╝╚══════╝╚══════╝   ╚═╝     
                                                
██████╗  █████╗ ██████╗ ███████╗███████╗██████╗ 
██╔══██╗██╔══██╗██╔══██╗██╔════╝██╔════╝██╔══██╗
██████╔╝███████║██████╔╝███████╗█████╗  ██████╔╝
██╔═══╝ ██╔══██║██╔══██╗╚════██║██╔══╝  ██╔══██╗
██║     ██║  ██║██║  ██║███████║███████╗██║  ██║
╚═╝     ╚═╝  ╚═╝╚═╝  ╚═╝╚══════╝╚══════╝╚═╝  ╚═╝
                                                
*/



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







/*

███████╗██╗  ██╗ ██████╗  ██████╗         
╚════██║██║  ██║██╔═████╗██╔═████╗        
    ██╔╝███████║██║██╔██║██║██╔██║        
   ██╔╝ ╚════██║████╔╝██║████╔╝██║        
   ██║       ██║╚██████╔╝╚██████╔╝        
   ╚═╝       ╚═╝ ╚═════╝  ╚═════╝         
                                          
███████╗████████╗ █████╗ ██████╗ ████████╗
██╔════╝╚══██╔══╝██╔══██╗██╔══██╗╚══██╔══╝
███████╗   ██║   ███████║██████╔╝   ██║   
╚════██║   ██║   ██╔══██║██╔══██╗   ██║   
███████║   ██║   ██║  ██║██║  ██║   ██║   
╚══════╝   ╚═╝   ╚═╝  ╚═╝╚═╝  ╚═╝   ╚═╝   
                                          
*/

void gate_test_nand( uint16_t indexnumber ) {
  
  i2cbusswitcher.setbank( I2C_BUS_MEASURE );

  Serial.println( "G A B Y Result");

  // 0 0 1
  Serial.print( indexnumber );
  Serial.print( " 0 0 1 " );

  dacarray[0].setval(0);
  dacarray[1].setval(0);
  delay(1);
  if ( multimeter[1].getbusvoltage() > 1 ) {
    Serial.println( "pass!" );
  }
  else {
    Serial.println( "Fail" );
    command_measure();
    while(1);
  }

  // 0 1 1
  Serial.print( indexnumber );
  Serial.print( " 0 1 1 " );

  dacarray[0].setval(0);
  dacarray[1].setval(4095);
  delay(1);
  if ( multimeter[1].getbusvoltage() > 1 ) {
    Serial.println( "pass!" );
  }
  else {
    Serial.println( "Fail" );
    command_measure();
    while(1);
  }

  // 1 0 1
  Serial.print( indexnumber );
  Serial.print( " 1 0 1 " );

  dacarray[0].setval(4095);
  dacarray[1].setval(0);
  delay(1);
  if ( multimeter[1].getbusvoltage() > 1 ) {
    Serial.println( "pass!" );
  }
  else {
    Serial.println( "Fail" );
    command_measure();
    while(1);
  }

  // 1 1 0
  Serial.print( indexnumber );
  Serial.print( " 1 1 0 " );

  dacarray[0].setval(4095);
  dacarray[1].setval(4095);
  delay(1);
  if ( multimeter[1].getbusvoltage() < 1 ) {
    Serial.println( "pass!" );
  }
  else {
    Serial.println( "Fail" );
    command_measure();
    while(1);
  }

}


void command_7400_test( String &commandline ) {

  commandline = commandline.substring( 5 );

  if ( commandline.equals("") ) { commandline = "show"; }

  if ( commandline.startsWith("show") ) {

    Serial.println(F("7400 Series Logic."));
    Serial.println();
    Serial.println(F("Quad 2 input NAND."));
    Serial.println();
    Serial.println(F("         +----u----+         "));
    Serial.println(F(" 1.  1A -| 1    14 |- VCC .16"));
    Serial.println(F(" 2.  1B -| 2    13 |- 4A  .15"));
    Serial.println(F(" 3.  1Y -| 3    12 |- 4B  .14"));
    Serial.println(F(" 4.  2A -| 4    11 |- 4Y  .13"));
    Serial.println(F(" 5.  2B -| 5    10 |- 3A  .12"));
    Serial.println(F(" 6.  2Y -| 6    9  |- 3B  .11"));
    Serial.println(F(" 7. GND -| 7    8  |- 3Y  .10"));
    Serial.println(F(" 8.      +---------+      .9 "));
    Serial.println();
    
  }

  if ( commandline.startsWith("test") ) {
    String templine = "show";
    command_7486_test( templine );

    // configure pins
    // 1A
    pinsetshim( 1 , BUS_VCC , BUS_CONNECTED );
    // 1B
    pinsetshim( 2 , BUS_VCC , BUS_CONNECTED );
    // 1Y
    pinsetshim( 3 , BUS_GND , BUS_CONNECTED );
    // 2A
    pinsetshim( 4 , BUS_VCC , BUS_CONNECTED );
    // 2B
    pinsetshim( 5 , BUS_VCC , BUS_CONNECTED );
    // 2Y
    pinsetshim( 6 , BUS_GND , BUS_CONNECTED );
    // 3A
    pinsetshim( 12 , BUS_GND , BUS_CONNECTED );
    // 3B
    pinsetshim( 11 , BUS_GND , BUS_CONNECTED );
    // 3Y
    pinsetshim( 10 , BUS_GND , BUS_CONNECTED );
    // 4A
    pinsetshim( 15 , BUS_GND , BUS_CONNECTED );
    // 4B
    pinsetshim( 14 , BUS_GND , BUS_CONNECTED );
    // 4Y
    pinsetshim( 13 , BUS_GND , BUS_CONNECTED );

    // VCC
    pinsetshim( 16 , BUS_MVCC , BUS_CONNECTED );
    // GND
    pinsetshim( 7 , BUS_GND , BUS_CONNECTED );



    // connect gate 1 to tester
    // 1A
    pinsetshim( 1 , BUS_VMVCC0 , BUS_CONNECTED );
    // 1B
    pinsetshim( 2 , BUS_VMVCC1 , BUS_CONNECTED );
    // 1Y
    pinsetshim( 3 , BUS_MGND , BUS_CONNECTED );

    gate_test_nand(1);
    
    // 1A
    pinsetshim( 1 , BUS_VCC , BUS_CONNECTED );
    // 1B
    pinsetshim( 2 , BUS_VCC , BUS_CONNECTED );
    // 1Y
    pinsetshim( 3 , BUS_GND , BUS_CONNECTED );

    // connect gate 2 to tester
    // 2A
    pinsetshim( 4 , BUS_VMVCC0 , BUS_CONNECTED );
    // 2B
    pinsetshim( 5 , BUS_VMVCC1 , BUS_CONNECTED );
    // 2Y
    pinsetshim( 6 , BUS_MGND , BUS_CONNECTED );

    gate_test_nand(2);
    
    // 2A
    pinsetshim( 4 , BUS_VCC , BUS_CONNECTED );
    // 2B
    pinsetshim( 5 , BUS_VCC , BUS_CONNECTED );
    // 2Y
    pinsetshim( 6 , BUS_GND , BUS_CONNECTED );

    // connect gate 3 to tester
    // 3A
    pinsetshim( 12 , BUS_VMVCC0 , BUS_CONNECTED );
    // 3B
    pinsetshim( 11 , BUS_VMVCC1 , BUS_CONNECTED );
    // 3Y
    pinsetshim( 10 , BUS_MGND , BUS_CONNECTED );

    gate_test_nand(3);
    
    //3A
    pinsetshim( 12 , BUS_VCC , BUS_CONNECTED );
    // 3B
    pinsetshim( 11 , BUS_VCC , BUS_CONNECTED );
    // 3Y
    pinsetshim( 10 , BUS_GND , BUS_CONNECTED );

    // connect gate 4 to tester
    // 4A
    pinsetshim( 15 , BUS_VMVCC0 , BUS_CONNECTED );
    // 4B
    pinsetshim( 14 , BUS_VMVCC1 , BUS_CONNECTED );
    // 4Y
    pinsetshim( 13 , BUS_MGND , BUS_CONNECTED );

    gate_test_nand(4);
    
    // 4A
    pinsetshim( 15 , BUS_GND , BUS_CONNECTED );
    // 4B
    pinsetshim( 14 , BUS_GND , BUS_CONNECTED );
    // 4Y
    pinsetshim( 13 , BUS_GND , BUS_CONNECTED );




    // Voh
    // 1A
    pinsetshim( 1 , BUS_GND , BUS_CONNECTED );
    // 1B
    pinsetshim( 2 , BUS_GND , BUS_CONNECTED );
    // 1Y
    pinsetshim( 3 , BUS_MGND , BUS_CONNECTED );

    delay(1);
    i2cbusswitcher.setbank( I2C_BUS_MEASURE );
    delay(1);

    Serial.print( "Voh: " );
    Serial.print( multimeter[1].getbusvoltage() );
    Serial.println( "V" );

    // 1A
    pinsetshim( 1 , BUS_VCC , BUS_CONNECTED );
    // 1B
    pinsetshim( 2 , BUS_VCC , BUS_CONNECTED );
    // 1Y
    pinsetshim( 3 , BUS_GND , BUS_CONNECTED );




    // Vol
    pinsetshim( 1 , BUS_VCC , BUS_CONNECTED );
    pinsetshim( 2 , BUS_VCC , BUS_CONNECTED );
    pinsetshim( 3 , BUS_MGND , BUS_CONNECTED );

    delay(1);
    i2cbusswitcher.setbank( I2C_BUS_MEASURE );
    delay(1);

    Serial.print( "Vol: " );
    Serial.print( multimeter[1].getbusvoltage() );
    Serial.println( "V" );
    
    pinsetshim( 1 , BUS_GND , BUS_CONNECTED );
    pinsetshim( 2 , BUS_GND , BUS_CONNECTED );
    pinsetshim( 3 , BUS_GND , BUS_CONNECTED );
    
  }

}



/*

███████╗██╗  ██╗ ██████╗ ██████╗ 
╚════██║██║  ██║██╔═████╗╚════██╗
    ██╔╝███████║██║██╔██║ █████╔╝
   ██╔╝ ╚════██║████╔╝██║██╔═══╝ 
   ██║       ██║╚██████╔╝███████╗
   ╚═╝       ╚═╝ ╚═════╝ ╚══════╝
                                 
███████╗████████╗ █████╗ ██████╗ ████████╗
██╔════╝╚══██╔══╝██╔══██╗██╔══██╗╚══██╔══╝
███████╗   ██║   ███████║██████╔╝   ██║   
╚════██║   ██║   ██╔══██║██╔══██╗   ██║   
███████║   ██║   ██║  ██║██║  ██║   ██║   
╚══════╝   ╚═╝   ╚═╝  ╚═╝╚═╝  ╚═╝   ╚═╝   
                                          
*/


void command_7402_test( String &commandline ) { return; }

/*

void command_7402_test( String &commandline ) {
  
  commandline = commandline.substring( 5 );

  if ( commandline.equals("") ) { commandline = "show"; }

  if ( commandline.startsWith("show") ) {

    Serial.println(F("7402 Series Logic."));
    Serial.println(F(""));
    Serial.println(F("Quad 2 input NOR gates."));
    Serial.println(F(""));
    Serial.println(F("      +----u----+     "));
    Serial.println(F("  1Y -| 1    14 |- VCC"));
    Serial.println(F("  1A -| 2    13 |- 4Y "));
    Serial.println(F("  1B -| 3    12 |- 4B "));
    Serial.println(F("  2Y -| 4    11 |- 4A "));
    Serial.println(F("  2A -| 5    10 |- 3Y "));
    Serial.println(F("  2B -| 6    9  |- 3B "));
    Serial.println(F(" GND -| 7    8  |- 3A "));
    Serial.println(F("      +---------+     "));
    Serial.println(F(""));
    
  }

  if ( commandline.startsWith("test") ) {

    Serial.println(F("7402 Series Logic."));
    Serial.println(F(""));
    Serial.println(F("Quad 2 input NOR gates."));
    Serial.println(F(""));
    Serial.println(F("      +----u----+     "));
    Serial.println(F("  1Y -| 1    14 |- VCC"));
    Serial.println(F("  1A -| 2    13 |- 4Y "));
    Serial.println(F("  1B -| 3    12 |- 4B "));
    Serial.println(F("  2Y -| 4    11 |- 4A "));
    Serial.println(F("  2A -| 5    10 |- 3Y "));
    Serial.println(F("  2B -| 6    9  |- 3B "));
    Serial.println(F(" GND -| 7    8  |- 3A "));
    Serial.println(F("      +---------+     "));
    Serial.println(F(""));



    // configure all the pins
    
    String commandstring = "";

    // 1A VCC
    pinsetshim( 2 , 0 , 1 );
    // 1B VCC
    pinsetshim( 3 , 0 , 1 );
    // 1Y GND
    pinsetshim( 1 , 0 , 1 );
    
    // 2A VCC
    pinsetshim( 5 , 0 , 1 );
    // 2B VCC
    pinsetshim( 6 , 0 , 1 );
    // 2Y GND
    pinsetshim( 4 , 0 , 1 );
    
    // 3A VCC
    pinsetshim( 10 , 0 , 1 );
    // 3B VCC
    pinsetshim( 11 , 0 , 1 );
    // 3Y GND
    pinsetshim( 12 , 0 , 1 );
    
    // 4A VCC
    pinsetshim( 13 , 0 , 1 );
    // 4B VCC
    pinsetshim( 14 , 0 , 1 );
    // 4Y GND
    pinsetshim( 15 , 0 , 1 );
    

    // GND GND
    pinsetshim( 7 , 0 , 0 );
    // VCC VCC
    pinsetshim( 16 , 3 , 0 );
    

    Serial.println( F("Bus configured!") );


    
    Serial.println( F("Gate 1:") );
    
    // connect gate A
    // 1A VCC
    pinsetshim( 2 , 4 , 0 );
    // 1B VCC
    pinsetshim( 3 , 5 , 0 );
    // 1Y MGND
    pinsetshim( 1 , 2 , 0 );
    
    // now switch to test and measurement bank
    Wire.beginTransmission( TCA9548_I2C_ADDRESS );
    Wire.write( 0b00000001 );
    Wire.endTransmission();

    // check basic functionality
    Serial.println( F("A B Y Result" ));
    // A B Y
    // 0 0 1
    Serial.print( F("0 0 1 " ));
    dacarray[0].setval(0);
    dacarray[1].setval(0);
    delay(1);
    // check for output voltage
    if ( multimeter[1].getbusvoltage() > 2 ) {
      showpass();
    }
    else {
      showfail();
    }

    // A B Y
    // 0 1 0
    Serial.print( F("0 1 0 " ));
    dacarray[0].setval(0);
    dacarray[1].setval(4095);
    delay(1);
    // check for output voltage
    if ( multimeter[1].getbusvoltage() < 2 ) {
      showpass();
    }
    else {
      showfail();
    }
    

    // A B Y
    // 1 0 0
    Serial.print( F("1 0 0 " ));
    dacarray[0].setval(4095);
    dacarray[1].setval(0);
    delay(1);
    // check for output voltage
    if ( multimeter[1].getbusvoltage() < 2 ) {
      showpass();
    }
    else {
      showfail();
    }
    

    // A B Y
    // 1 1 0
    Serial.print( F("1 1 0 " ));
    dacarray[0].setval(4095);
    dacarray[1].setval(4095);
    delay(1);
    // check for output voltage
    if ( multimeter[1].getbusvoltage() < 2 ) {
      showpass();
    }
    else {
      showfail();
    }
    

    // 1A VCC
    pinsetshim( 1 , 1 , 1 );
    // 1B VCC
    pinsetshim( 2 , 1 , 1 );
    // 1Y MGND
    pinsetshim( 3 , 0 , 1 );
    




    Serial.println( F("Gate 2:" ));
    
    // connect gate A
    // 2A VCC
    pinsetshim( 5 , 4 , 0 );
    // 2B VCC
    pinsetshim( 6 , 5 , 0 );
    // 2Y MGND
    pinsetshim( 4 , 2 , 0 );
    
    // now switch to test and measurement bank
    Wire.beginTransmission( TCA9548_I2C_ADDRESS );
    Wire.write( 0b00000001 );
    Wire.endTransmission();

    // check basic functionality
    Serial.println( F("A B Y Result" ));
    // A B Y
    // 0 0 1
    Serial.print( F("0 0 1 " ));
    dacarray[0].setval(0);
    dacarray[1].setval(0);
    delay(1);
    // check for output voltage
    if ( multimeter[1].getbusvoltage() > 2 ) {
      showpass();
    }
    else {
      showfail();
    }
    

    // A B Y
    // 0 1 0
    Serial.print( F("0 1 0 " ));
    dacarray[0].setval(0);
    dacarray[1].setval(4095);
    delay(1);
    // check for output voltage
    if ( multimeter[1].getbusvoltage() < 2 ) {
      showpass();
    }
    else {
      showfail();
    }
    

    // A B Y
    // 1 0 0
    Serial.print( F("1 0 0 " ));
    dacarray[0].setval(4095);
    dacarray[1].setval(0);
    delay(1);
    // check for output voltage
    if ( multimeter[1].getbusvoltage() < 2 ) {
      showpass();
    }
    else {
      showfail();
    }
    

    // A B Y
    // 1 1 0
    Serial.print( F("1 1 0 " ));
    dacarray[0].setval(4095);
    dacarray[1].setval(4095);
    delay(1);
    // check for output voltage
    if ( multimeter[1].getbusvoltage() < 2 ) {
      showpass();
    }
    else {
      showfail();
    }
    

    // 2A VCC
    pinsetshim( 4 , 1 , 1 );
    // 2B VCC
    pinsetshim( 5 , 1 , 1 );
    // 2Y GND
    pinsetshim( 6 , 0 , 1 );
    





    Serial.println( F("Gate 3:" ));
    
    // 3A VCC
    pinsetshim( 10 , 4 , 0 );
    // 3B VCC
    pinsetshim( 11 , 5 , 0 );
    // 3Y MGND
    pinsetshim( 12 , 2 , 0 );
    
    // now switch to test and measurement bank
    Wire.beginTransmission( TCA9548_I2C_ADDRESS );
    Wire.write( 0b00000001 );
    Wire.endTransmission();

    // check basic functionality
    Serial.println( F("A B Y Result" ));
    // A B Y
    // 0 0 1
    Serial.print( F("0 0 1 " ));
    dacarray[0].setval(0);
    dacarray[1].setval(0);
    delay(1);
    // check for output voltage
    if ( multimeter[1].getbusvoltage() > 2 ) {
      showpass();
    }
    else {
      showfail();
    }
    
    // A B Y
    // 0 1 0
    Serial.print( F("0 1 0 " ));
    dacarray[0].setval(0);
    dacarray[1].setval(4095);
    delay(1);
    // check for output voltage
    if ( multimeter[1].getbusvoltage() < 2 ) {
      showpass();
    }
    else {
      showfail();
    }
    
    // A B Y
    // 1 0 0
    Serial.print( F("1 0 0 " ));
    dacarray[0].setval(4095);
    dacarray[1].setval(0);
    delay(1);
    // check for output voltage
    if ( multimeter[1].getbusvoltage() < 2 ) {
      showpass();
    }
    else {
      showfail();
    }
    

    // A B Y
    // 1 1 0
    Serial.print( F("1 1 0 " ));
    dacarray[0].setval(4095);
    dacarray[1].setval(4095);
    delay(1);
    // check for output voltage
    if ( multimeter[1].getbusvoltage() < 2 ) {
      showpass();
    }
    else {
      showfail();
    }
    

    // 2A VCC
    pinsetshim( 10 , 1 , 1 );
    // 2B VCC
    pinsetshim( 11 , 1 , 1 );
    // 2Y GND
    pinsetshim( 12 , 0 , 1 );
    





    Serial.println( F("Gate 4:" ));
    
    // 4A VCC
    pinsetshim( 13 , 4 , 0 );
    // 4B VCC
    pinsetshim( 14 , 5 , 0 );
    // 4Y MGND
    pinsetshim( 15 , 2 , 0 );
    
    // now switch to test and measurement bank
    Wire.beginTransmission( TCA9548_I2C_ADDRESS );
    Wire.write( 0b00000001 );
    Wire.endTransmission();

    // check basic functionality
    Serial.println( F("A B Y Result" ));
    // A B Y
    // 0 0 1
    Serial.print( F("0 0 1 " ));
    dacarray[0].setval(0);
    dacarray[1].setval(0);
    delay(1);
    // check for output voltage
    if ( multimeter[1].getbusvoltage() > 2 ) {
      showpass();
    }
    else {
      showfail();
    }
    

    // A B Y
    // 0 1 0
    Serial.print( F("0 1 0 " ));
    dacarray[0].setval(0);
    dacarray[1].setval(4095);
    delay(1);
    // check for output voltage
    if ( multimeter[1].getbusvoltage() < 2 ) {
      showpass();
    }
    else {
      showfail();
    }
    

    // A B Y
    // 1 0 0
    Serial.print( F("1 0 0 " ));
    dacarray[0].setval(4095);
    dacarray[1].setval(0);
    delay(1);
    // check for output voltage
    if ( multimeter[1].getbusvoltage() < 2 ) {
      showpass();
    }
    else {
      showfail();
    }
    
    // A B Y
    // 1 1 0
    Serial.print( F("1 1 0 " ));
    dacarray[0].setval(4095);
    dacarray[1].setval(4095);
    delay(1);
    // check for output voltage
    if ( multimeter[1].getbusvoltage() < 2 ) {
      showpass();
    }
    else {
      showfail();
    }

    // 2A VCC
    pinsetshim( 14 , 1 , 1 );
    // 2B VCC
    pinsetshim( 15 , 1 , 1 );
    // 2Y MGND
    pinsetshim( 13 , 0 , 1 );
    





    // now a threshold test
    Serial.print( F("Gate A rise threshold: (Vih) " ) );

    // 1A VCC
    pinsetshim( 2 , 4 , 0 );
    // 1B VCC
    pinsetshim( 3 , 5 , 0 );
    // 1Y MGND
    pinsetshim( 1 , 2 , 0 );
    
    Wire.beginTransmission( TCA9548_I2C_ADDRESS );
    Wire.write( 0b00000001 );
    Wire.endTransmission();

    uint16_t curdacvalue = 0;
    dacarray[0].setval(curdacvalue);
    dacarray[1].setval(curdacvalue);
    delay(1);
    while ( multimeter[1].getbusvoltage() > 2 ) {
      curdacvalue++;
      dacarray[0].setval(curdacvalue);
      dacarray[1].setval(curdacvalue);
      delay(1);
    }

    Serial.println( multimeter[2].getbusvoltage() );

    Serial.print( "Vol " );
    Serial.println( multimeter[1].getbusvoltage() );

    // 1A VCC
    pinsetshim( 2 , 1 , 1 );
    // 1B VCC
    pinsetshim( 3 , 1 , 1 );
    // 1Y GND
    pinsetshim( 1 , 0 , 1 );
    




    // now a threshold test
    Serial.print( F("Gate A fall threshold: (Vil) " ) );

    // 1A VCC
    pinsetshim( 2 , 4 , 0 );
    // 1B VCC
    pinsetshim( 3 , 5 , 0 );
    // 1Y MGND
    pinsetshim( 1 , 2 , 0 );
    
    Wire.beginTransmission( TCA9548_I2C_ADDRESS );
    Wire.write( 0b00000001 );
    Wire.endTransmission();

    curdacvalue = 4095;
    dacarray[0].setval(curdacvalue);
    dacarray[1].setval(curdacvalue);
    delay(1);
    while ( multimeter[1].getbusvoltage() < 2 ) {
      curdacvalue--;
      dacarray[0].setval(curdacvalue);
      dacarray[1].setval(curdacvalue);
      delay(1);
    }

    Serial.println( multimeter[2].getbusvoltage() );
    
    Serial.print( "Voh " );
    Serial.println( multimeter[1].getbusvoltage() );


        // 1A VCC
    pinsetshim( 2 , 1 , 1 );
    // 1B VCC
    pinsetshim( 3 , 1 , 1 );
    // 1Y GND
    pinsetshim( 1 , 0 , 1 );
    
  }

}

*/






/*

███████╗██╗  ██╗ ██████╗ ██╗  ██╗         
╚════██║██║  ██║██╔═████╗██║  ██║         
    ██╔╝███████║██║██╔██║███████║         
   ██╔╝ ╚════██║████╔╝██║╚════██║         
   ██║       ██║╚██████╔╝     ██║         
   ╚═╝       ╚═╝ ╚═════╝      ╚═╝         
                                          
███████╗████████╗ █████╗ ██████╗ ████████╗
██╔════╝╚══██╔══╝██╔══██╗██╔══██╗╚══██╔══╝
███████╗   ██║   ███████║██████╔╝   ██║   
╚════██║   ██║   ██╔══██║██╔══██╗   ██║   
███████║   ██║   ██║  ██║██║  ██║   ██║   
╚══════╝   ╚═╝   ╚═╝  ╚═╝╚═╝  ╚═╝   ╚═╝   
                                          
*/



void command_7404_test( String &commandline ) { return; }

/*

void command_7404_test( String &commandline ) {
  
  commandline = commandline.substring( 5 );

  if ( commandline.equals("") ) { commandline = "show"; }

  if ( commandline.startsWith("show") ) {

    Serial.println(F("7404 Series Logic."));
    Serial.println();
    Serial.println(F("Hex inverter NOT gate."));
    Serial.println();
    Serial.println(F("         +----u----+         "));
    Serial.println(F(" 1.  1A -| 1    14 |- VCC .16"));
    Serial.println(F(" 2.  1Y -| 2    13 |- 6A  .15"));
    Serial.println(F(" 3.  2A -| 3    12 |- 6Y  .14"));
    Serial.println(F(" 4.  2Y -| 4    11 |- 5A  .13"));
    Serial.println(F(" 5.  3A -| 5    10 |- 5Y  .12"));
    Serial.println(F(" 6.  3Y -| 6    9  |- 4A  .11"));
    Serial.println(F(" 7. GND -| 7    8  |- 4Y  .10"));
    Serial.println(F(" 8.      +---------+      .9 "));
    Serial.println();
    
  }

  if ( commandline.startsWith("test") ) {

    Serial.println(F("7404 Series Logic."));
    Serial.println();
    Serial.println(F("Hex inverter NOT gate."));
    Serial.println();
    Serial.println(F("         +----u----+         "));
    Serial.println(F(" 1.  1A -| 1    14 |- VCC .16"));
    Serial.println(F(" 2.  1Y -| 2    13 |- 6A  .15"));
    Serial.println(F(" 3.  2A -| 3    12 |- 6Y  .14"));
    Serial.println(F(" 4.  2Y -| 4    11 |- 5A  .13"));
    Serial.println(F(" 5.  3A -| 5    10 |- 5Y  .12"));
    Serial.println(F(" 6.  3Y -| 6    9  |- 4A  .11"));
    Serial.println(F(" 7. GND -| 7    8  |- 4Y  .10"));
    Serial.println(F(" 8.      +---------+      .9 "));
    Serial.println();



    // configure all the pins
    
    String commandstring = "";

    // 1A VCC
    pinsetshim( 1 , 1 , 0 );
    // 1Y GND
    pinsetshim( 2 , 0 , 0 );
    // 2A VCC
    pinsetshim( 3 , 1 , 0 );
    // 2Y GND
    pinsetshim( 4 , 0 , 0 );
    // 3A VCC
    pinsetshim( 5 , 1 , 0 );
    // 3Y GND
    pinsetshim( 6 , 0 , 0 );
    // 4A VCC
    pinsetshim( 11 , 1 , 0 );
    // 4Y GND
    pinsetshim( 10 , 0 , 0 );
    // 5A VCC
    pinsetshim( 13 , 1 , 0 );
    // 5Y GND
    pinsetshim( 12 , 0 , 0 );
    // 6A VCC
    pinsetshim( 15 , 1 , 0 );
    // 6Y GND
    pinsetshim( 14 , 0 , 0 );
    // GND
    pinsetshim( 7 , 0 , 0 );
    // VCC
    pinsetshim( 16 , 1 , 0 );
    
    Serial.println( F("Bus configured!") );

    Serial.println( F("N A Y Result" ));
    
    // connect gate A to measurement probes
    pinsetshim( 1 , 4 , 0 );
    pinsetshim( 2 , 2 , 0 );
    
    // now switch to test and measurement bank
    Wire.beginTransmission( TCA9548_I2C_ADDRESS );
    Wire.write( 0b00000001 );
    Wire.endTransmission();

    // A Y
    // 0 1
    Serial.print( F("1 0 1 ") );
    dacarray[0].setval(0);
    delay(1);
    if ( multimeter[1].getbusvoltage() > 2 ) {
      showpass();
    }
    else {
      showfail();
    }
    
    
    // 1 0
    Serial.print( F("1 1 0 ") );
    dacarray[0].setval(4095);
    delay(1);
    if ( multimeter[1].getbusvoltage() < 2 ) {
      showpass();
    }
    else {
      showfail();
    }

    // connect gate A to power rails
    pinsetshim( 1 , 1 , 0 );
    pinsetshim( 2 , 0 , 0 );
    




    // A
    pinsetshim( 3 , 4 , 0 );
    // Y
    pinsetshim( 4 , 2 , 0 );
    
    // now switch to test and measurement bank
    Wire.beginTransmission( TCA9548_I2C_ADDRESS );
    Wire.write( 0b00000001 );
    Wire.endTransmission();

    // A Y
    // 0 1
    Serial.print( F("2 0 1 ") );
    dacarray[0].setval(0);
    delay(1);
    if ( multimeter[1].getbusvoltage() > 2 ) {
      showpass();
    }
    else {
      showfail();
    }
    
    
    // 1 0
    Serial.print( F("2 1 0 ") );
    dacarray[0].setval(4095);
    delay(1);
    if ( multimeter[1].getbusvoltage() < 2 ) {
      showpass();
    }
    else {
      showfail();
    }

    // A
    pinsetshim( 3 , 1 , 0 );
    // Y
    pinsetshim( 4 , 0 , 0 );







    
    // A
    pinsetshim( 5 , 4 , 0 );
    // Y
    pinsetshim( 6 , 2 , 0 );

    // now switch to test and measurement bank
    Wire.beginTransmission( TCA9548_I2C_ADDRESS );
    Wire.write( 0b00000001 );
    Wire.endTransmission();

    // check basic functionality
    
    // A Y
    // 0 1
    Serial.print( F("3 0 1 ") );
    dacarray[0].setval(0);
    delay(1);
    if ( multimeter[1].getbusvoltage() > 2 ) {
      showpass();
    }
    else {
      showfail();
    }
    
    
    // 1 0
    Serial.print( F("3 1 0 ") );
    dacarray[0].setval(4095);
    delay(1);
    if ( multimeter[1].getbusvoltage() < 2 ) {
      showpass();
    }
    else {
      showfail();
    }

    // A
    pinsetshim( 5 , 1 , 0 );
    // Y
    pinsetshim( 6 , 0 , 0 );






    // A
    pinsetshim( 11 , 4 , 0 );
    // Y
    pinsetshim( 10 , 2 , 0 );

    // now switch to test and measurement bank
    Wire.beginTransmission( TCA9548_I2C_ADDRESS );
    Wire.write( 0b00000001 );
    Wire.endTransmission();

    
    // A Y
    // 0 1
    Serial.print( F("4 0 1 ") );
    dacarray[0].setval(0);
    delay(1);
    if ( multimeter[1].getbusvoltage() > 2 ) {
      showpass();
    }
    else {
      showfail();
    }
    
    
    // 1 0
    Serial.print( F("4 1 0 ") );
    dacarray[0].setval(4095);
    delay(1);
    if ( multimeter[1].getbusvoltage() < 2 ) {
      showpass();
    }
    else {
      showfail();
    }

    // A
    pinsetshim( 11 , 1 , 0 );
    // Y
    pinsetshim( 10 , 0 , 0 );




    
    // A
    pinsetshim( 13 , 4 , 0 );
    // Y
    pinsetshim( 12 , 2 , 0 );

    // now switch to test and measurement bank
    Wire.beginTransmission( TCA9548_I2C_ADDRESS );
    Wire.write( 0b00000001 );
    Wire.endTransmission();

    
    // A Y
    // 0 1
    Serial.print( F("5 0 1 ") );
    dacarray[0].setval(0);
    delay(1);
    if ( multimeter[1].getbusvoltage() > 2 ) {
      showpass();
    }
    else {
      showfail();
    }
    
    
    // 1 0
    Serial.print( F("5 1 0 ") );
    dacarray[0].setval(4095);
    delay(1);
    if ( multimeter[1].getbusvoltage() < 2 ) {
      showpass();
    }
    else {
      showfail();
    }

    // A
    pinsetshim( 13 , 1 , 0 );
    // Y
    pinsetshim( 12 , 0 , 0 );









    
    // A
    pinsetshim( 15 , 4 , 0 );
    // Y
    pinsetshim( 14 , 2 , 0 );

    // now switch to test and measurement bank
    Wire.beginTransmission( TCA9548_I2C_ADDRESS );
    Wire.write( 0b00000001 );
    Wire.endTransmission();

    // A Y
    // 0 1
    Serial.print( F("6 0 1 ") );
    dacarray[0].setval(0);
    delay(1);
    if ( multimeter[1].getbusvoltage() > 2 ) {
      showpass();
    }
    else {
      showfail();
    }
    
    
    // 1 0
    Serial.print( F("6 1 0 ") );
    dacarray[0].setval(4095);
    delay(1);
    if ( multimeter[1].getbusvoltage() < 2 ) {
      showpass();
    }
    else {
      showfail();
    }

    // A
    pinsetshim( 15 , 1 , 0 );
    // Y
    pinsetshim( 14 , 0 , 0 );

    dacarray[0].setval(0);
    dacarray[1].setval(0);
    







    // now find vih
    // gate 1
    pinsetshim( 1 , 4 , 0 );
    pinsetshim( 2 , 2 , 0 );

    Serial.print( "vih " );
    Serial.print( findvih(0) );

    Serial.println();

    command_measure();




    // now find vih
    // gate 1
    pinsetshim( 1 , 4 , 0 );
    pinsetshim( 2 , 2 , 0 );

    Serial.print( "vil " );
    Serial.print( findvil(1) );

    Serial.println();

    command_measure();












    return;

  }

}

*/







/*

███████╗██╗  ██╗ ██████╗  █████╗          
╚════██║██║  ██║██╔═████╗██╔══██╗         
    ██╔╝███████║██║██╔██║╚█████╔╝         
   ██╔╝ ╚════██║████╔╝██║██╔══██╗         
   ██║       ██║╚██████╔╝╚█████╔╝         
   ╚═╝       ╚═╝ ╚═════╝  ╚════╝          
                                          
███████╗████████╗ █████╗ ██████╗ ████████╗
██╔════╝╚══██╔══╝██╔══██╗██╔══██╗╚══██╔══╝
███████╗   ██║   ███████║██████╔╝   ██║   
╚════██║   ██║   ██╔══██║██╔══██╗   ██║   
███████║   ██║   ██║  ██║██║  ██║   ██║   
╚══════╝   ╚═╝   ╚═╝  ╚═╝╚═╝  ╚═╝   ╚═╝   

*/



void command_7408_test( String &commandline ) { return; }
  
/*

void gate_test_and( uint16_t indexnumber ) {

  
  i2cbusswitcher.setbank( I2C_BUS_MEASURE );

  Serial.println( "G A B Y Result");

  // 0 0 0
  Serial.print( indexnumber );
  Serial.print( " 0 0 0 " );

  dacarray[0].setval(0);
  dacarray[1].setval(0);
  delay(1);
  if ( multimeter[1].getbusvoltage() < (float)1 ) {
    Serial.println( "pass!" );
  }
  else {
    Serial.println( "Fail" );
    command_measure();
    while(1);
  }

  // 0 1 0
  Serial.print( indexnumber );
  Serial.print( " 0 1 0 " );

  dacarray[0].setval(0);
  dacarray[1].setval(4095);
  delay(1);
  if ( multimeter[1].getbusvoltage() < 1 ) {
    Serial.println( "pass!" );
  }
  else {
    Serial.println( "Fail" );
    command_measure();
    while(1);
  }

  // 1 0 0
  Serial.print( indexnumber );
  Serial.print( " 1 0 0 " );

  dacarray[0].setval(4095);
  dacarray[1].setval(0);
  delay(1);
  if ( multimeter[1].getbusvoltage() < 1 ) {
    Serial.println( "pass!" );
  }
  else {
    Serial.println( "Fail" );
    command_measure();
    while(1);
  }

  // 1 1 1
  Serial.print( indexnumber );
  Serial.print( " 1 1 1 " );

  dacarray[0].setval(4095);
  dacarray[1].setval(4095);
  delay(1);
  if ( multimeter[1].getbusvoltage() > 1 ) {
    Serial.println( "pass!" );
  }
  else {
    Serial.println( "Fail" );
    command_measure();
    while(1);
  }

}

void command_7408_test( String &commandline ) {
  
  commandline = commandline.substring( 5 );

  if ( commandline.equals("") ) { commandline = "show"; }

  if ( commandline.startsWith("show") ) {

    Serial.println(F("7408 Series Logic."));
    Serial.println();
    Serial.println(F("Quad 2 input AND."));
    Serial.println();
    Serial.println(F("         +----u----+         "));
    Serial.println(F(" 1.  1A -| 1    14 |- VCC .16"));
    Serial.println(F(" 2.  1B -| 2    13 |- 4B  .15"));
    Serial.println(F(" 3.  1Y -| 3    12 |- 4A  .14"));
    Serial.println(F(" 4.  2A -| 4    11 |- 4Y  .13"));
    Serial.println(F(" 5.  2B -| 5    10 |- 3B  .12"));
    Serial.println(F(" 6.  2Y -| 6    9  |- 3A  .11"));
    Serial.println(F(" 7. GND -| 7    8  |- 3Y  .10"));
    Serial.println(F(" 8.      +---------+      .9 "));
    Serial.println();
    
  }

  if ( commandline.startsWith("test") ) {
    String templine = "show";
    command_7408_test( templine );

    // 1A
    pinsetshim( 1 , BUS_GND , BUS_CONNECTED );
    // 1B
    pinsetshim( 2 , BUS_GND , BUS_CONNECTED );
    // 1Y
    pinsetshim( 3 , BUS_GND , BUS_CONNECTED );
    // 2A
    pinsetshim( 4 , BUS_GND , BUS_CONNECTED );
    // 2B
    pinsetshim( 5 , BUS_GND , BUS_CONNECTED );
    // 2Y
    pinsetshim( 6 , BUS_GND , BUS_CONNECTED );
    // 3A
    pinsetshim( 11 , BUS_GND , BUS_CONNECTED );
    // 3B
    pinsetshim( 12 , BUS_GND , BUS_CONNECTED );
    // 3Y
    pinsetshim( 10 , BUS_GND , BUS_CONNECTED );
    // 4A
    pinsetshim( 14 , BUS_GND , BUS_CONNECTED );
    // 4B
    pinsetshim( 15 , BUS_GND , BUS_CONNECTED );
    // 4Y
    pinsetshim( 13 , BUS_GND , BUS_CONNECTED);
    
    // GND
    pinsetshim( 7 , BUS_GND , BUS_CONNECTED );
    // VCC
    pinsetshim( 16 , BUS_MVCC , BUS_CONNECTED );
    

    // connect gate 1 to tester
    pinsetshim( 1 , BUS_VMVCC0 , BUS_CONNECTED );
    pinsetshim( 2 , BUS_VMVCC1 , BUS_CONNECTED );
    pinsetshim( 3 , BUS_MGND , BUS_CONNECTED );

    gate_test_and(1);
    
    pinsetshim( 1 , BUS_GND , BUS_CONNECTED );
    pinsetshim( 2 , BUS_GND , BUS_CONNECTED );
    pinsetshim( 3 , BUS_GND , BUS_CONNECTED );

    // connect gate 2 to tester
    pinsetshim( 4 , BUS_VMVCC0 , BUS_CONNECTED );
    pinsetshim( 5 , BUS_VMVCC1 , BUS_CONNECTED );
    pinsetshim( 6 , BUS_MGND , BUS_CONNECTED );

    gate_test_and(2);
    
    pinsetshim( 4 , BUS_GND , BUS_CONNECTED );
    pinsetshim( 5 , BUS_GND , BUS_CONNECTED );
    pinsetshim( 6 , BUS_GND , BUS_CONNECTED );

    // connect gate 3 to tester
    pinsetshim( 11 , BUS_VMVCC0 , BUS_CONNECTED );
    pinsetshim( 12 , BUS_VMVCC1 , BUS_CONNECTED );
    pinsetshim( 10 , BUS_MGND , BUS_CONNECTED );

    gate_test_and(3);
    
    pinsetshim( 11 , BUS_GND , BUS_CONNECTED );
    pinsetshim( 12 , BUS_GND , BUS_CONNECTED );
    pinsetshim( 10 , BUS_GND , BUS_CONNECTED );

    // connect gate 4 to tester
    pinsetshim( 14 , BUS_VMVCC0 , BUS_CONNECTED );
    pinsetshim( 15 , BUS_VMVCC1 , BUS_CONNECTED );
    pinsetshim( 13 , BUS_MGND , BUS_CONNECTED );

    gate_test_and(4);
    
    pinsetshim( 14 , BUS_GND , BUS_CONNECTED );
    pinsetshim( 15 , BUS_GND , BUS_CONNECTED );
    pinsetshim( 13 , BUS_GND , BUS_CONNECTED );




    // Voh
    pinsetshim( 1 , BUS_VCC , BUS_CONNECTED );
    pinsetshim( 2 , BUS_VCC , BUS_CONNECTED );
    pinsetshim( 3 , BUS_MGND , BUS_CONNECTED );

    delay(1);
    i2cbusswitcher.setbank( I2C_BUS_MEASURE );
    delay(1);

    float voh = multimeter[1].getbusvoltage();

    pinsetshim( 1 , BUS_GND , BUS_CONNECTED );
    pinsetshim( 2 , BUS_GND , BUS_CONNECTED );
    pinsetshim( 3 , BUS_GND , BUS_CONNECTED );

    Serial.print( "Voh: " );
    Serial.print( voh );
    Serial.println( "V" );


    // Vol
    pinsetshim( 1 , BUS_GND , BUS_CONNECTED );
    pinsetshim( 2 , BUS_GND , BUS_CONNECTED );
    pinsetshim( 3 , BUS_MGND , BUS_CONNECTED );

    delay(1);
    i2cbusswitcher.setbank( I2C_BUS_MEASURE );
    delay(1);

    float vol = multimeter[1].getbusvoltage();

    pinsetshim( 1 , BUS_GND , BUS_CONNECTED );
    pinsetshim( 2 , BUS_GND , BUS_CONNECTED );
    pinsetshim( 3 , BUS_GND , BUS_CONNECTED );

    Serial.print( "Vol: " );
    Serial.print( vol );
    Serial.println( "V" );



    return;

  }

}

*/







/*

███████╗██╗  ██╗██████╗ ██████╗           
╚════██║██║  ██║╚════██╗╚════██╗          
    ██╔╝███████║ █████╔╝ █████╔╝          
   ██╔╝ ╚════██║ ╚═══██╗██╔═══╝           
   ██║       ██║██████╔╝███████╗          
   ╚═╝       ╚═╝╚═════╝ ╚══════╝          
                                          
███████╗████████╗ █████╗ ██████╗ ████████╗
██╔════╝╚══██╔══╝██╔══██╗██╔══██╗╚══██╔══╝
███████╗   ██║   ███████║██████╔╝   ██║   
╚════██║   ██║   ██╔══██║██╔══██╗   ██║   
███████║   ██║   ██║  ██║██║  ██║   ██║   
╚══════╝   ╚═╝   ╚═╝  ╚═╝╚═╝  ╚═╝   ╚═╝   
                                          

*/


void command_7432_test( String &commandline ) { return; }


/*

void gate_test_or( uint16_t indexnumber ) {
  
  i2cbusswitcher.setbank( I2C_BUS_MEASURE );

  Serial.println( "G A B Y Result");

  // 0 0 0
  Serial.print( indexnumber );
  Serial.print( " 0 0 0 " );

  dacarray[0].setval(0);
  dacarray[1].setval(0);
  delay(1);
  if ( multimeter[1].getbusvoltage() < (float)1 ) {
    Serial.println( "pass!" );
  }
  else {
    Serial.println( "Fail" );
    command_measure();
    while(1);
  }

  // 0 1 0
  Serial.print( indexnumber );
  Serial.print( " 0 1 1 " );

  dacarray[0].setval(0);
  dacarray[1].setval(4095);
  delay(1);
  if ( multimeter[1].getbusvoltage() > 1 ) {
    Serial.println( "pass!" );
  }
  else {
    Serial.println( "Fail" );
    command_measure();
    while(1);
  }

  // 1 0 0
  Serial.print( indexnumber );
  Serial.print( " 1 0 1 " );

  dacarray[0].setval(4095);
  dacarray[1].setval(0);
  delay(1);
  if ( multimeter[1].getbusvoltage() > 1 ) {
    Serial.println( "pass!" );
  }
  else {
    Serial.println( "Fail" );
    command_measure();
    while(1);
  }

  // 1 1 1
  Serial.print( indexnumber );
  Serial.print( " 1 1 1 " );

  dacarray[0].setval(4095);
  dacarray[1].setval(4095);
  delay(1);
  if ( multimeter[1].getbusvoltage() > 1 ) {
    Serial.println( "pass!" );
  }
  else {
    Serial.println( "Fail" );
    command_measure();
    while(1);
  }

}

void command_7432_test( String &commandline ) {

  commandline = commandline.substring( 5 );

  if ( commandline.equals("") ) { commandline = "show"; }

  if ( commandline.startsWith("show") ) {

    Serial.println(F("7432 Series Logic."));
    Serial.println();
    Serial.println(F("Quad 2 input OR."));
    Serial.println();
    Serial.println(F("         +----u----+         "));
    Serial.println(F(" 1.  1A -| 1    14 |- VCC .16"));
    Serial.println(F(" 2.  1B -| 2    13 |- 4B  .15"));
    Serial.println(F(" 3.  1Y -| 3    12 |- 4A  .14"));
    Serial.println(F(" 4.  2A -| 4    11 |- 4Y  .13"));
    Serial.println(F(" 5.  2B -| 5    10 |- 3B  .12"));
    Serial.println(F(" 6.  2Y -| 6    9  |- 3A  .11"));
    Serial.println(F(" 7. GND -| 7    8  |- 3Y  .10"));
    Serial.println(F(" 8.      +---------+      .9 "));
    Serial.println();
    
  }

  if ( commandline.startsWith("test") ) {
    String templine = "show";
    command_7432_test( templine );

    // configure pins
    // 1A
    pinsetshim( 1 , BUS_GND , BUS_CONNECTED );
    // 1B
    pinsetshim( 2 , BUS_GND , BUS_CONNECTED );
    // 1Y
    pinsetshim( 3 , BUS_GND , BUS_CONNECTED );
    // 2A
    pinsetshim( 4 , BUS_GND , BUS_CONNECTED );
    // 2B
    pinsetshim( 5 , BUS_GND , BUS_CONNECTED );
    // 2Y
    pinsetshim( 6 , BUS_GND , BUS_CONNECTED );
    // 3A
    pinsetshim( 11 , BUS_GND , BUS_CONNECTED );
    // 3B
    pinsetshim( 12 , BUS_GND , BUS_CONNECTED );
    // 3Y
    pinsetshim( 10 , BUS_GND , BUS_CONNECTED );
    // 4A
    pinsetshim( 14 , BUS_GND , BUS_CONNECTED );
    // 4B
    pinsetshim( 15 , BUS_GND , BUS_CONNECTED );
    // 4Y
    pinsetshim( 13 , BUS_GND , BUS_CONNECTED );

    // VCC
    pinsetshim( 16 , BUS_MVCC , BUS_CONNECTED );
    // GND
    pinsetshim( 7 , BUS_GND , BUS_CONNECTED );



    // connect gate 1 to tester
    pinsetshim( 1 , BUS_VMVCC0 , BUS_CONNECTED );
    pinsetshim( 2 , BUS_VMVCC1 , BUS_CONNECTED );
    pinsetshim( 3 , BUS_MGND , BUS_CONNECTED );

    gate_test_or(1);
    
    pinsetshim( 1 , BUS_GND , BUS_CONNECTED );
    pinsetshim( 2 , BUS_GND , BUS_CONNECTED );
    pinsetshim( 3 , BUS_GND , BUS_CONNECTED );

    // connect gate 2 to tester
    pinsetshim( 4 , BUS_VMVCC0 , BUS_CONNECTED );
    pinsetshim( 5 , BUS_VMVCC1 , BUS_CONNECTED );
    pinsetshim( 6 , BUS_MGND , BUS_CONNECTED );

    gate_test_or(2);
    
    pinsetshim( 4 , BUS_GND , BUS_CONNECTED );
    pinsetshim( 5 , BUS_GND , BUS_CONNECTED );
    pinsetshim( 6 , BUS_GND , BUS_CONNECTED );

    // connect gate 3 to tester
    pinsetshim( 11 , BUS_VMVCC0 , BUS_CONNECTED );
    pinsetshim( 12 , BUS_VMVCC1 , BUS_CONNECTED );
    pinsetshim( 10 , BUS_MGND , BUS_CONNECTED );

    gate_test_or(3);
    
    pinsetshim( 11 , BUS_GND , BUS_CONNECTED );
    pinsetshim( 12 , BUS_GND , BUS_CONNECTED );
    pinsetshim( 10 , BUS_GND , BUS_CONNECTED );

    // connect gate 4 to tester
    pinsetshim( 14 , BUS_VMVCC0 , BUS_CONNECTED );
    pinsetshim( 15 , BUS_VMVCC1 , BUS_CONNECTED );
    pinsetshim( 13 , BUS_MGND , BUS_CONNECTED );

    gate_test_or(4);
    
    pinsetshim( 14 , BUS_GND , BUS_CONNECTED );
    pinsetshim( 15 , BUS_GND , BUS_CONNECTED );
    pinsetshim( 13 , BUS_GND , BUS_CONNECTED );




    // Voh
    pinsetshim( 1 , BUS_VCC , BUS_CONNECTED );
    pinsetshim( 2 , BUS_VCC , BUS_CONNECTED );
    pinsetshim( 3 , BUS_MGND , BUS_CONNECTED );

    delay(1);
    i2cbusswitcher.setbank( I2C_BUS_MEASURE );
    delay(1);

    Serial.print( "Voh: " );
    Serial.print( multimeter[1].getbusvoltage() );
    Serial.println( "V" );

    pinsetshim( 1 , BUS_GND , BUS_CONNECTED );
    pinsetshim( 2 , BUS_GND , BUS_CONNECTED );
    pinsetshim( 3 , BUS_GND , BUS_CONNECTED );




    // Vol
    pinsetshim( 1 , BUS_GND , BUS_CONNECTED );
    pinsetshim( 2 , BUS_GND , BUS_CONNECTED );
    pinsetshim( 3 , BUS_MGND , BUS_CONNECTED );

    delay(1);
    i2cbusswitcher.setbank( I2C_BUS_MEASURE );
    delay(1);

    Serial.print( "Vol: " );
    Serial.print( multimeter[1].getbusvoltage() );
    Serial.println( "V" );
    
    pinsetshim( 1 , BUS_GND , BUS_CONNECTED );
    pinsetshim( 2 , BUS_GND , BUS_CONNECTED );
    pinsetshim( 3 , BUS_GND , BUS_CONNECTED );

    



  }



}

*/
















/*

███████╗██╗  ██╗ █████╗  ██████╗          
╚════██║██║  ██║██╔══██╗██╔════╝          
    ██╔╝███████║╚█████╔╝███████╗          
   ██╔╝ ╚════██║██╔══██╗██╔═══██╗         
   ██║       ██║╚█████╔╝╚██████╔╝         
   ╚═╝       ╚═╝ ╚════╝  ╚═════╝          
                                          
███████╗████████╗ █████╗ ██████╗ ████████╗
██╔════╝╚══██╔══╝██╔══██╗██╔══██╗╚══██╔══╝
███████╗   ██║   ███████║██████╔╝   ██║   
╚════██║   ██║   ██╔══██║██╔══██╗   ██║   
███████║   ██║   ██║  ██║██║  ██║   ██║   
╚══════╝   ╚═╝   ╚═╝  ╚═╝╚═╝  ╚═╝   ╚═╝   
                                          

*/


void command_7486_test( String &commandline ) { return; }

/*

void gate_test_xor( uint16_t indexnumber ) {
  
  i2cbusswitcher.setbank( I2C_BUS_MEASURE );

  Serial.println( "G A B Y Result");

  // 0 0 0
  Serial.print( indexnumber );
  Serial.print( " 0 0 0 " );

  dacarray[0].setval(0);
  dacarray[1].setval(0);
  delay(1);
  if ( multimeter[1].getbusvoltage() < (float)1 ) {
    Serial.println( "pass!" );
  }
  else {
    Serial.println( "Fail" );
    command_measure();
    while(1);
  }

  // 0 1 1
  Serial.print( indexnumber );
  Serial.print( " 0 1 1 " );

  dacarray[0].setval(0);
  dacarray[1].setval(4095);
  delay(1);
  if ( multimeter[1].getbusvoltage() > 1 ) {
    Serial.println( "pass!" );
  }
  else {
    Serial.println( "Fail" );
    command_measure();
    while(1);
  }

  // 1 0 1
  Serial.print( indexnumber );
  Serial.print( " 1 0 1 " );

  dacarray[0].setval(4095);
  dacarray[1].setval(0);
  delay(1);
  if ( multimeter[1].getbusvoltage() > 1 ) {
    Serial.println( "pass!" );
  }
  else {
    Serial.println( "Fail" );
    command_measure();
    while(1);
  }

  // 1 1 0
  Serial.print( indexnumber );
  Serial.print( " 1 1 0 " );

  dacarray[0].setval(4095);
  dacarray[1].setval(4095);
  delay(1);
  if ( multimeter[1].getbusvoltage() < 1 ) {
    Serial.println( "pass!" );
  }
  else {
    Serial.println( "Fail" );
    command_measure();
    while(1);
  }

}


void command_7486_test( String &commandline ) {

  commandline = commandline.substring( 5 );

  if ( commandline.equals("") ) { commandline = "show"; }

  if ( commandline.startsWith("show") ) {

    Serial.println(F("7486 Series Logic."));
    Serial.println();
    Serial.println(F("Quad 2 input XOR."));
    Serial.println();
    Serial.println(F("         +----u----+         "));
    Serial.println(F(" 1.  1A -| 1    14 |- VCC .16"));
    Serial.println(F(" 2.  1B -| 2    13 |- 4B  .15"));
    Serial.println(F(" 3.  1Y -| 3    12 |- 4A  .14"));
    Serial.println(F(" 4.  2A -| 4    11 |- 4Y  .13"));
    Serial.println(F(" 5.  2B -| 5    10 |- 3B  .12"));
    Serial.println(F(" 6.  2Y -| 6    9  |- 3A  .11"));
    Serial.println(F(" 7. GND -| 7    8  |- 3Y  .10"));
    Serial.println(F(" 8.      +---------+      .9 "));
    Serial.println();
    
  }

  if ( commandline.startsWith("test") ) {
    String templine = "show";
    command_7486_test( templine );

    // configure pins
    // 1A
    pinsetshim( 1 , BUS_GND , BUS_CONNECTED );
    // 1B
    pinsetshim( 2 , BUS_GND , BUS_CONNECTED );
    // 1Y
    pinsetshim( 3 , BUS_GND , BUS_CONNECTED );
    // 2A
    pinsetshim( 4 , BUS_GND , BUS_CONNECTED );
    // 2B
    pinsetshim( 5 , BUS_GND , BUS_CONNECTED );
    // 2Y
    pinsetshim( 6 , BUS_GND , BUS_CONNECTED );
    // 3A
    pinsetshim( 11 , BUS_GND , BUS_CONNECTED );
    // 3B
    pinsetshim( 12 , BUS_GND , BUS_CONNECTED );
    // 3Y
    pinsetshim( 10 , BUS_GND , BUS_CONNECTED );
    // 4A
    pinsetshim( 14 , BUS_GND , BUS_CONNECTED );
    // 4B
    pinsetshim( 15 , BUS_GND , BUS_CONNECTED );
    // 4Y
    pinsetshim( 13 , BUS_GND , BUS_CONNECTED );

    // VCC
    pinsetshim( 16 , BUS_MVCC , BUS_CONNECTED );
    // GND
    pinsetshim( 7 , BUS_GND , BUS_CONNECTED );



    // connect gate 1 to tester
    pinsetshim( 1 , BUS_VMVCC0 , BUS_CONNECTED );
    pinsetshim( 2 , BUS_VMVCC1 , BUS_CONNECTED );
    pinsetshim( 3 , BUS_MGND , BUS_CONNECTED );

    gate_test_xor(1);
    
    pinsetshim( 1 , BUS_GND , BUS_CONNECTED );
    pinsetshim( 2 , BUS_GND , BUS_CONNECTED );
    pinsetshim( 3 , BUS_GND , BUS_CONNECTED );

    // connect gate 2 to tester
    pinsetshim( 4 , BUS_VMVCC0 , BUS_CONNECTED );
    pinsetshim( 5 , BUS_VMVCC1 , BUS_CONNECTED );
    pinsetshim( 6 , BUS_MGND , BUS_CONNECTED );

    gate_test_xor(2);
    
    pinsetshim( 4 , BUS_GND , BUS_CONNECTED );
    pinsetshim( 5 , BUS_GND , BUS_CONNECTED );
    pinsetshim( 6 , BUS_GND , BUS_CONNECTED );

    // connect gate 3 to tester
    pinsetshim( 11 , BUS_VMVCC0 , BUS_CONNECTED );
    pinsetshim( 12 , BUS_VMVCC1 , BUS_CONNECTED );
    pinsetshim( 10 , BUS_MGND , BUS_CONNECTED );

    gate_test_xor(3);
    
    pinsetshim( 11 , BUS_GND , BUS_CONNECTED );
    pinsetshim( 12 , BUS_GND , BUS_CONNECTED );
    pinsetshim( 10 , BUS_GND , BUS_CONNECTED );

    // connect gate 4 to tester
    pinsetshim( 14 , BUS_VMVCC0 , BUS_CONNECTED );
    pinsetshim( 15 , BUS_VMVCC1 , BUS_CONNECTED );
    pinsetshim( 13 , BUS_MGND , BUS_CONNECTED );

    gate_test_xor(4);
    
    pinsetshim( 14 , BUS_GND , BUS_CONNECTED );
    pinsetshim( 15 , BUS_GND , BUS_CONNECTED );
    pinsetshim( 13 , BUS_GND , BUS_CONNECTED );




    // Voh
    pinsetshim( 1 , BUS_VCC , BUS_CONNECTED );
    pinsetshim( 2 , BUS_GND , BUS_CONNECTED );
    pinsetshim( 3 , BUS_MGND , BUS_CONNECTED );

    delay(1);
    i2cbusswitcher.setbank( I2C_BUS_MEASURE );
    delay(1);

    Serial.print( "Voh: " );
    Serial.print( multimeter[1].getbusvoltage() );
    Serial.println( "V" );

    pinsetshim( 1 , BUS_GND , BUS_CONNECTED );
    pinsetshim( 2 , BUS_GND , BUS_CONNECTED );
    pinsetshim( 3 , BUS_GND , BUS_CONNECTED );




    // Vol
    pinsetshim( 1 , BUS_GND , BUS_CONNECTED );
    pinsetshim( 2 , BUS_GND , BUS_CONNECTED );
    pinsetshim( 3 , BUS_MGND , BUS_CONNECTED );

    delay(1);
    i2cbusswitcher.setbank( I2C_BUS_MEASURE );
    delay(1);

    Serial.print( "Vol: " );
    Serial.print( multimeter[1].getbusvoltage() );
    Serial.println( "V" );
    
    pinsetshim( 1 , BUS_GND , BUS_CONNECTED );
    pinsetshim( 2 , BUS_GND , BUS_CONNECTED );
    pinsetshim( 3 , BUS_GND , BUS_CONNECTED );
    
  }

}

*/



