
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
#define  BUSCONNECT_CHANNEL_0_I2C_ADDRESS 0x20
#define BUSCONNECT_CHANNEL_0_BYTE_ADDRESS 0
#define  BUSCONNECT_CHANNEL_1_I2C_ADDRESS 0x20
#define BUSCONNECT_CHANNEL_1_BYTE_ADDRESS 1
#define  BUSCONNECT_CHANNEL_2_I2C_ADDRESS 0x21
#define BUSCONNECT_CHANNEL_2_BYTE_ADDRESS 0
#define  BUSCONNECT_CHANNEL_3_I2C_ADDRESS 0x21
#define BUSCONNECT_CHANNEL_3_BYTE_ADDRESS 1




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






// command function declarations

String commandline = "";

void command_busconnect_parser( String &commandline );

void command_ina219_parser( String &commandline );

void command_mcp4725_parser( String &commandline );

void command_tca9548_parser( String &commandline );


void command_dump_registers();

void command_measure();







void setup() {
  // put your setup code here, to run once:


  // configure the bus connector object.

  // call bigin to initialise the object.
  mybusconnectobj.begin();

  // set hardware addresses.
  mybusconnectobj.setchanneladdresss( 0 , BUSCONNECT_CHANNEL_0_I2C_ADDRESS , BUSCONNECT_CHANNEL_0_BYTE_ADDRESS );
  mybusconnectobj.setchanneladdresss( 1 , BUSCONNECT_CHANNEL_1_I2C_ADDRESS , BUSCONNECT_CHANNEL_1_BYTE_ADDRESS );
  mybusconnectobj.setchanneladdresss( 2 , BUSCONNECT_CHANNEL_2_I2C_ADDRESS , BUSCONNECT_CHANNEL_2_BYTE_ADDRESS );
  mybusconnectobj.setchanneladdresss( 3 , BUSCONNECT_CHANNEL_3_I2C_ADDRESS , BUSCONNECT_CHANNEL_3_BYTE_ADDRESS );
  
  // set everything to off.
  mybusconnectobj.setchannelstate( 0 , 1 );
  mybusconnectobj.setchannelstate( 1 , 1 );
  mybusconnectobj.setchannelstate( 2 , 1 );
  mybusconnectobj.setchannelstate( 3 , 1 );

  // set all busses to highest entry.
  // we default to top because pcf defaults to on
  // so at power on it will devault to 0b111 = 7
  mybusconnectobj.setchannelinput( 0 , 7 );
  mybusconnectobj.setchannelinput( 1 , 7 );
  mybusconnectobj.setchannelinput( 2 , 7 );
  mybusconnectobj.setchannelinput( 3 , 7 );
  
  // the bus is now in a known safe state.
  // all channels are disabled
  // all channels are on default bus line 7



  // configure ina219's
  multimeter[0].begin( 0x40 );
  multimeter[1].begin( 0x41 );
  multimeter[2].begin( 0x44 );
  multimeter[3].begin( 0x45 );


  // turn on dacs
  dacarray[0].begin( 0x60 );
  dacarray[1].begin( 0x61 );



  // i2c busmaster!
  i2cbusmaster.begin( 0x70 );

  i2cbusmaster.setbank(0);




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
  if ( commandline.startsWith( "busconnect" ) ) { command_busconnect_parser( commandline ); return; }

  // ina219 command parser
  if ( commandline.startsWith( "ina219" ) ) { command_ina219_parser( commandline ); return; }

  if ( commandline.startsWith( "mcp4725" ) ) { command_mcp4725_parser( commandline ); return; }


  if ( commandline.startsWith( "tca9548" ) ) { command_tca9548_parser( commandline ); return; }



  // dump registers
  if ( commandline.startsWith( "dr" ) ) { command_dump_registers(); return; }

  // quick measure
  if ( commandline.startsWith( "mr" ) ) { command_measure(); return; }


  // done command processing
  Serial.println("Command not recognised.");
  
}








void command_tca9548_parser( String &commandline ) {
  commandline = commandline.substring( 8 );

  if ( commandline.startsWith( "get" ) ) {
    Serial.print( "I2cbus " );
    Serial.print( i2cbusmaster.getbank() );
    Serial.println( " is selected!" );
  }

  if ( commandline.startsWith( "set" ) ) {
    commandline = commandline.substring( 4 );
    uint8_t bankrequested = commandline.toInt();
    i2cbusmaster.setbank( bankrequested );
    Serial.print( "I2cbus " );
    Serial.print( bankrequested );
    Serial.println( " has been set" );

  }

}



void command_busconnect_parser( String &commandline ) {

  // clean up the input string
  commandline = commandline.substring( 11 );
  commandline.trim();
  commandline.toLowerCase();

  // handle empty string
  if (!commandline.length()) { Serial.println("busconnect loaded ok!" ); return; }


  // actually do stuff
  if ( commandline.startsWith("show") ) {
    // show the current bus setup
    Serial.println( "Chan\ti2c\tByte\tBus\tState" );

    for ( uint8_t i = 0 ; i < 4 ; i++ ) {
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

  Serial.println("Busconnect command not recognised!");

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



