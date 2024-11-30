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

#define TEST_MESSAGE "Wiadomosc Testowa"

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

int sending_flag;
int reciving_flag;
int test_flag = 0;
uint64_t r_time;

int counter = 0;
esp_timer_handle_t timer;  // Uchwyt timera, tworzenie, uruchamianie i zatrzymywanie

uint64_t startTime = 0;
uint64_t endTime = 0;

enum Status { ERROR, PASSED}; 

enum Status CheckID(int CurrentId){
  if (CurrentId-PreviousId>1) {
    PreviousId = CurrentId; 
    return ERROR; 
  } 
  else { 
    PreviousId = CurrentId;
    return PASSED; 
  } 
}

void IRAM_ATTR onTimer(void* arg) { //IRAM_ATTR - onTimer jest umieszczany w RAM
  Serial.println("---------ERROR_TIME_OUT---------");
}

 // Konfiguracja timera
  const esp_timer_create_args_t timer_args = {
    .callback = &onTimer,   // Ustawienie funkcji przerwania
    .name = "Timer1"        // Nazwa timera (opcjonalne)
  };

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

String onReceive(int packetSize) {
  if (packetSize == 0) return "NO_PACKET";          // if there's no packet, return

  // read packet header bytes:
  int recipient = LoRa.read();          // recipient address
  byte sender = LoRa.read();            // sender address
  byte INcounter = LoRa.read();         // incoming msg ID
  byte incomingLength = LoRa.read();    // incoming msg length


  String incoming = "";

  while (LoRa.available()) {
    incoming += (char)LoRa.read();
  }

  if (incomingLength != incoming.length()) {   // check length for error
    Serial.println("error: message length does not match length");
    return "ERROR_LENGTH";                             // skip rest of function
  }

  // if the recipient isn't this device or broadcast,
  if (recipient != localAddress && recipient != 0xFF) {
    Serial.println("This message is not for me.");
    return "ERROR_ADDRES";                             // skip rest of function
  }

  // if message is for this device, or broadcast, print details:
  //Serial.println("Received from: 0x" + String(sender, HEX));
  //Serial.println("Sent to: 0x" + String(recipient, HEX));
  if(CheckID(INcounter) == PASSED){
    Serial.println("PASSED");
  }else{
    Serial.println();
    Serial.println();
    Serial.println();
    Serial.println();
    Serial.println("----------ERROR-----------");
    Serial.println();
    Serial.println();
    Serial.println();
    Serial.println();
  }

  Serial.print("Message ID: " + String(INcounter));
  Serial.print("   ");
  Serial.print("Message length: " + String(incomingLength));
  Serial.print("   ");
  Serial.print("Message: " + incoming);
  Serial.print("   ");
  Serial.print("RSSI: " + String(LoRa.packetRssi()));
  Serial.print("   ");
  Serial.print("Snr: " + String(LoRa.packetSnr()));
  Serial.println();    

  return incoming;
}


//----------------------------------------MAIN PROGRAM----------------------------------------

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

  // Tworzenie timera
  esp_timer_create(&timer_args, &timer);
  Serial.println("Timer created");

  // Zapisanie czasu początkowego
  startTime = esp_timer_get_time();
  Serial.println("Start time saved");

  // Uruchomienie timera cyklicznego z okresem 1 sekundy (1000000 mikrosekund)
  //esp_timer_start_periodic(timer, 1000000);
  Serial.println("Timer started");


  Serial.println("LoRa init succeeded.");
}

void random_delay(uint minimum_ms, uint random_ms){

  // Generowanie losowego opóźnienia
  r_time = random(random_ms) + minimum_ms;
  Serial.print("Delay Time: ");
  Serial.println(r_time);
  delay(r_time); 
}

void loop() {
  // Obsługa wysyłania
  if (sending_flag == 1 && reciving_flag == 0) {
    sendMessage(TEST_MESSAGE);
    reciving_flag = 1;
    sending_flag = 0;
    test_flag = 0;
    Serial.println("----------WYSLANO----------");

    
    random_delay(5000,500);
  }
  // Obsługa odbierania
  else if (reciving_flag == 1 && sending_flag == 0) {
    test_flag == 1;
  Serial.println("----------odbieranie----------");
    if(onReceive(LoRa.parsePacket()) == TEST_MESSAGE || test_flag == 1 ){
      reciving_flag = 0;
      sending_flag = 1;
      test_flag = 0;
      Serial.println("----------ODEBRANO----------");
    }
  }
  // Obsługa stanu początkowego
  else {
    reciving_flag = 1;
    sending_flag = 0;
    Serial.println("Obsługa stanu początkowego");
  }
  test_flag = 1;
  if(test_flag == 1){
    Serial.print("test_flag: ");
    Serial.print(test_flag);
    Serial.print(" reciving_flag: ");
    Serial.print(reciving_flag);
    Serial.print(" sending_flag: ");
    Serial.println(sending_flag);
  }
  
 
}