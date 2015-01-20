#include <SPI.h>

#include <RF24.h>
#include <nRF24L01.h>
#include <RF24_config.h>

#include "dht11.h"

const long int address = 0x66996699L;  // So that's 0x0066996699

static DHT11 dht(3);
static RF24 rf( /*ce */ 9, /*cs */ 10);

void setup()
{
    // init serial
    Serial.begin(9600);
    Serial.println("Humidity transmitter started");

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
    if (!dht.read(&humi, &temp)) {
        Serial.println("Fail!\n");
    } else {
        // format
        char buf[32];
        snprintf(buf, sizeof(buf), "RH=%d%%,T=%2d", humi, temp);

        // send it
        Serial.println(buf);
        rf.write(&buf, strlen(buf));
    }

    // wait a bit
    delay(1000);
}
