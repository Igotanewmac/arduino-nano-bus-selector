
#include <Arduino.h>


// i2c tools
#include <i2ctools.h>

i2ctools myi2ctoolsobj;


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






// command function declarations

void command_busconnect_parser( String &commandline );







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
  String commandline = Serial.readStringUntil( 0x0A );

  // discard the rest
  while ( Serial.available() ) { Serial.read(); }

  // repeat it
  Serial.print("<<: " );
  Serial.println( commandline );

  if ( commandline.startsWith( "test" ) ) { Serial.println("Testing 123" ); return; }


  // enumerate the i2c bus.
  if ( commandline.startsWith( "i2cscan" ) ) { myi2ctoolsobj.i2cscan(); return; }

  
  if ( commandline.startsWith( "busconnect" ) ) {
    command_busconnect_parser( commandline );
    return;
  }



  // done command processing
  Serial.println("Command not recognised.");
  
}








void showhex( uint8_t hexvalue ) {
  if ( hexvalue < 16 ) { Serial.print( 0 ); }
  Serial.print( hexvalue , HEX );
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


  Serial.println(commandline);

  Serial.println( commandline.length() );

}






















