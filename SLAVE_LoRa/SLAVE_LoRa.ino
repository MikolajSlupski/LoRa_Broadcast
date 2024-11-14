/*
**************Projekt do komunikacji po protokole LoRa*****************




*/

#include <SPI.h>
#include <LoRa.h>

void setup() {
  Serial.begin(9600);
  while (Serial == 0);
 
  LoRa.setPins(18, 23, 26);
  Serial.println("LoRa Receiver");


  if (LoRa.begin(868E6) == 0) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }


}

void loop() {
  // try to parse packet
  void ReciveData()
 
}

void SendData(){

}

void ReciveData(){
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    // received a packet
    Serial.print("Received packet '");

    // read packet
    while (LoRa.available()) {
      Serial.print((char)LoRa.read());
    }
    // send response
    void SendData();
    // print RSSI of packet
    Serial.print("' with RSSI ");
    Serial.println(LoRa.packetRssi());
    Serial.println(LoRa.packetSnr());
  }
}

