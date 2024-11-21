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
  pinMode(A14, OUTPUT);
}

void loop() {
  if(ReadPin()==0){
    SendData();
  }

  ReciveData();
}

void SendData(){
  int counter;
  counter++;
  Serial.print("Sending packet: ");
  // send packet
  LoRa.beginPacket();
  /*
  if(ReadPin()==1){
    LoRa.print("OPEN");
    Serial.print("OPEN");
  }
  else{
    LoRa.print("CLOSE");
    Serial.print("CLOSE");
  }
  */
  LoRa.print(counter);
  LoRa.print("hello Szczepan Wariacie 69 xD");
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
    return 1;
  }
  else{
    return 0;
  }
}

void WritePin(int variable){
  analogWrite(A14, variable);
}

