/*
  LoRa Duplex communication

  Sends a message every half second, and polls continually
  for new incoming messages. Implements a one-byte addressing scheme,
  with 0xFF as the broadcast address.

  Uses readString() from Stream class to read payload. The Stream class'
  timeout may affect other functuons, like the radio's callback. For an

  created 28 April 2017
  by Tom Igoe
*/
#include <SPI.h>              // include libraries
#include <LoRa.h>

const int csPin = 7;          // LoRa radio chip select
const int resetPin = 6;       // LoRa radio reset
const int irqPin = 1;         // change for your board; must be a hardware interrupt pin

String outgoing;              // outgoing message

int msgCount = 0;            // count of outgoing messages
byte localAddress = 0xBB;     // address of this device
byte destination = 0xFF;      // destination to send to
long lastSendTime = 0;        // last send time
int interval = 2000;          // interval between sends
int PreviousId;

enum Status { ERROR, PASSED}; 

enum Status CheckStatus(int CurrentId){
  if (CurrentId-PreviousId>1) {
    PreviousId = CurrentId; 
    return PASSED; 
  } 
  else { 
    PreviousId = CurrentId;
    return ERROR; 
  } 
}

void setup() {
  Serial.begin(115200);
  Serial.println("Serial begin");
  while (Serial == 0);

  Serial.println("LoRa Duplex");

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

  Serial.println("LoRa init succeeded.");
}

void loop() {
  Send("Pakiet");
  onReceive(LoRa.parsePacket());
}


void Send(String message){
    if (millis() - lastSendTime > interval) {
    sendMessage(message);
    Serial.println("Sending " + message);
    lastSendTime = millis();            // timestamp the message
    interval = random(2000) + 1000;    // 2-3 seconds
  }
}



void sendMessage(String outgoing) {
  LoRa.beginPacket();                   // start packet
  LoRa.write(destination);              // add destination address
  LoRa.write(localAddress);             // add sender address
  LoRa.write(msgCount);                 // add message ID
  LoRa.write(outgoing.length());        // add payload length
  LoRa.print(outgoing);                 // add payload
  LoRa.endPacket();                     // finish packet and send it
  msgCount++;                           // increment message ID
}





void onReceive(int packetSize) {
  if (packetSize == 0) return;          // if there's no packet, return

  // read packet header bytes:
  int recipient = LoRa.read();          // recipient address
  byte sender = LoRa.read();            // sender address
  byte INcounter = LoRa.read();     // incoming msg ID
  byte incomingLength = LoRa.read();    // incoming msg length


  String incoming = "";

  while (LoRa.available()) {
    incoming += (char)LoRa.read();
  }

  if (incomingLength != incoming.length()) {   // check length for error
    Serial.println("error: message length does not match length");
    return;                             // skip rest of function
  }

  // if the recipient isn't this device or broadcast,
  if (recipient != localAddress && recipient != 0xFF) {
    Serial.println("This message is not for me.");
    return;                             // skip rest of function
  }

  // if message is for this device, or broadcast, print details:
  //Serial.println("Received from: 0x" + String(sender, HEX));
  //Serial.println("Sent to: 0x" + String(recipient, HEX));
  if(CheckStatus(INcounter) == PASSED){
    Serial.print("PASSED");
  }else{
    Serial.println();
    Serial.println();
    Serial.println();
    Serial.println();
    Serial.print("----------ERROR-----------");
    Serial.println();
    Serial.println();
    Serial.println();
    Serial.println();
  }

  Serial.print("Message ID: " + String(INcounter));
  Serial.print("---");
  Serial.print("Message length: " + String(incomingLength));
  Serial.print("---");
  Serial.print("Message: " + incoming);
  Serial.print("---");
  Serial.print("RSSI: " + String(LoRa.packetRssi()));
  Serial.print("---");
  Serial.print("Snr: " + String(LoRa.packetSnr()));
  Serial.println();    
}

