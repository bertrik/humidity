#include <SPI.h>

#include <RF24.h>
#include <nRF24L01.h>
#include <RF24_config.h>

#include "dht11.h"

static long int address = 0x66996699L;  // So that's 0x0066996699

static DHT11 dht(2);
static RF24 rf(/*ce*/ 8, /*cs*/ 9);

void setup()
{
  // init serial
  Serial.begin(115200);
  // init DHT
  dht.init();
  // init RF24
  rf.begin();
  rf.setAutoAck(true);
  rf.setRetries(15, 15);
  rf.enableDynamicPayloads();
  rf.openWritingPipe(address);
}

void loop()
{
  // read humidity / temperature
  int humi, temp;
  dht.read(&humi, &temp);
  
  Serial.print("T=");
  Serial.print(temp);
  Serial.print(", ");
  Serial.print("H=");
  Serial.println(humi); 
 
  // send it
  uint8_t buf[7];
  buf[0] = 6;
  buf[1] = 'H';
  buf[2] = 'U';
  buf[3] = 'M';
  buf[4] = 'I';
  buf[5] = humi;
  buf[6] = temp;
  rf.write(&buf, sizeof(buf));
 
  // wait a bit
  delay(1000);
}

