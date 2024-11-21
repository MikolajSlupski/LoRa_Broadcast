#include <SPI.h>
#include <LoRa.h>

int counter = 0;

void setup() {
  Serial.begin(9600);
  Serial.println("Serial begin");

 // LoRa.setFrequency(868E6);
 // Serial.println("SetFrequency");

  LoRa.setPins(18, 23, 26);
  Serial.println("SetPin");

  LoRa.begin(868E6);
  Serial.println("LoraBegin");
  
  while (!Serial);
  Serial.println("after !while");

  Serial.println("LoRa Sender");

  if (!LoRa.begin(868E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  
  Serial.println("End Setup");
}

void loop() {
  while(1){
  Serial.print("Sending packet: ");
  Serial.println(counter);

  // send packet
  LoRa.beginPacket();
  LoRa.print("hello Szczepan Wariacie 69 xD GOMBAO 33 KURFO");
  Serial.print("hello Szczepan Wariacie 69 xD ");
  LoRa.print(counter);
  Serial.print(counter);
  LoRa.endPacket();

  counter++;

  delay(5000);
  }
}

