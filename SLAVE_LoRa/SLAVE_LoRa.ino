/*
**************Projekt do komunikacji po protokole LoRa*****************




*/

#include <SPI.h>
#include <LoRa.h>
int counter = 0;
void setup() {
  Serial.begin(115200);
  Serial.println("Serial begin");
  while (Serial == 0);

  LoRa.setPins(18, 23, 26);
  Serial.println("SetPin");


  if (LoRa.begin(868E6) == 0) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  analogReadResolution(1); //ustawienie rozdzielczości pinu, w tym przypadku 1bit
  pinMode(A12, INPUT_PULLUP);
  Serial.println("End Setup");
}

void loop() {

  Serial.println(ReadPin());
  delay(1000);
}

void SendData(){
  Serial.print("Sending packet: ");
  // send packet
  LoRa.beginPacket();
  if(ReadPin()==1){
    LoRa.print("OPEN");
    Serial.print("OPEN");
  }
  else{
    LoRa.print("CLOSE");
    Serial.print("CLOSE");
  }
  Serial.print("hello Szczepan Wariacie 69 xD ");
  LoRa.endPacket();
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


int ReadPin(){
  int analogValue = analogRead(0);
  if(analogValue==1){
    Serial.println("OPEN");
    return 1;
  }
  else{
    Serial.println("CLOSE");
    return 0;
  }
}
