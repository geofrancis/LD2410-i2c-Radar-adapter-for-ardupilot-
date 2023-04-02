#include <Wire.h>
#include <ld2410.h>
#include <Arduino.h>
//Standard MaxsonarI2CXL address
#define I2C_SLAVE_ADDR  0x70  



    #define MONITOR_SERIAL Serial
    #define RADAR_SERIAL Serial1
    #define RADAR_RX_PIN 16
    #define RADAR_TX_PIN 17



ld2410 radar;

uint32_t lastReading = 0;
bool radarConnected = false;


uint32_t i = 0;


unsigned char data_buffer[4] = {0};
int distances = 0;
int distance = 0;
unsigned char CS;
uint8_t Index;
byte received;
int readings[numReadings]; 
int readIndex = 0; 
int total = 0;  
int average = 0; 


void readsonar(){     
 
 radar.read();
  if(radar.isConnected() && millis() - lastReading > 250)  //Report every 1000ms
  {
    lastReading = millis();
    if(radar.presenceDetected())
    {
      if(radar.stationaryTargetDetected())
      {
        Serial.print(F("Stationary target: "));
        Serial.print(radar.stationaryTargetDistance());
        Serial.print(F("cm energy:"));
        Serial.print(radar.stationaryTargetEnergy());
        Serial.print(' ');

        Serial.print(F("Moving target: "));
        Serial.print(radar.movingTargetDistance());
        Serial.print(F("cm energy:"));
        Serial.print(radar.movingTargetEnergy());
      }
  
    }
    else
    {
      Serial.println(F("No target"));
      distance = 0;
    }
  }
  distance = (radar.movingTargetDistance());
  if ((radar.movingTargetEnergy()) == (0)) {
    distance = (radar.stationaryTargetDistance());
    }  
    Serial.println();
}
    //look for i2c read read request
   void receiveEvent(int howMany) {
      while (Wire.available()) { // loop through all but the last
        {         
      received = Wire.read(); 
      if (received == 0x51)
      {
    }
  }
}
}


//send results over i2c
void requestEvent() 
{  
Wire.write (highByte(distance));
Wire.write (lowByte(distance));
}

void setup() {
 Serial.begin(115200);
 RADAR_SERIAL.begin(256000, SERIAL_8N1, RADAR_RX_PIN, RADAR_TX_PIN); //UART for monitoring the radar
 Wire.begin(I2C_SLAVE_ADDR);
 Wire.onReceive(receiveEvent); // register event
 Wire.onRequest(requestEvent);

    const uint8_t fixed_query[] = {0x55, 0x5A, 0x02, 0xD3, 0x84};
    const uint8_t continuous[] = {0xBA, 0xAB, 0x00, 0xF6, 0x00, 0x07, 0x00, 0x55, 0xBB};
    Serial1.write(fixed_query, sizeof(fixed_query));
    Serial1.write(continuous, sizeof(continuous));


 if(radar.begin(RADAR_SERIAL))
  {
    MONITOR_SERIAL.println(F("OK"));
    MONITOR_SERIAL.print(F("LD2410 firmware version: "));
    MONITOR_SERIAL.print(radar.firmware_major_version);
    MONITOR_SERIAL.print('.');
    MONITOR_SERIAL.print(radar.firmware_minor_version);
    MONITOR_SERIAL.print('.');
    MONITOR_SERIAL.println(radar.firmware_bugfix_version, HEX);
  }
  else
  {
    MONITOR_SERIAL.println(F("not connected"));
  }

 #if CONFIG_IDF_TARGET_ESP32
  char message[64];
  snprintf(message, 64, "%u Packets.", i++);
  Wire.slaveWrite((uint8_t *)message, strlen(message));
#endif


}
void loop() {
  readsonar();
}
