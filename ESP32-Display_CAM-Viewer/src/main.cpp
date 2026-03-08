#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <TFT_eSPI.h>
#include <TJpg_Decoder.h>

const char *ssid = "ssid";
const char *password = "password";
const char* snapshotUrl = "snapshotUrl";

TFT_eSPI tft = TFT_eSPI();

bool tft_output(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t* bitmap) {
  if (y >= tft.height()) return false;
  tft.pushImage(x, y, w, h, bitmap);
  return true;
}

void setup() {
  Serial.begin(115200);
  pinMode(27, OUTPUT);
  digitalWrite(27, HIGH); 

  tft.begin();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);

  // Reduz 640x480 para 320x240 durante a decodificação
  TJpgDec.setJpgScale(2); 
  TJpgDec.setCallback(tft_output);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) { delay(500); }
  WiFi.setSleep(false);
  Serial.println("Conectado!");
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(snapshotUrl);
    
    int httpCode = http.GET();
    if (httpCode == HTTP_CODE_OK) {
      WiFiClient* stream = http.getStreamPtr();
      
      // 60KB é o limite seguro de RAM para o ESP32 sem PSRAM
      size_t maxLen = 60000; 
      uint8_t* buffer = (uint8_t*)malloc(maxLen);

      if (buffer) {
        // Lê o stream até o fim ou até encher os 60KB
        int bytesRead = stream->readBytes(buffer, maxLen);

        if (bytesRead > 0) {
          Serial.printf("Recebido: %d bytes. Desenhando...\n", bytesRead);
          
          tft.startWrite();
          // Usa a função de 4 argumentos que o seu compilador confirmou que existe
          TJpgDec.drawJpg(0, 0, buffer, bytesRead);
          tft.endWrite();
        }
        free(buffer);
      } else {
        Serial.println("Erro: RAM insuficiente para 60KB.");
      }
    }
    http.end();
  }
  delay(500); 
}