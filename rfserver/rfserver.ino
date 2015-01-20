#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

const long int address = 0x66996699L;   // So that's 0x0066996699

static RF24 rf( /*ce */ 9, /*cs */ 10);

void setup()
{
    Serial.begin(9600);
    Serial.println("Humidity receiver started");

    rf.begin();
    rf.setRetries(15, 15);
    rf.enableDynamicPayloads();
    rf.openReadingPipe(1, address);
    rf.setAutoAck(true);
    rf.startListening();
}

void loop()
{
    char buf[33];

    if (rf.available()) {
        boolean done = false;
        while (!done) {
            int len = rf.getDynamicPayloadSize();
            done = rf.read(buf, len);
            buf[len] = '\0';
            Serial.println(buf);
        }
    }
}
