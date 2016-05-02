/*  Este codigo monitora 1 sensor DHT22, 2 sensores moisture sensor YL-69, um sensor AtlanticScientific ENV-TMP-D e um sensor DAVIS 6440 .
 *  O arduino Fio trabalha em sleep mode e acorda a cada nx8 segundos para coletar os dados dos sensores.
 *  Os dados são enviados via XBee serie 2 em modo API 2 no formato CSV
 */



#include "Arduino.h"
#include "Wire.h" 
#include "DHT.h"
#include <SoftwareSerial.h>      //we have to include the SoftwareSerial library for ENV-TMP-D sensor
#include <XBee.h>
#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/wdt.h>

#define rx 2                     //define what pin Rx from TMP-D is going to be.
#define tx 3                     //define what pin Tx to TMP-D is going to be.

SoftwareSerial myserial(rx, tx); //define how the soft serial port is going to work. 

// create the XBee object
XBee xbee = XBee();

uint8_t payload[64];
String stringPayload;

// SH + SL Address of receiving XBee
XBeeAddress64 addr64 = XBeeAddress64(0x0013a200, 0x40BD2590);
ZBTxRequest zbTx = ZBTxRequest(addr64, payload, sizeof(payload));
ZBTxStatusResponse txStatus = ZBTxStatusResponse();

// Data logging configuration for Sleep Mode
#define LOGGING_FREQ_SECONDS   8       // Seconds to wait before a new sensor reading is logged.
#define MAX_SLEEP_ITERATIONS   LOGGING_FREQ_SECONDS / 8  // Number of times to sleep (for 8 seconds) before
                                                         // a sensor reading is taken and sent to the server.
                                                         // Don't change this unless you also change the 
                                                         // watchdog timer configuration.
// Internal state used by the sketch.
int sleepIterations = 0;
volatile bool watchdogActivated = false;

String deviceName = "ArduinoFio" ; //define the name for this device used on MQTT tags
//dismodulortcString readTime ; //create a variable used to preserve POSIX time of each sensor read
//String readTime = "1491234560123" ; //create a variable used to preserve POSIX time of each sensor read
String readTime = "0" ; //create a variable used to preserve POSIX time of each sensor read

#define DHTPIN1 8       // what pin we're connected to DHT 1
#define DHTTYPE DHT22   // define wich type of DHT sensor is used (int this case DHT 22  (AM2302))
DHT dht1(DHTPIN1, DHTTYPE);   //create a object to DHT 1

int moisture6440_level = 0;
int soilMoisture_level_1 = 0;
int soilMoisture_level_2 = 0;


char tmp_data[20];                //we make a 20 byte character array to hold incoming data from the ENV-TMP-D. 
char computerdata[20];            //we make a 20 byte character array to hold incoming data from a pc/mac/other.
byte pc_debug=1;                  //if you would like to debug the ENV-TMP-D through the serial monitor(pc/mac/other). if not set this to 0. 
byte received_from_computer=0;     //we need to know how many characters have been received.                                 
byte received_from_sensor=0;       //we need to know how many characters have been received.
byte arduino_only=0;               //if you would like to operate the ENV-TMP-D with the arduino only and not use the serial monitor to send it commands set this to 1. The data will still come out on the serial monitor, so you can see it working.  
byte startup=0;                   //used to make sure the arduino takes over control of the ENV-TMP-D properly.
float float_tmp=0;                 //used to hold a floating point number that is the ENV-TMP-D. 
byte string_received=0;            //used to identify when we have received a string from the ENV-TMP-D  .


void setup(){  
  Serial.begin(19200);
  xbee.setSerial(Serial);
  myserial.begin(38400);     //enable the software serial port
  delay(150); // Power up,delay 150ms,until voltage is stable

  dht1.begin();
  
  // Setup the watchdog timer to run an interrupt which
  // wakes the Arduino from sleep every 8 seconds.
  // Note that the default behavior of resetting the Arduino
  // with the watchdog will be disabled.
  // This next section of code is timing critical, so interrupts are disabled.
  // See more details of how to change the watchdog in the ATmega328P datasheet
  // around page 50, Watchdog Timer.
  noInterrupts();
  // Set the watchdog reset bit in the MCU status register to 0.
  MCUSR &= ~(1<<WDRF);
  // Set WDCE and WDE bits in the watchdog control register.
  WDTCSR |= (1<<WDCE) | (1<<WDE);
  // Set watchdog clock prescaler bits to a value of 8 seconds.
  WDTCSR = (1<<WDP0) | (1<<WDP3);
  // Enable watchdog as interrupt only (no reset).
  WDTCSR |= (1<<WDIE);
  // Enable interrupts again.
  interrupts();
}
 

void loop(){
  if (watchdogActivated){ // Don't do anything unless the watchdog timer interrupt has fired.
    watchdogActivated = false;
    sleepIterations += 1; // Increase the count of sleep iterations and take a sensor
    if (sleepIterations >= MAX_SLEEP_ITERATIONS) {  // reading once the max number of iterations has been hit.
      sleepIterations = 0;  // Reset the number of sleep iterations.

      dht22();          //read DHT22
//      envTmpD();      //read ENV-TMP-D sensor
      soilMoisture();   //read moisture sensors
      moisture6440();   //read DAVIS 6440 moisture sensor
    }
  }
}
 
