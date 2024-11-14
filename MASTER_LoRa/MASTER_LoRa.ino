#include <SPI.h>
#include <LoRa.h>
#include "esp_timer.h"

int counter = 0;
esp_timer_handle_t timer;  // Uchwyt timera, tworzenie, uruchamianie i zatrzymywanie

uint64_t startTime = 0;
uint64_t endTime = 0;

void IRAM_ATTR onTimer(void* arg) { //IRAM_ATTR - onTimer jest umieszczany w RAM
  endTime = esp_timer_get_time() - startTime; // Odliczony czas od startu w mikrosekundach
}

void setup() {
  Serial.begin(115200);
  Serial.println("Serial begin");

  LoRa.setPins(18, 23, 26);
  Serial.println("Pins are set");

  LoRa.begin(868E6);
  Serial.println("LoraBegin 868E6");
  
  while (!Serial);
  Serial.println("Serial Passed");

  if (!LoRa.begin(868E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  Serial.println("LoraBegin 868E6 Passed");

  // Konfiguracja timera
  const esp_timer_create_args_t timer_args = {
    .callback = &onTimer,   // Ustawienie funkcji przerwania
    .name = "Timer1"        // Nazwa timera (opcjonalne)
  };
  Serial.println("Timer configured");

  // Tworzenie timera
  esp_timer_create(&timer_args, &timer);
  Serial.println("Timer created");

  // Zapisanie czasu początkowego
  startTime = esp_timer_get_time();
  Serial.println("Start time saved");

  // Uruchomienie timera cyklicznego z okresem 1 sekundy (1000000 mikrosekund)
  esp_timer_start_periodic(timer, 1000000);
  Serial.println("Timer started");

  Serial.println("Setup Finished");
}

void loop() {
  while(1){
  
  // send packet
  
  /*
  Serial.print("Sending packet: ");
  Serial.println(counter);

  LoRa.beginPacket();
  LoRa.print("hello Szczepan Wariacie 69 xD GOMBAO 33 KURFO");
  Serial.println("hello Szczepan Wariacie 69 xD ");
  LoRa.print(counter);
  Serial.println(counter);
  LoRa.endPacket();
 */
  counter++;

  Serial.print("Elapsed time (ms): ");
  Serial.println(endTime / 1000); // Konwersja mikrosekund na milisekundy

  delay(500);
  }
}
