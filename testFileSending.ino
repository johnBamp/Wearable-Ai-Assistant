#include <WiFi.h>
#include <HTTPClient.h>
#include <SdFat.h>
#include <SPI.h>

SdFat SD;

const char* ssid = "TheLanai";
const char* password = "PennyLuke5776$";
const char* server_url = "http://10.0.0.106:5000/upload";

const int chipSelect = 15; // SD card CS pin

void setup() {
  Serial.begin(115200);
  while (!Serial) {}

  Serial.print("Initializing SD card...");

  if (!SD.begin(chipSelect, SPI_HALF_SPEED)) {
    Serial.println("Card failed or not present");
    while (1);
  }
  Serial.println("Card initialized.");

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  sendFileToServer("test.wav");
}

void loop() {

}

void sendFileToServer(const String &filename) {
  File audio = SD.open(filename.c_str());
  if (!audio) {
    Serial.println("Failed to open file for reading");
    return;
  }

  HTTPClient http;
  http.begin(server_url);
  http.addHeader("Content-Type", "application/octet-stream");
  http.addHeader("X-File-Name", filename);

  int httpCode = http.sendRequest("POST", &audio, audio.size());

  if (httpCode > 0) {
    String payload = http.getString();
    Serial.println("Server response:");
    Serial.println(payload);
  } else {
    Serial.print("Error on sending POST request: ");
    Serial.println(httpCode);
  }

  http.end();
  audio.close();
}
