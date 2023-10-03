/**************************************************************
 *
 * This sketch obtains the GPS values and send at commands through the serial terminal.
 * 
 * For more information:
 * https://altronics.cl/modem-sim7600g
 * https://github.com/Xinyuan-LilyGO/LilyGO-T-SIM7000G/tree/master
 * 
 **************************************************************/

#include <Arduino.h>

// Select your modem
#define TINY_GSM_MODEM_SIM7600

// Set serial for debug console (to the Serial Monitor, default speed 115200)
#define SerialMon Serial
#define SerialAT Serial1

// See all AT commands, if wanted
//#define DUMP_AT_COMMANDS

// Define the serial console for debug prints, if needed
#define TINY_GSM_DEBUG SerialMon

// Pin definitions
#define MODEM_SLEEP          4
#define MODEM_TX             17
#define MODEM_RX             16

//baud rate value for SerialAT an SerialMON
#define BAUD_RATE 115200

#include <TinyGsmClient.h>

#ifdef DUMP_AT_COMMANDS
#include <StreamDebugger.h>
StreamDebugger debugger(SerialAT, SerialMon);
TinyGsm        modem(debugger);
#else
TinyGsm        modem(SerialAT);
#endif

//Set gps variables 
float lat2      = 0;
float lon2      = 0;
float speed2    = 0;
float alt2      = 0;
int   vsat2     = 0;
int   usat2     = 0;
float accuracy2 = 0;
int   year2     = 0;
int   month2    = 0;
int   day2      = 0;
int   hour2     = 0;
int   min2      = 0;
int   sec2      = 0;

void setup() 
{
  // Set console baud rate
  SerialMon.begin(BAUD_RATE);
  delay(10);
  
  //Set sleep pin 
  pinMode(MODEM_SLEEP, OUTPUT);
  digitalWrite(MODEM_SLEEP, LOW);

  // Set GSM module baud rate
  DBG("Wait...");
  SerialAT.begin(BAUD_RATE, SERIAL_8N1, MODEM_RX, MODEM_TX);
  delay(6000L);

  // Restart takes quite some time
  DBG("Initializing modem...");
  modem.restart();
  modem.init();
  delay(50000L);
}

void loop() 
{ 
  for (int8_t i = 15; i; i--) 
  {
    DBG("Requesting current GPS/GNSS/GLONASS location");
    if (modem.getGPS(&lat2, &lon2, &speed2, &alt2, &vsat2, &usat2, &accuracy2,
        &year2, &month2, &day2, &hour2, &min2, &sec2)) 
    {
      DBG("Latitude:", String(lat2, 8), "\tLongitude:", String(lon2, 8));
      DBG("Speed:", speed2, "\tAltitude:", alt2);
      DBG("Visible Satellites:", vsat2, "\tUsed Satellites:", usat2);
      DBG("Accuracy:", accuracy2);
      DBG("Year:", year2, "\tMonth:", month2, "\tDay:", day2);
      DBG("Hour:", hour2, "\tMinute:", min2, "\tSecond:", sec2);
    }
    delay(5000);
  }
  DBG("End of tests.");

  /* If you don't see GPS values, try automatically turning on GPS with the AT commands */

  DBG("Now, You can send AT command in the serial terminal.");
  /*
  Some example of AT commands
  For more information: https://altronics.cl/uploads/sim7600/SIM7500_SIM7600%20Series_AT%20Command%20Manual_V3.00.pdf
  Start GPS session:
    AT+CGPS=1
  Stop GPS session:
    AT+CGPS=0
  Status GPS:
    AT+CGPS?
  Get GNSS fixed position information:
    AT+CGNSSINFO
  Get GPS fixed position information:
    AT+CGPSINFO
  Start GPS automatic:
    AT+CGPSAUTO=1
  Stop GPS automatic:
    AT+CGPSAUTO=0
  */

  while (1) 
  {
    while (SerialAT.available()) 
    {
        SerialMon.write(SerialAT.read());
    }
    while (SerialMon.available()) 
    {
        SerialAT.write(SerialMon.read());
    }
  }
}

